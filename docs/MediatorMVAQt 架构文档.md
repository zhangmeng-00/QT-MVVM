# MediatorMVAQt 架构文档

## 1. 概述

MediatorMVAQt 是一个基于 Qt5 的桌面应用程序，采用 **MVVM** 架构模式，结合 **观察者模式 (Observer)**， **中介者模式 (Mediator)** 实现组件间的解耦通信。

### 1.1 架构目标

- **低耦合**：组件间通过 Mediator 通信，避免直接依赖
- **线程安全**：基于QT的信号槽模式确保消息串行处理
- **可维护**：清晰的职责分离，易于理解和扩展
- **可测试**：框架与业务逻辑分离

---

## 2. 整体架构

```
┌─────────────────────────────────────────────────────────────────────────┐
│                            MediatorMVAQt                                │
├─────────────────────────────────────────────────────────────────────────┤
│  ┌─────────────────────────────────────────────────────────────────────┐│
│  │                         View Layer (视图层)                          ││
│  │  ┌────────────┐  ┌────────────┐  ┌────────────┐  ┌────────────────┐ ││
│  │  │MainWindow  │  │LogListView │  │TraceViewer │  │   UI Controls  │ ││
│  │  └────────────┘  └────────────┘  └────────────┘  └────────────────┘ ││
│  └─────────────────────────────────────────────────────────────────────┘│
│                                    │                                    │
│                                    ▼                                    │
│  ┌─────────────────────────────────────────────────────────────────────┐│
│  │                      Binding Layer (绑定层)                          ││
│  │  ┌──────────────────────────────────────────────────────────────┐   ││
│  │  │  Binding::BindProperty() - 单向属性绑定 (ViewModel → View)     │   ││
│  │  └──────────────────────────────────────────────────────────────┘   ││
│  └─────────────────────────────────────────────────────────────────────┘│
│                                    │                                    │
│                                    ▼                                    │
│  ┌─────────────────────────────────────────────────────────────────────┐│
│  │                   ViewModel Layer (视图模型层)                        ││
│  │  ┌────────────┐  ┌────────────┐  ┌────────────────────────────┐     ││
│  │  │UserViewModel│ │SensorVM    │  │   BaseViewModel            │     ││
│  │  └────────────┘  └────────────┘  │  - 继承 ActorObserve        │     ││
│  │                                  │  - 处理 UI 交互事件          │     ││
│  │                                  │  - 发布业务状态变化           │    ││
│  │                                  └────────────────────────────┘     ││
│  └─────────────────────────────────────────────────────────────────────┘│
│                                    │                                     │
│                                    ▼                                     │
│  ┌─────────────────────────────────────────────────────────────────────┐│
│  │                     Model Layer (模型层)                             ││
│  │  ┌────────────┐  ┌────────────┐  ┌────────────┐  ┌──────────────┐   ││
│  │  │UserModel   │  │SensorModel │  │LogModel    │  │ModbusMaster  │   ││
│  │  │            │  │            │  │            │  │  Model       │   ││
│  │  └────────────┘  └────────────┘  └────────────┘  └──────────────┘   ││
│  │        │              │              │                │             ││
│  │        └──────────────┴──────────────┴────────────────┘             ││
│  │                               │                                     ││
│  │                    继承 BaseModel (ActorObserve)                    ││
│  └─────────────────────────────────────────────────────────────────────┘│
│                                                                         │
├─────────────────────────────────────────────────────────────────────────┤
│                        Core Framework (核心框架)                         │
│  ┌───────────────────────────────────────────────────────────────────┐ │
│  │                        Mediator Core                              │ │
│  │  ┌─────────┐   ┌─────────┐   ┌──────────┐   ┌─────────────────┐   │ │
│  │  │Mediator │◄──│  Topic  │◄──│  Observe │◄──│    Policy       │   │ │
│  │  │(中介者)  │   │ (主题)  │    │ (观察者) │   │   (订阅策略)     │   │ │
│  │  └─────────┘   └─────────┘   └──────────┘   └─────────────────┘   │ │
│  │       │                                                  │        │ │
│  │       │            ┌─────────────────────────────────────┘        │ │
│  │       │            ▼                                              │ │
│  │  ┌────▼───────────────────────────────────────────────────────┐    │ │
│  │  │                     Actor (ActorObserve)                   │    │ │
│  │  │  - 消息串行处理                                              │    │ │
│  │  │  - 线程归属 (可配置独立线程)                                  │    │ │
│  │  │  - Mailbox 模式确保线程安全                                  │    │ │
│  │  └───────────────────────────────────────────────────────────┘    │ │
│  └───────────────────────────────────────────────────────────────────┘ │
│                                    │                                    │
│                                    ▼                                    │
│  ┌─────────────────────────────────────────────────────────────────────┐│
│  │                    AppContext (应用上下文)                            ││
│  │  - 组件生命周期管理                                                   ││
│  │  - 依赖注入容器                                                       ││
│  │  - 线程安全调用封装                                                   ││
│  └─────────────────────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────────────────┘
```

