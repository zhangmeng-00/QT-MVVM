# 📘 MediatorMVAQt 使用大纲（完整版）

> **MediatorMVAQt**
>  一个基于 **Qt + MVVM + Mediator + Command** 的应用架构，
>  面向 **中大型上位机 / 工业软件 / 可追溯系统 / 可回放系统**。

------

## 一、设计目标（Why）

### 1️⃣ 要解决的问题

传统 Qt 项目常见问题：

- UI `connect` 到业务 slot，耦合严重
- ViewModel 既管业务又管 UI enable
- 数据流向混乱（UI ↔ VM ↔ Model 双向）
- 行为不可记录、不可回放
- 权限 / 状态控制分散在各处

------

### 2️⃣ MediatorMVAQt 的核心目标

| 目标        | 说明                           |
| ----------- | ------------------------------ |
| UI 彻底解耦 | UI 不直接调用业务逻辑          |
| 行为对象化  | 所有业务行为都是 `Command`     |
| 数据单向流  | Publish → Mediator → Subscribe |
| 可追溯      | 所有行为 / 数据可记录          |
| 可演进      | 权限 / 回放 / 宏命令           |

------

## 二、总体架构（What）

### 1️⃣ 架构总览（核心共识）

```
UI (View)
  ↓ 只绑定 ICommand / Property
Binding（胶水层）
  ↓
ViewModel（业务入口）
  ↓ Publish(tag, value)
Mediator（消息中枢）
  ↓
Topic（按 tag 分发）
  ↓
Model / ViewModel / Actor（订阅者）
```

------

### 2️⃣ 架构关键词解释

| 名称      | 角色               |
| --------- | ------------------ |
| View      | 纯 UI，不含业务    |
| ViewModel | 业务入口 + 状态源  |
| Model     | 业务实体 / 计算    |
| Mediator  | 全局消息总线       |
| Topic     | tag 对应的分发节点 |
| Command   | 业务行为对象       |
| Binding   | UI 与 VM 的胶水    |

------

## 三、目录结构规范（Structure）

```
core/
├── mediator/        // Mediator / Topic
├── actor/           // LoggerActor / RecorderActor
├── policy/          // AlwaysPolicy / FilterPolicy
├── command/         // ICommand / SimpleCommand
├── binding/         // Binding / BindingCommand

model/
├── UserModel
├── SensorModel
└── ...

viewmodel/
├── BaseViewModel
├── UserViewModel
├── SensorViewModel
└── ...

view/
├── MainWindow
├── TraceViewer
└── ...

common/
```

> ✅ **强约定：**
>
> - `UI` 不 import `model`
> - `UI` 不 connect 到 `ViewModel slot`
> - `ViewModel` 不操作 UI 控件

------

## 四、Mediator 与数据流（How - Data）

### 1️⃣ Mediator 的职责

- 管理 Topic
- 接收 Publish
- 分发到 Subscribe
- 支持跨线程（QueuedConnection）

```
Publish("user/score", 123);
```

------

### 2️⃣ Topic + Policy

```
Subscribe("user/score", std::make_shared<AlwaysPolicy>());
```

Policy 用于：

- 条件订阅
- 权限过滤
- 模式过滤（Running / Idle）

------

### 3️⃣ ObserveData（唯一入口）

```
void ObserveData(const QString& tag,
                 const QVariant& value) override;
```

> **规则：**
>
> - 所有外部数据只能从这里进 ViewModel / Model
> - 禁止 UI 直接 set ViewModel 成员

------

## 五、Command 机制（How - Behavior）

### 1️⃣ 为什么必须用 Command

| 问题                | Command 的解决方案 |
| ------------------- | ------------------ |
| 行为不可追溯        | Command 可记录     |
| UI 行为不统一       | ICommand 统一入口  |
| enable/disable 混乱 | CanExecute         |

------

### 2️⃣ ICommand（终态接口）

```
class ICommand : public QObject {
    Q_OBJECT
public:
    virtual void Execute() = 0;
    virtual bool CanExecute() const = 0;

signals:
    void CanExecuteChanged(bool);
};
```

------

### 3️⃣ SimpleCommand（标准实现）

```
new SimpleCommand(
    [] { /* Execute */ },
    [] { return canExecute; }
);
```

------

### 4️⃣ CanExecute 的架构原则（非常重要）

> ❌ UI 不判断
>  ❌ Binding 不判断
>  ✅ ViewModel 是状态源
>  ✅ Command 投影状态

```
业务状态 → Command.CanExecute → UI enable
```

------

## 六、Binding 层（How - UI）

### 1️⃣ Binding 的定位

> **只做胶水，不做逻辑**

------

### 2️⃣ Property Binding

```
Binding::BindProperty(
    ui->labelScore,
    "text",
    m_userVM,
    "scoreText"
);
```

------

### 3️⃣ Command Binding（唯一方式）

```
BindingCommand::BindCommand(
    ui->btnPublishScore,
    m_userVM->publishScoreCommand()
);
```

------

### 4️⃣ UI 层强制规则

| 允许          | 禁止               |
| ------------- | ------------------ |
| 绑定 ICommand | connect 到 VM slot |
| 打开窗口      | 业务判断           |
| 显示数据      | enable / disable   |

------

## 七、MainWindow（Composition Root）

### 1️⃣ MainWindow 的职责

- 创建 Model / ViewModel
- 建立订阅
- 建立绑定
- **不包含业务逻辑**

------

### 2️⃣ 推荐初始化顺序

```
setupModels();
setupViewModels();
setupSubscriptions();
setupBindings();
```

------

## 八、Actor / Recorder（可追溯）

### 1️⃣ Actor 的角色

- 不参与 UI
- 订阅 Topic
- 记录 / 转发 / 存储

示例：

- LoggerActor
- SQLiteRecorderActor

------

### 2️⃣ Recorder 的典型用途

- 行为审计
- 测试回放
- TraceViewer 可视化

------

## 九、典型使用流程（完整链路）

### 1️⃣ 点击按钮 → 发布积分

```
QPushButton::clicked
→ ICommand::Execute
→ ViewModel::publishCommand
→ Publish("user/score", value)
→ Mediator
→ Topic
→ Model / VM / Actor
→ PropertyChanged
→ UI 更新
```

------

## 十、扩展指南（Evolution）

### 1️⃣ 权限控制

```
CanExecute = Role == Admin;
```

------

### 2️⃣ 运行状态锁（Running / Idle）

- 系统状态 → Topic
- 所有 Command 订阅 → 自动禁用

------

### 3️⃣ Command 记录（推荐）

```
RecordingCommand
  └── 装饰 ICommand
```

------

### 4️⃣ QAction / Shortcut 统一

```
BindCommand(QAction*, ICommand*);
BindCommand(QShortcut*, ICommand*);
```

------

## 十一、常见错误与红线（Pitfalls）

### ❌ UI connect 到 ViewModel slot

### ❌ ViewModel 直接 set UI 控件

### ❌ 在 UI 写 enable / disable

### ❌ ViewModel 主动调用 Model 方法

------

## 十二、适用场景总结

非常适合：

- 工业上位机
- 自动化测试软件
- 可追溯 / 审计系统
- 多设备异步系统
- 中长期维护项目

------

## 十三、一句话总结（可以写在 README 顶部）

> **MediatorMVAQt 是一套：
>  用 Command 管行为，用 Mediator 管数据，
>  用 ViewModel 管状态，用 Binding 解耦 UI 的 Qt 架构。**

------

