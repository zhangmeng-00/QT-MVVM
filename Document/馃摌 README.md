# 📘 README.md（正式版）

```
# MediatorMVAQt

MediatorMVAQt 是一套基于 **Qt + MVVM + Mediator + Command** 的应用架构，
用于构建 **解耦、可追溯、可演进** 的中大型 Qt 应用（工业上位机 / 测试系统）。

## ✨ 核心思想

- **UI 不直接调用业务逻辑**
- **所有业务行为必须对象化为 Command**
- **所有业务数据通过 Mediator 单向流转**
- **UI 状态由业务状态投影，而非 UI 自己判断**
```

UI → Command → ViewModel → Publish → Mediator → Subscribe

```
---

## 📐 总体架构
```

View (UI)
 └─ Binding (Property / Command)
 └─ ViewModel
 └─ Publish(tag, value)
 └─ Mediator
 └─ Topic
 └─ Model / ViewModel / Actor

```
---

## 📂 目录结构

​```text
core/
├── mediator/        // Mediator / Topic
├── command/         // ICommand / SimpleCommand
├── binding/         // Binding / BindingCommand
├── policy/          // AlwaysPolicy / FilterPolicy
├── actor/           // LoggerActor / RecorderActor

model/               // 纯业务模型
viewmodel/           // 业务入口 + 状态源
view/                // UI（MainWindow / Viewer）
```

------

## 🧠 设计原则（必须遵守）

- UI **只能绑定 Command / Property**
- UI **禁止 connect ViewModel slot**
- ViewModel **不操作 UI 控件**
- 所有状态变化必须来自 **Mediator**

------

## 🚀 典型流程（点击按钮）

1. QPushButton clicked
2. ICommand::Execute
3. ViewModel::publishCommand
4. Publish("tag", value)
5. Mediator → Topic → Subscribers
6. PropertyChanged → UI 更新

------

## 🔧 扩展能力

- Command 自动记录（SQLite）
- 行为回放 / 审计
- 权限控制（CanExecute）
- 系统状态锁（Running / Idle）
- QAction / Shortcut 统一入口

------

## 📌 一句话总结

> **用 Command 管行为，用 Mediator 管数据，用 ViewModel 管状态，用 Binding 解耦 UI。**

```
---

# 📊 架构类图（Mermaid）

> 👉 GitHub / VSCode / Typora / Obsidian 都可直接渲染

​```mermaid
classDiagram
    class View {
        UI Controls
    }

    class BindingCommand
    class Binding

    class ICommand {
        +Execute()
        +CanExecute()
    }

    class SimpleCommand

    class ViewModel {
        +Publish()
        +ObserveData()
    }

    class Mediator
    class Topic
    class Model
    class Actor

    View --> Binding
    View --> BindingCommand
    BindingCommand --> ICommand
    SimpleCommand --|> ICommand
    ViewModel --> SimpleCommand
    ViewModel --> Mediator
    Mediator --> Topic
    Topic --> Model
    Topic --> ViewModel
    Topic --> Actor
```

------

# ⏱ 行为时序图（Mermaid）

```
sequenceDiagram
    participant UI
    participant Command
    participant ViewModel
    participant Mediator
    participant Topic
    participant Subscriber

    UI->>Command: Execute()
    Command->>ViewModel: publishCommand()
    ViewModel->>Mediator: Publish(tag, value)
    Mediator->>Topic: Notify
    Topic->>Subscriber: ObserveData(tag, value)
```

------

# 🧪 Demo 使用文档（最小可运行）

## 1️⃣ Demo 场景

- 发布积分
- 发布温度
- 自动禁用按钮
- SQLite 记录行为
- TraceViewer 回放

------

## 2️⃣ 启动顺序（MainWindow）

```
setupModels();
setupViewModels();
setupSubscriptions();
setupBindings();
```

------

## 3️⃣ UI 绑定示例

```
BindingCommand::BindCommand(
    ui->btnPublishScore,
    m_userVM->publishScoreCommand()
);

Binding::BindProperty(
    ui->labelScore,
    "text",
    m_userVM,
    "scoreText"
);
```

------

## 4️⃣ 禁用按钮机制说明

- ViewModel 内维护业务状态（如 m_canPublish）
- Command 的 CanExecute 绑定该状态
- 状态变化 → RaiseCanExecuteChanged
- UI 自动 enable / disable

------

## 5️⃣ TraceViewer

- SQLiteRecorderActor 订阅所有 Topic
- 记录 Publish 行为
- TraceViewer 只读数据库展示

------

# ✅ 团队使用规范 Checklist（强制）

## 🚫 UI 层（红线）

-  ❌ connect 到 ViewModel slot
-  ❌ 在 UI 写 enable / disable
-  ❌ UI 直接调用 Publish

## ✅ UI 层（允许）

-  BindProperty
-  BindCommand
-  打开窗口（纯 UI 行为）

------

## 🚫 ViewModel 层（红线）

-  ❌ 操作 UI 控件
-  ❌ 主动调用 Model 方法
-  ❌ 双向数据流

------

## ✅ ViewModel 层（必须）

-  所有数据来自 ObserveData
-  所有行为通过 Command
-  CanExecute 控制状态

------

## 🚫 Command 层（红线）

-  ❌ 包含 UI 逻辑
-  ❌ 操作 Model
-  ❌ 保存业务状态

------

## ✅ Mediator（规则）

-  所有 Publish 都可被 Recorder 订阅
-  Topic 只做分发
-  Policy 决定“能不能收”

------

# 🏁 最终总结（给团队）

> **如果你在犹豫要不要 connect：
>  那你现在写的地方，一定不该 connect。**