---

## 3. 核心组件

### 3.1 Mediator (中介者)

**职责**：
- 管理 Topic（按 tag 创建/获取）
- 处理订阅：将 (observer + policy) 加入 Topic
- 处理发布：转发到 Topic->Notify()

**位置**：`core/mediator/Mediator.h`

**特点**：
- 线程安全（QMutex 保护）
- 支持 Sticky 模式（新订阅者立即收到最近状态）
- 支持多种订阅策略

```cpp
// 使用示例
Mediator* mediator = AppContext::instance().mediator();
mediator->ConnectObserve(observe);
observe->Subscribe("sensor/temperature", QVariant(0), std::make_shared<AlwaysPolicy>());
```

---

### 3.2 Topic (主题)

**职责**：
- 保存订阅者列表
- 管理订阅策略
- 消息分发
- 清理已析构的订阅者

**位置**：`core/mediator/Topic.h`

---

### 3.3 Observe (观察者)

**职责**：
- 发布消息
- 接收消息（虚函数 ObserveData）

**位置**：`core/mediator/Observe.h`

```cpp
class MyObserver : public Observe {
    void ObserveData(const QString& tag, const QVariant& value) override {
        if (tag == "sensor/temperature") {
            // 处理温度数据
        }
    }
};
```

---

### 3.4 Policy (订阅策略)

**职责**：决定订阅者何时收到消息

**类型**：
| Policy | 说明 |
|--------|------|
| `AlwaysPolicy` | 每次发布都通知 |
| `ValueChangedPolicy` | 值变化时才通知 |
| `StickyPolicy` | 订阅后立即收到最近值 |

**位置**：`core/policy/`

---

### 3.5 ActorObserve (参与者观察者)

**职责**：
- 拦截 Topic → Observe 的入口
- 把消息投递到对象所属线程
- 确保 ObserveData 串行执行

**位置**：`core/actor/ActorObserve.h`

**特点**：
- 支持独立线程运行
- Qt 事件循环保证串行

```cpp
// 在独立线程中运行
ActorObserve* actor = new ActorObserve(parent, true);
```

---

### 3.6 BaseModel (基础模型)

**职责**：
- 所有业务 Model 的基类
- 已具备 Actor 特性（串行、线程安全）

**位置**：`core/model/BaseModel.h`

---

### 3.7 BaseViewModel (基础视图模型)

**职责**：
- ViewModel 基类
- 处理 UI 交互事件
- 提供属性绑定

**位置**：`core/viewmodel/BaseViewModel.h`

**UI 事件处理**：
```cpp
// 可重写的事件处理
virtual void onClicked(const QString& senderId = QString()) {}
virtual void onPressed(const QString& senderId = QString()) {}
virtual void onToggled(bool checked, const QString& senderId = QString()) {}
virtual void onValueChanged(int value, const QString& senderId = QString()) {}
virtual void onTextChanged(const QString& text, const QString& senderId = QString()) {}
// ... 更多事件
```

---

### 3.8 Binding (属性绑定)

**职责**：
- 单向绑定 ViewModel 属性到 View 属性
- 自动同步初始值
- 属性变化时自动更新 View

**位置**：`core/binding/Binding.h`

**使用**：
```cpp
// 绑定标签文本
Binding::BindProperty(ui->label, "text", viewModel, "title");

// 绑定按钮启用状态
Binding::BindProperty(ui->pushButton, "enabled", viewModel, "canSave");

// 绑定模型到列表视图
Binding::BindProperty(ui->listView, "model", viewModel, "dataModel");
```

**支持绑定**：
| 控件 | 可绑定的属性 |
|------|-------------|
| QLabel | text, enabled, visible |
| QLineEdit | text, enabled, readOnly |
| QPushButton | text, enabled, visible |
| QWidget | enabled, visible |
| QProgressBar | value, minimum, maximum |
| QSlider | value, minimum, maximum |
| QSpinBox | value |
| QCheckBox | checked, enabled |
| QRadioButton | checked |
| QAbstractItemView | model |

---

### 3.9 AppContext (应用上下文)

**职责**：
- 单例管理全局状态
- 组件容器（生命周期托管）
- 线程安全调用封装
- Mediator 初始化和管理

**位置**：`app/AppContext.h`

