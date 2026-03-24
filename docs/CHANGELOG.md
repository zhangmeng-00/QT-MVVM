# 修改日志

## 2026-03-24

### 变更范围
- 统一 Mediator Topic 的 Tag 定义，减少业务和核心代码中的硬编码字符串。
- 同步并完善当前框架实现对应的架构文档。
- 本次提交仅包含源码与文档，不包含构建产物和本地临时文件。

### 主要修改
- 新增统一 Tag 定义文件：
  - `app/Tags.h`
- 在构建脚本中纳入 Tag 头文件：
  - `CMakeLists.txt`

- 日志链路改为使用统一 Tag 常量：
  - `app/log/Logger.cpp`
  - `core/model/LogModel.cpp`
  - `core/viewmodel/LogListViewModel.cpp`

- 用户与传感器模块改为使用统一 Tag 常量：
  - `model/UserModel.cpp`
  - `viewmodel/UserViewModel.cpp`
  - `viewmodel/SensorViewModel.cpp`

- 调整核心头文件的 include 路径：
  - `core/actor/ActorObserve.h`
  - `core/mediator/Observe.h`
  - `core/mediator/Topic.h`

- 修改 Mediator 中与互斥锁相关的代码位置（当前为注释状态）：
  - `core/mediator/Mediator.cpp`

- 更新架构文档内容：
  - `docs/MediatorMVAQt 架构文档.md`

### 备注
- `run_trace.db` 删除以及 `build/`、`build_temp/` 等本地构建输出未纳入本次提交。
