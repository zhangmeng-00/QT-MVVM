# MediatorMVAQt 架构文档

## 1. 概述

MediatorMVAQt 是一个基于 Qt5 的桌面应用程序，采用 **MVVM** 架构模式，结合 **观察者模式 (Observer)**、**中介者模式 (Mediator)** 和 **Actor 模式** 实现组件间的解耦通信。

### 1.1 架构目标

- **低耦合**：组件间通过 Mediator 通信，避免直接依赖
- **线程安全**：基于 Qt 的信号槽模式和 Actor Mailbox 机制确保消息串行处理
- **可维护**：清晰的职责分离，易于理解和扩展
- **可测试**：框架与业务逻辑分离，支持依赖注入

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
│  │                    继承 BaseModel (ActorObserve + IModel)            ││
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
│  │  │  - 消息串行处理 (Mailbox 模式)                              │    │ │
│  │  │  - 线程归属 (可配置独立线程)                                  │    │ │
│  │  │  - 通过 Qt::QueuedConnection 跨线程                          │    │ │
│  │  └───────────────────────────────────────────────────────────┘    │ │
│  └───────────────────────────────────────────────────────────────────┘ │
│                                    │                                    │
│                                    ▼                                    │
│  ┌─────────────────────────────────────────────────────────────────────┐│
│  │                    AppContext (应用上下文)                            ││
│  │  - 组件生命周期管理                                                   ││
│  │  - 依赖注入容器                                                       ││
│  │  - 线程安全调用封装                                                   ││
│  │  - Mediator 初始化和管理                                             ││
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
- 线程安全（QMutex 保护）

**位置**：`core/mediator/Mediator.h`

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
- 消息分发（支持 Policy 过滤）
- 清理已析构的订阅者

**位置**：`core/mediator/Topic.h`

---

### 3.3 Observe (观察者基类)

**职责**：
- 提供 `Publish()` 方法发布消息
- 提供 `Subscribe()` 方法订阅消息

**位置**：`core/mediator/Observe.h`

```cpp
// 基础观察者（直接接收消息，无线程转换）
class MyObserver : public Observe {
    void ObserveData(const QString& tag, const QVariant& value) override {
        if (tag == "sensor/temperature") {
            // 处理温度数据
        }
    }
};
```

---

### 3.4 ActorObserve (参与者观察者)

**职责**：
- 拦截 Topic → Observe 的入口
- 把消息投递到对象所属线程
- 确保 ObserveData 串行执行（Mailbox 模式）

**位置**：`core/actor/ActorObserve.h`

**特点**：
- 支持独立线程运行（`useSeparateThread = true`）
- Qt 事件循环保证串行处理
- 通过 Qt::QueuedConnection 跨线程调用

```cpp
// 在独立线程中运行
ActorObserve* actor = new ActorObserve(parent, true);

// 默认在主线程运行
ActorObserve* actor = new ActorObserve(parent, false);
```

---

### 3.5 Policy (订阅策略)

**职责**：决定订阅者何时收到消息

**类型**：

| Policy | 说明 | 参数 |
|--------|------|------|
| `AlwaysPolicy` | 每次发布都通知 | `replayLastValue` (默认 false) |
| `ValueChangedPolicy` | 值变化时才通知 | `replayLastValue` (默认 false) |

**使用 Sticky 行为**：
```cpp
// 使用 AlwaysPolicy(true) 实现新订阅者立即收到最近值
Subscribe("user/logList", defaultValue, std::make_shared<AlwaysPolicy>(true));
```

**位置**：`core/policy/`

---

### 3.6 IModel (模型接口)

**职责**：
- 所有业务 Model 实现此接口
- 统一生命周期管理
- 支持依赖注入和工厂创建

**位置**：`core/model/IModel.h`

```cpp
class IModel {
public:
    virtual ~IModel() = default;

    // 模型标识
    virtual QString modelName() const = 0;

    // 生命周期
    virtual void initialize() = 0;
    virtual void shutdown() = 0;

    // 调试信息
    virtual QString debugInfo() const = 0;
};
```

