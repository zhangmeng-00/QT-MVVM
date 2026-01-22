# MediatorMVAQt

Qt 5.15 + MVVM + Mediator + Observe + Binding + Command  
面向工业上位机 / 自动化测试软件的可维护架构 Demo。

---

## 目录

- [核心目标](#核心目标)
- [架构概览](#架构概览)
- [项目结构](#项目结构)
- [启动流程](#启动流程)
- [职责边界](#职责边界)
- [订阅策略（推荐：Model 内自订阅）](#订阅策略推荐model-内自订阅)
- [绑定与命令（BindProperty / BindCommand）](#绑定与命令bindproperty--bindcommand)
- [新增模块/新增模型的标准流程](#新增模块新增模型的标准流程)
- [常见问题](#常见问题)

---

## 核心目标

1. **解耦**：UI、业务、硬件、记录/追溯彼此低耦合  
2. **可追溯**：Command + Recorder 记录行为，支持回放/审计  
3. **可演进**：新增硬件/业务模块不改 UI 主流程  
4. **线程安全**：Mediator 运行在独立线程，所有操作通过 AppContext 串行投递

---

## 架构概览

- **AppContext**  
  应用容器（Composition Root），持有 `Mediator` 和其线程，提供线程安全的 `ConnectObserve()` 等接口。

- **Mediator / Topic**  
  消息总线与 Topic 路由。发布/订阅统一走 Topic。

- **Observe（基类）**  
  Model / ViewModel / Actor 都可以继承 Observe，获得 Publish/Subscribe 能力。

- **Model**（业务层）  
  业务状态、协议/驱动、算法、状态机。  
  ✅ 推荐：在 Model 内声明订阅（`SetupSubscriptions()`），高内聚。

- **ViewModel**（UI 逻辑层）  
  暴露 `Q_PROPERTY` 给 UI；提供 `ICommand*` 给 BindCommand。  
  不直接操作 UI，只操作状态与 Publish。

- **View(MainWindow/UI)**  
  只做：创建 VM、绑定属性、绑定命令。不负责创建 Model/驱动/日志器。

- **Bootstrap（core/app/Bootstrap.*）**  
  统一创建全局组件（Model/Logger/Recorder/Driver 等）并连接 Mediator。

---

## 项目结构

推荐结构（与你当前工程一致）：
core/
app/
AppContext.h/.cpp
Bootstrap.h/.cpp
mediator/
Mediator.h/.cpp
Topic*.h/.cpp
Observe.h/.cpp
binding/
Binding.h/.cpp # BindProperty
BindingCommand.h # BindCommand（通用 signal -> ICommand）
command/
ICommand.h
SimpleCommand.h
model/
UserModel.*
SensorModel.*
LoggerActor.*
SQLiteRecorderActor.*
viewmodel/
UserViewModel.*
SensorViewModel.*
view/
TraceViewer.*
MainWindow.ui
MainWindow.h/.cpp


---

## 启动流程

应用启动后，推荐流程如下：

1. `AppContext::instance()` 创建 Mediator 并启动 Mediator 线程  
2. `Bootstrap::InstallAll(AppContext::instance())`  
   - 创建全局组件（Model / Logger / Recorder / Driver…）
   - `ctx.ConnectObserve(obj)` 接入 Mediator  
   - 调用 logger/recorder 的 `Init()`  
3. `MainWindow` 创建 ViewModel，并 `ConnectObserve(vm)`  
4. `MainWindow` 做 UI 绑定（Property + Command）

> 重点：MainWindow 不再创建 Model/Logger/Recorder，只做 UI 装配。

---

## 职责边界

### AppContext（应用容器）
- 创建/持有 Mediator + 线程
- 提供线程安全调用：所有 mediator 操作通过 `invokeMethod(QueuedConnection)`
- 统一连接 Observe：`ConnectObserve(obj)`

### Bootstrap（装配入口）
- 创建全局 Model/Actor/Driver/Service
- 将组件接入 Mediator
- 做基础设施 Init（记录器、日志器、数据库等）

### Model（业务层）
- 业务状态、协议驱动、算法、状态机
- ✅ 推荐：Model 内声明订阅（自订阅），外部无需写 Subscribe
- 通过 Publish 输出业务状态（Topic）

### ViewModel（UI 逻辑层）
- 提供 Q_PROPERTY 给 UI（显示数据）
- 提供 ICommand 给 UI（点击、值改变等）
- 通过 Publish 触发业务动作或状态变化

### View（MainWindow）
- 创建 ViewModel（parent 为窗口）
- BindProperty 绑定显示
- BindCommand 绑定控件事件到 ICommand

---

## 订阅策略（推荐：Model 内自订阅）

### 为什么推荐？
- 订阅就是依赖声明，放在 Model 内可实现高内聚
- 外部装配只需 `ConnectObserve(model)`，无需散落 Subscribe

### 推荐实现方式

在 `Observe`（或你的业务基类）中添加：

```cpp
virtual void SetupSubscriptions() {}
Model 重写它：

void UserModel::SetupSubscriptions() override {
  Subscribe("user/score", std::make_shared<AlwaysPolicy>());
  Subscribe("user/level", std::make_shared<ValueChangedPolicy>());
}


在 AppContext::ConnectObserve() 中，连接后调用（建议在 mediator 线程执行）：

InvokeOnMediator([this, obs](){
  m_mediator->ConnectObserve(obs);
  obs->SetupSubscriptions();
});


这样外部只需要：

ctx.ConnectObserve(model);   // 自动完成订阅

绑定与命令（BindProperty / BindCommand）
BindProperty（属性绑定）

将 VM 的 Q_PROPERTY 映射到控件属性：

Binding::BindProperty(ui->labelScore, "text", m_userVM, "scoreText");

BindCommand（事件绑定）

将任意 QObject 的任意 signal 绑定到 ICommand，并传入 signal 参数：

BindingCommand::BindCommand(ui->btnLogin, &QAbstractButton::clicked, m_userVM->loginCommand());

BindingCommand::BindCommand(ui->horizontalSliderTemp, &QSlider::valueChanged, m_sensorVM->setTargetTemperatureCommand());

BindingCommand::BindCommand(ui->comboBoxMode,
    QOverload<int>::of(&QComboBox::currentIndexChanged),
    m_userVM->modeChangedCommand());


注意：有重载的 signal 必须用 QOverload 指定签名。

SimpleCommand（做法A：支持参数）

无参：SimpleCommand(std::function<void()>)

有参：SimpleCommand(std::function<void(const QVariantList&)>)

BindCommand 会把信号参数自动打包为 QVariantList 传入 ExecuteArgs(args)。
