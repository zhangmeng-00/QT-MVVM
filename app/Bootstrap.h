#pragma once

#include <QObject>
#include "AppConfig.h"

class AppContext;

// 你项目里的组件（按你现有代码）
class UserModel;
class SensorModel;
class LogModel;

namespace Bootstrap {

// 全局组件集合（需要哪些就加哪些）
struct CoreObjects {
    UserModel* userModel = nullptr;
    SensorModel* sensorModel = nullptr;
    LogModel* logModel = nullptr;
};

// 安装/装配（幂等，多次调用只会创建一次）
// 使用默认配置
CoreObjects InstallAll(AppContext& ctx);

// 使用自定义配置
CoreObjects InstallAll(AppContext& ctx, const AppConfig& config);

// 可选：如果你将来有 Stop/Close 逻辑，可以在这里统一做
void Shutdown(AppContext& ctx);

// 便捷 getter（可选）
UserModel* userModel();
SensorModel* sensorModel();
LogModel* logModel();

} // namespace Bootstrap
