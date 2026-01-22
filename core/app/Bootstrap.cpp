#include "core/app/Bootstrap.h"

#include "AppContext.h"

// 这些 include 根据你的实际路径调整
#include "model/UserModel.h"
#include "model/SensorModel.h"
#include "model/LoggerActor.h"
#include "model/SQLiteRecorderActor.h"

#include <QDebug>

// ========== 内部静态存储（应用级单例对象） ==========
static Bootstrap::CoreObjects g_core;
static bool g_installed = false;

namespace Bootstrap {

static void SetParentIfQObject(QObject* obj, QObject* parent)
{
    if (!obj || !parent) return;
    if (!obj->parent()) obj->setParent(parent);
}

CoreObjects InstallAll(AppContext& ctx)
{
    if (g_installed) {
        return g_core;
    }
    g_installed = true;

    qDebug() << "[Bootstrap] InstallAll";

    // 1) 创建 Model（业务层，全局共享）
    g_core.userModel   = new UserModel(&ctx);
    g_core.sensorModel = new SensorModel(&ctx);

    // 2) 创建基础设施（日志/记录）
    g_core.logger   = new LoggerActor(&ctx);
    g_core.recorder = new SQLiteRecorderActor("run_trace.db", &ctx);

    // 3) 统一 parent（确保生命周期由 AppContext 托管）
    SetParentIfQObject(g_core.userModel,   &ctx);
    SetParentIfQObject(g_core.sensorModel, &ctx);
    SetParentIfQObject(g_core.logger,      &ctx);
    SetParentIfQObject(g_core.recorder,    &ctx);

    // 4) 接入 Mediator（注意：如果你的 AppContext::ConnectObserve 已做线程安全 invoke，更稳）
    ctx.ConnectObserve(g_core.userModel);
    ctx.ConnectObserve(g_core.sensorModel);
    ctx.ConnectObserve(g_core.logger);
    ctx.ConnectObserve(g_core.recorder);

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

} // namespace Bootstrap