---

### 3.7 BaseModel (基础模型)

**职责**：
- 所有业务 Model 的基类
- 继承 ActorObserve + IModel
- 已具备 Actor 特性（串行、线程安全）

**位置**：`core/model/BaseModel.h`

```cpp
class UserModel : public BaseModel {
    Q_OBJECT
public:
    explicit UserModel(QObject* parent = nullptr)
        : BaseModel(parent) {}

    // 实现 IModel 接口
    QString modelName() const override { return "UserModel"; }

    void initialize() override {
        SetupSubscriptions();
    }

protected:
    void ObserveData(const QString& tag, const QVariant& value) override {
        // 处理订阅消息
    }
};
```

---

### 3.8 BaseViewModel (基础视图模型)

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
virtual void onValueChangedDouble(double value, const QString& senderId = QString()) {}
virtual void onTextChanged(const QString& text, const QString& senderId = QString()) {}
virtual void onCurrentIndexChanged(int index, const QString& senderId = QString()) {}
// ... 更多事件
```

---

### 3.9 Logger (日志服务)

**职责**：
- 统一日志记录入口
- 继承 ActorObserve，通过 Publish 发布日志
- 日志级别过滤
- 同时输出到控制台和 Mediator

**位置**：`app/log/Logger.h`

**使用方式**：
```cpp
// 使用便捷宏（推荐）
LOG_DEBUG("Module", "debug message");
LOG_INFO("Module", "info message");
LOG_WARN("Module", "warning message");
LOG_ERROR("Module", "error message");

// 或直接调用
Logger::instance()->info("Module", "message");
```

**特点**：
- 单例模式，全局可访问
- 支持日志级别过滤
- 自动发布到 `user/logging` Topic
- 发出 `logEmitted` 信号用于 UI 实时显示

---

### 3.10 LogModel (日志模型)

**职责**：
- 订阅 `user/logging` Topic
- 将日志存储到 SQLite 数据库
- 提供 `user/logList` Topic 发布最近日志列表

**位置**：`core/model/LogModel.h`

**数据库表结构**：
- `applications` - 应用表
- `log_levels` - 日志级别表
- `log_sources` - 日志来源表
- `log_entries` - 日志条目表

---

### 3.11 Binding (属性绑定)

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

### 3.12 AppContext (应用上下文)

**职责**：
- 单例管理全局状态
- 组件容器（生命周期托管）
- 线程安全调用封装
- Mediator 初始化和管理

**位置**：`app/AppContext.h`

**使用**：
```cpp
// 连接观察者到 Mediator
ctx.ConnectObserve(userModel);
ctx.ConnectObserve(sensorModel);

// 线程安全调用
ctx.InvokeOnMediator([=]() {
    mediator->Publish("tag", value);
});

// 获取 Mediator
Mediator* mediator = ctx.mediator();
```

---

### 3.13 Bootstrap (启动引导)

**职责**：
- 应用组件组装
- 配置初始化
- 所有核心组件的创建和连接

**位置**：`app/Bootstrap.h`

```cpp
// 在 MainWindow 中调用
Bootstrap::InstallAll(AppContext::instance());

// 或带配置
AppConfig config;
Bootstrap::InstallAll(AppContext::instance(), config);
```

---

### 3.14 AppConfig (应用配置)

**职责**：
- 集中管理所有配置项
- 支持 Modbus、Logging 等配置

**位置**：`app/config/AppConfig.h`

```cpp
struct ModbusConfig {
    QString host = "127.0.0.1";
    int port = 502;
    int timeout = 1000;      // 读取超时(ms)
    int pollInterval = 200;  // 轮询间隔(ms)
};

struct LoggingConfig {
    QString minLevel = "Info";
    int maxCount = 1000;
    QString databasePath = "logs.db";
};