**使用**：
```cpp
// 添加组件（自动连接 Observe）
UserModel* user = ctx.AddComponent<UserModel>();
SensorModel* sensor = ctx.AddComponent<SensorModel>();

// 线程安全调用
ctx.InvokeOnMediator([=]() {
    mediator->Publish("tag", value);
});
```

---

## 4. 数据流

### 4.1 发布-订阅流程

```
┌──────────┐     Publish      ┌───────────┐     Notify     ┌──────────┐
│  Model   │ ───────────────► │  Mediator │ ─────────────► │  Topic   │
│          │   (tag, value)   │           │                │          │
└──────────┘                  └───────────┘                └────┬─────┘
                                                                │
                                        ┌───────────────────────┘
                                        │ Policy 过滤
                                        ▼
┌──────────┐     ObserveData     ┌───────────────┐    ObserveData   ┌──────────┐
│  View    │ ◄────────────────── │ ActorObserve  │ ──────────────►  │  View    │
│  Model   │    (界面处理)        │               │   (业务处理)      │  Model   │
└──────────┘                     └───────────────┘                  └──────────┘
```

### 4.2 属性绑定流程

```
┌─────────────────┐   propertyChanged    ┌─────────────────┐   setProperty   ┌─────────────────┐
│   ViewModel     │ ──────────────────►  │ BindingHelper   │ ──────────────► │      View       │
│   (Q_PROPERTY)  │   notifySignal       │                 │                 │   (QLabel...)   │
└─────────────────┘                      └─────────────────┘                 └─────────────────┘
```

### 4.3 UI 事件流程

```
┌─────────────────┐   clicked() signal   ┌─────────────────┐   onClicked()    ┌─────────────────┐
│      View       │ ──────────────────►  │ BaseViewModel   │ ──────────────►  │    Business     │
│  (QPushButton)  │    senderId          │                 │   (重写处理)      │    Logic        │
└─────────────────┘                      └─────────────────┘                  └─────────────────┘
```

---

## 5. 目录结构

```
MediatorMVAQt/
├── app/                      # 应用启动层
│   ├── AppContext.h/cpp      # 应用上下文、全局容器
│   └── Bootstrap.h/cpp        # 组件安装/组装
│
├── core/                      # 核心框架（可复用）
│   ├── actor/
│   │   └── ActorObserve.h/cpp    # 参与者观察者（线程模型）
│   │
│   ├── binding/
│   │   ├── Binding.h             # 属性绑定
│   │   └── BindingHelper.h/cpp   # 绑定辅助
│   │
│   ├── mediator/
│   │   ├── Mediator.h/cpp        # 中介者
│   │   ├── Observe.h/cpp         # 观察者基类
│   │   └── Topic.h/cpp           # 主题
│   │
│   ├── model/
│   │   ├── BaseModel.h/cpp       # 模型基类
│   │   ├── LogEntry.h            # 日志条目
│   │   └── LogModel.h/cpp        # 日志模型
│   │
│   ├── policy/
│   │   ├── ISubscriptionPolicy.h # 策略接口
│   │   ├── AlwaysPolicy.h        # 始终通知
│   │   ├── ValueChangedPolicy.h  # 值变化通知
│   │   └── StickyPolicy.h        # 粘性策略
│   │
│   ├── view/
│   │   ├── LogListView.h/cpp     # 日志列表视图
│   │   └── LogListViewModel.h/cpp # 日志视图模型
│   │
│   └── viewmodel/
│       ├── BaseViewModel.h/cpp   # 视图模型基类
│       └── LogListViewModel.h/cpp
│
├── model/                     # 业务模型
│   ├── UserModel.h/cpp
│   ├── SensorModel.h/cpp
│   └── modbus/                # Modbus 通信
│       ├── ModbusMasterModel.h/cpp
│       ├── ModbusModuleBase.h/cpp
│       └── PidPressureModule.h/cpp
│
├── view/                      # 业务视图
│   ├── MainWindow.h/cpp
│   └── TraceViewer.h/cpp
│
├── viewmodel/                # 业务视图模型
│   ├── UserViewModel.h/cpp
│   └── SensorViewModel.h/cpp
│
└── main.cpp                  # 入口点
```

---

## 6. 使用指南

### 6.1 创建新的 Model

```cpp
// 1. 继承 BaseModel
class SensorModel : public BaseModel {
    Q_OBJECT
public:
    explicit SensorModel(QObject* parent = nullptr)
        : BaseModel(parent) {}

    // 发布数据
    void publishTemperature(double temp) {
        Publish("sensor/temperature", temp);
    }

protected:
    // 处理订阅消息
    void ObserveData(const QString& tag, const QVariant& value) override {
        if (tag == "command/reset") {
            // 处理重置命令
        }
    }
};
```

