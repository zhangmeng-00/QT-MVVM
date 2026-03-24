#include "Bootstrap.h"

#include "AppContext.h"
#include "config/AppConfig.h"

// 这些 include 根据你的实际路径调整
#include "UserModel.h"
#include "SensorModel.h"
#include "LogModel.h"
#include "Logger.h"

#include <QDebug>

// ========== 内部静态存储（应用级单例对象） ==========
static Bootstrap::CoreObjects g_core;
static bool g_installed = false;
static AppConfig g_config;  // 保存配置副本

namespace Bootstrap {

static void SetParentIfQObject(QObject* obj, QObject* parent)
{
    if (!obj || !parent) return;
    if (obj->thread() != parent->thread()) {
        qWarning() << "[Bootstrap] Skip setParent due to cross-thread object:" << obj;
        return;
    }
    if (!obj->parent()) obj->setParent(parent);
}

CoreObjects InstallAll(AppContext& ctx)
{
    // 使用默认配置
    AppConfig defaultConfig;
    return InstallAll(ctx, defaultConfig);
}

CoreObjects InstallAll(AppContext& ctx, const AppConfig& config)
{
    if (g_installed) {
        return g_core;
    }
    g_installed = true;
    g_config = config;  // 保存配置

    qDebug() << "[Bootstrap] InstallAll with config:" << config.appName << config.version;

    // 1) 创建 Model（业务层，全局共享）
    g_core.userModel   = new UserModel(&ctx);
    g_core.sensorModel = new SensorModel(&ctx);
    g_core.logModel    = new LogModel(nullptr, true); // 不传递父对象，以便能移动到新线程

    // 2) 统一 parent（确保生命周期由 AppContext 托管）
    SetParentIfQObject(g_core.userModel,   &ctx);
    SetParentIfQObject(g_core.sensorModel, &ctx);
    SetParentIfQObject(g_core.logModel,    &ctx);

    // 3) 接入 Mediator（注意：如果你的 AppContext::ConnectObserve 已做线程安全 invoke，更稳）
    ctx.ConnectObserve(g_core.userModel);
    ctx.ConnectObserve(g_core.sensorModel);
    ctx.ConnectObserve(g_core.logModel);

    // 4) Logger 也需要接入 Mediator（单例，父对象设为 ctx）
    ctx.ConnectObserve(Logger::instance());

    return g_core;
}

void Shutdown(AppContext& ctx)
{
    Q_UNUSED(ctx);

    // 目前你 logger/recorder 只有 Init，没有 Stop，就先留空
    // 如果你后面加 Stop()/Close()/Flush()，就在这里统一调用并控制顺序
    qDebug() << "[Bootstrap] Shutdown";
}

UserModel* userModel() { return g_core.userModel; }
SensorModel* sensorModel() { return g_core.sensorModel; }
LogModel* logModel() { return g_core.logModel; }

// 获取当前配置
const AppConfig& currentConfig() { return g_config; }

} // namespace Bootstrap