struct AppConfig {
    ModbusConfig modbus;
    LoggingConfig logging;
    QString appName = "MediatorMVAQt";
    QString version = "1.0.0";
};
```

---

### 3.15 Modbus 通信模块

#### 3.15.1 ModbusMasterModel

**职责**：
- 管理 Modbus 连接（TCP/RTU）
- 统一轮询调度
- 读写操作

**位置**：`model/modbus/ModbusMasterModel.h`

#### 3.15.2 ModbusModuleBase

**职责**：
- Modbus 模块基类
- 不创建连接，不做轮询调度
- 只负责：描述要读哪些寄存器、解析寄存器为业务语义、发布 state/*、订阅并处理 cmd/*

**位置**：`model/modbus/ModbusModuleBase.h`

```cpp
class PidPressureModule : public ModbusModuleBase {
public:
    // 构造读取请求（只描述"读什么"）
    QModbusDataUnit buildReadUnit() override {
        return QModbusDataUnit(QModbusDataUnit::HoldingRegisters, m_baseReg - 1, m_stride);
    }

    // 解析 Modbus 响应，发布 state
    void handleReadResult(const QModbusDataUnit& unit) override {
        for (const auto& p : m_points) {
            QVariant value = parsePoint(p, unit);
            Publish(topicBase() + "/state/" + p.key, value);
        }
    }

    // 处理控制命令
    void ObserveData(const QString& tag, const QVariant& value) override {
        if (tag.endsWith("/cmd/set_sp")) {
            m_master->writeFloat(m_unitId, m_baseReg + 2, value.toDouble());
        }
    }
};
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

### 4.2 日志流程

```
┌──────────┐   LOG_INFO()    ┌────────┐   Publish    ┌──────────┐   存储     ┌─────────┐
│  业务代码 │ ──────────────► │ Logger │ ──────────► │ Mediator │ ────────► │ LogModel │
│          │                 │        │   (logging)  │          │           │ (SQLite) │
└──────────┘                 └────────┘              └──────────┘           └─────────┘
                                        │
                                        ▼
                              ┌─────────────────┐
                              │   LogListView    │  ◄── 订阅 user/logList
                              └─────────────────┘
```

### 4.3 属性绑定流程

```
┌─────────────────┐   propertyChanged    ┌─────────────────┐   setProperty   ┌─────────────────┐
│   ViewModel     │ ──────────────────►  │ BindingHelper   │ ──────────────► │      View       │
│   (Q_PROPERTY)  │   notifySignal       │                 │                 │   (QLabel...)   │
└─────────────────┘                      └─────────────────┘                 └─────────────────┘
```

### 4.4 UI 事件流程

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
│   ├── Bootstrap.h/cpp      # 组件安装/组装
│   ├── config/               # 配置相关
│   │   ├── AppConfig.h       # 配置结构体
│   │   └── ConfigLoader.h   # 配置加载器
│   ├── di/                   # 依赖注入
│   │   ├── IModelFactory.h   # 模型工厂接口
│   │   └── ModelFactory.h    # 模型工厂实现
│   └── log/                  # 日志服务
│       ├── Logger.h
│       └── Logger.cpp
│
├── core/                      # 核心框架（可复用）
│   ├── actor/
│   │   └── ActorObserve.h/cpp    # 参与者观察者（线程模型）
│   │
│   ├── binding/
│   │   ├── Binding.h             # 属性绑定
│   │   └── BindingHelper.h/cpp    # 绑定辅助
│   │
│   ├── mediator/
│   │   ├── Mediator.h/cpp        # 中介者
│   │   ├── Observe.h/cpp         # 观察者基类
│   │   └── Topic.h/cpp           # 主题
│   │
│   ├── model/
│   │   ├── BaseModel.h/cpp       # 模型基类
│   │   ├── IModel.h              # 模型接口
│   │   ├── LogEntry.h            # 日志条目结构
│   │   └── LogModel.h/cpp        # 日志模型
│   │
│   ├── policy/
│   │   ├── ISubscriptionPolicy.h # 策略接口
│   │   ├── AlwaysPolicy.h         # 始终通知 (支持 replayLastValue)
│   │   └── ValueChangedPolicy.h  # 值变化通知
│   │
│   ├── view/
│   │   ├── LogListView.h/cpp     # 日志列表视图
│   │   └── LogListViewModel.h/cpp # 日志视图模型
│   │
│   └── viewmodel/
│       ├── BaseViewModel.h/cpp  # 视图模型基类
│       └── LogListViewModel.h/cpp
│
├── model/                     # 业务模型
│   ├── UserModel.h/cpp
│   ├── SensorModel.h/cpp
│   └── modbus/                # Modbus 通信
│       ├── ModbusMasterModel.h/cpp
│       ├── ModbusModuleBase.h/cpp
│       ├── PidPressureModule.h/cpp
│       └── PointDef.h          # 点位定义
│
├── view/                      # 业务视图
│   ├── MainWindow.h/cpp
│   └── TraceViewer.h/cpp      # 轨迹查看器
│
├── viewmodel/                # 业务视图模型
│   ├── UserViewModel.h/cpp
│   └── SensorViewModel.h/cpp
│
├── docs/                      # 文档
│   └── MediatorMVAQt 架构文档.md
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

    // 实现 IModel 接口
    QString modelName() const override { return "SensorModel"; }

    void initialize() override {
        SetupSubscriptions();
    }

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
        // 订阅传感器数据，使用 AlwaysPolicy(true) 实现 Sticky 效果
        Subscribe("sensor/temperature", QVariant(0), std::make_shared<AlwaysPolicy>(true));
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