### 6.2 创建新的 ViewModel

```cpp
// 1. 继承 BaseViewModel
class TemperatureViewModel : public BaseViewModel {
    Q_OBJECT
    Q_PROPERTY(double temperature READ temperature NOTIFY temperatureChanged)

public:
    explicit TemperatureViewModel(QObject* parent = nullptr)
        : BaseViewModel(parent) {
        // 订阅传感器数据
        Subscribe("sensor/temperature", QVariant(0), std::make_shared<StickyPolicy>());
    }

    double temperature() const { return m_temperature; }

signals:
    void temperatureChanged();

protected:
    void ObserveData(const QString& tag, const QVariant& value) override {
        if (tag == "sensor/temperature") {
            m_temperature = value.toDouble();
            emit temperatureChanged();
        }
    }

    // 处理按钮点击
    void onClicked(const QString& senderId) override {
        if (senderId == "btnRefresh") {
            // 刷新数据
        }
    }

private:
    double m_temperature = 0.0;
};
```

### 6.3 创建新的 View

```cpp
// 1. 在 MainWindow 中组装
void MainWindow::setupViewModels() {
    m_tempVM = new TemperatureViewModel(this);
}

void MainWindow::setupBindings() {
    // 绑定温度显示
    Binding::BindProperty(ui->labelTemp, "text", m_tempVM, "temperature");

    // 绑定刷新按钮
    Binding::BindProperty(ui->btnRefresh, "enabled", m_tempVM, "canRefresh");

    // 绑定按钮点击事件
    connect(ui->btnRefresh, &QPushButton::clicked,
            [this]() { m_tempVM->onClicked("btnRefresh"); });
}
```

---

## 7. 线程模型

### 7.1 默认模式

```
┌─────────────────────────┐
│      Main Thread        │
│  (Qt GUI Thread)        │
│                         │
│  ┌─────────────────┐    │
│  │   MainWindow    │    │
│  │   ViewModels    │    │
│  └─────────────────┘    │
│           │             │
│           │ Qt::Queued  │
│           ▼             │
│  ┌─────────────────┐    │
│  │   Mediator      │    │  ◄── 也在主线程
│  │   (同进程)       │    │
│  └─────────────────┘    │
└─────────────────────────┘
```

### 7.2 独立线程模式

```
┌─────────────────────────┐
│      Main Thread        │
│  (Qt GUI Thread)        │
│                         │
│  ┌─────────────────┐    │
│  │   MainWindow    │    │
│  │   ViewModels    │    │
│  └─────────────────┘    │
│           │             │
│           │ Qt::Queued  │
│           ▼             │
│  ┌─────────────────┐    │
│  │   Mediator      │    │  ◄── 也在主线程
│  │   (同进程)       │    │
│  └─────────────────┘    │
└─────────────────────────┘
            │ Qt::Queued
            ▼
┌─────────────────────────┐
│    Worker Thread        │
│                         │
│  ┌─────────────────┐    │
│  │   Model         │    │
│  │   (ActorObserve)│    │
│  │   - Mailbox     │    │
│  │   - 串行处理     │    │
│  └─────────────────┘    │
└─────────────────────────┘
```

---

## 8. 当前架构缺点及优化方向

### 8.1 依赖注入缺失
- 当前直接 `new` 具体类型，难以测试
- 建议：引入工厂模式或 DI 容器

### 8.2 Binding 功能不完整
- 仅支持单向绑定 (VM → View)
- 缺少命令绑定 (按钮点击等)
- 建议：扩展 Binding 层

### 8.3 错误处理机制缺失
- Mediator 没有错误回调
- 建议：增加错误处理策略

### 8.4 配置管理分散
- 配置硬编码散落各处
- 建议：统一配置管理

### 8.5 日志系统薄弱
- 建议：封装统一日志服务

### 8.6 测试覆盖缺失
- 建议：添加单元测试

---

## 9. 附录

### 9.1 Tag 命名约定

```
# 传感器数据
sensor/temperature
sensor/pressure
sensor/humidity

# 用户操作
user/login
user/logout
user/logging

# 命令
command/start
command/stop
command/reset

# 状态
status/connection
status/error
```

### 9.2 Policy 选择指南

| 场景 | 推荐 Policy |
|------|------------|
| 实时状态显示 | `AlwaysPolicy` |
| 节省资源、只在值变化时更新 | `ValueChangedPolicy` |
| 新界面需要显示当前状态 | `StickyPolicy` |
| 复杂条件判断 | 自定义 Policy |

---

*文档版本: 1.0*
*最后更新: 2026-03-18*