### 6.3 使用 Logger

```cpp
#include "Logger.h"

// 在任何地方使用
void someFunction() {
    LOG_INFO("ModuleName", "操作成功");
    LOG_WARN("ModuleName", "警告信息");
    LOG_ERROR("ModuleName", "错误: " + errorMessage);
}
```

### 6.4 创建 Modbus 模块

```cpp
class PressureModule : public ModbusModuleBase {
public:
    PressureModule(int instanceId, int unitId, int baseReg, QObject* parent)
        : ModbusModuleBase("pressure", instanceId, parent),
          m_unitId(unitId), m_baseReg(baseReg) {
        // 订阅控制命令
        Subscribe(topicBase() + "/cmd/set_sp", QVariant(0.0), std::make_shared<AlwaysPolicy>());
    }

    QModbusDataUnit buildReadUnit() override {
        return QModbusDataUnit(QModbusDataUnit::HoldingRegisters, m_baseReg - 1, 10);
    }

    void handleReadResult(const QModbusDataUnit& unit) override {
        // 解析寄存器，发布 state
        float pressure = parseFloat(unit.value(0), unit.value(1));
        Publish(topicBase() + "/state/pressure", pressure);
    }

    void ObserveData(const QString& tag, const QVariant& value) override {
        if (tag.endsWith("/cmd/set_sp")) {
            m_master->writeFloat(m_unitId, m_baseReg + 2, value.toDouble());
        }
    }

private:
    int m_unitId;
    int m_baseReg;
};
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
│  │   Mediator      │    │  ◄── 也在主线程
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
│           │              │
│           │ Qt::Queued  │
│           ▼              │
│  ┌─────────────────┐    │
│  │   Mediator      │    │  ◄── 也在主线程
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

## 8. Tag 命名约定

```
# 传感器数据 (state/*)
sensor/temperature
sensor/pressure

# 控制命令 (cmd/*)
command/start
command/stop
command/reset

# 用户操作
user/login
user/logout

# 日志系统
user/logging        # 日志发布
user/logList        # 日志列表

# 业务状态
user/score
user/level

# Modbus 模块状态 (模块名/实例/state/*)
pid_pressure/0/state/value
pid_pressure/0/cmd/set_sp
```

---

## 9. Policy 选择指南

| 场景 | 推荐 Policy |
|------|------------|
| 实时状态显示 | `AlwaysPolicy` |
| 节省资源、只在值变化时更新 | `ValueChangedPolicy` |
| 新界面需要显示当前状态 | `AlwaysPolicy(true)` |
| 复杂条件判断 | 自定义 Policy |

---

*文档版本: 1.1*
*最后更新: 2026-03-24*