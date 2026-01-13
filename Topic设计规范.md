

# 📘 Topic 设计规范（MediatorMVAQt）

> **文件名：Topic_Design_Specification.md**  
> 适用架构：MediatorMVAQt  
> 适用范围：Model / ViewModel / Actor / Recorder

---

## 1. 文档目的

本规范用于统一 **MediatorMVAQt** 中 **Topic Tag 的命名、语义、版本化与演进规则**，  
以保证系统在 **长期维护、多模块协作、多人员开发** 场景下：

- 语义清晰  
- 行为可预期  
- 可演进而不破坏旧逻辑  

> **Topic 一旦发布，即视为系统 API，不得随意变更语义。**

---

## 2. Topic 的核心分类（强制）

所有 Topic **必须且只能**属于以下两类之一：

### 2.1 事件型 Topic（Event Topic）

表示：**某件事情在某一刻发生**

- 一次性  
- 不可重放  
- 新订阅者不需要知道过去发生的事件  

**示例：**

```text
system/start
system/stop
user/login
test/step_finished
sensor/sample_ready
```

---

### 2.2 状态型 Topic（State Topic）

表示：**系统当前处于什么状态**

- 持续存在  
- 会被新值覆盖  
- 新订阅者必须立刻知道当前值  

**示例：**
```text
system/state
user/score
user/level
sensor/temperature
```

---

## 3. Topic 命名总规则（强制）

### 3.1 命名格式

```text
<domain>/<subject>/<verb_or_state>
```

或简化为：

```text
<domain>/<subject>
```

| 部分          | 含义               |
| ------------- | ------------------ |
| domain        | 系统域 / 子系统    |
| subject       | 业务对象           |
| verb_or_state | 动作（事件）或状态 |

### 3.2 命名语义规则

| 类型 | 命名要求 |
|----|----|
| 事件型 Topic | 使用动词 |
| 状态型 Topic | 使用名词 |
| UI 行为 | ❌ 禁止出现在 Topic |

---

## 4. 事件型 Topic 设计规范

### 4.1 命名规则

```text
<domain>/<verb>
<domain>/<subject>/<verb>
```

**正确示例：**
```text
system/start
user/logout
test/start
```

**禁止示例：**
```text
❌ system/is_running
❌ user/logged_in
```

### 4.2 语义约束

- 不保证新订阅者收到  
- 不用于 UI 初始化  
- 不代表当前状态  

---

## 5. 状态型 Topic 设计规范

### 5.1 命名规则

```text
<domain>/<subject>
<domain>/<subject>/state
```

**正确示例：**
```text
system/state
user/logged_in
sensor/temperature
```

### 5.2 语义约束

- 表示当前事实  
- 新订阅者必须立刻知道  
- 可被记录与回放  

---

## 6. Topic 版本化设计（核心）

### 6.1 什么时候必须升级版本

- 语义发生变化  
- 数据结构发生变化  
- 新旧订阅者存在歧义风险  

### 6.2 推荐版本化方式

**方式一：路径版本化（推荐）**
```text
system/v1/state
system/v2/state
```

**方式二：后缀版本化**
```text
system/state@v1
system/state@v2
```

### 6.3 版本演进规则

- 新版本必须新建 Topic  
- 旧版本至少保留一个完整周期  
- 禁止覆盖升级  

---

## 7. Model / ViewModel 使用规范

### Model

- Publish 状态型 Topic  
- Subscribe 事件型 Topic  

### ViewModel

- Subscribe 状态型 Topic  
- Command Publish 事件型 Topic  

---

## 8. 推荐系统级 Topic 示例

```text
system/start
system/stop
system/state
system/error
system/online
```

---

## 9. 明确禁止的 Topic（红线）

```text
❌ ui/xxx
❌ btn/xxx
❌ data/update
❌ do_something
```

---

## 10. Topic 设计自检清单

1. 是事件还是状态？  
2. 新订阅者是否需要？  
3. 是否需要版本化？  
4. 语义是否唯一？  
5. UI 更换后是否仍成立？  

---

## 11. 总结

> **事件用动词，状态用名词；  
> 发生过 ≠ 现在是；  
> Topic 一旦发布，即是契约。**

# 🧭 Topic 决策速查表

## Event Topic vs State Topic（MediatorMVAQt）

> **目的**：在新增 Topic 前，用 30 秒判断
>  👉 这是事件？还是状态？

------

## 一、先问这 3 个问题（核心）

### ❓Q1：它表示的是——

- ⭕ **发生了一件事** → **Event**
- ⭕ **现在是什么情况** → **State**

------

### ❓Q2：新订阅者是否必须立刻知道？

- ❌ 不需要 → **Event**
- ✅ 必须知道 → **State**

------

### ❓Q3：这个信息有没有“当前值”的概念？

- ❌ 没有，只是一瞬间 → **Event**
- ✅ 有一个“现在值” → **State**

------

👉 **3 个问题中，只要有一个指向 State，就优先考虑 State**

------

## 二、快速对照表（最常用）

| 对比项     | Event Topic | State Topic |
| ---------- | ----------- | ----------- |
| 语义       | 发生过      | 现在是      |
| 是否持续   | ❌           | ✅           |
| 是否可重放 | ❌           | ✅           |
| 新订阅者   | 不关心      | 必须知道    |
| 命名       | 动词        | 名词        |
| 常见来源   | Command     | Model       |
| 常见消费方 | Model / VM  | ViewModel   |
| UI 初始化  | ❌ 不可      | ✅ 必须      |

------

## 三、典型示例（直接对照）

### ✅ Event Topic（事件）

```
system/start
system/stop
user/login
user/logout
test/step_finished
sensor/sample_ready
```

特征：

- 只在某一刻有意义
- 不能用于 UI 初始化

------

### ✅ State Topic（状态）

```
system/state
user/logged_in
user/score
user/level
sensor/temperature
test/current_step
```

特征：

- 表示“当前事实”
- 新界面一打开就必须知道

------

## 四、最容易混的场景（必看）

### ❌ 错误 1：用事件表示状态

```
system/start = true   ❌
```

> 含义混乱：
>  是“点过启动”？还是“正在运行”？

✅ 正确拆分：

```
system/start        // Event
system/state        // State = Running
```

------

### ❌ 错误 2：用状态表示事件

```
test/finished = true   ❌
```

> 第二次 finished 会覆盖第一次，事件语义丢失。

✅ 正确拆分：

```
test/step_finished    // Event
test/result           // State
```

------

### ❌ 错误 3：一个 Topic 两种语义

```
system/run   ❌
```

> 有时表示“点击启动”
>  有时表示“正在运行”

🚨 **这是禁止级设计**

------

## 五、Model / ViewModel 使用速查

### 🧱 Model（业务层）

- **Publish：State Topic**
- **Subscribe：Event Topic**

```
Publish("system/state", Running);
Publish("user/level", level);
```

------

### 🧠 ViewModel（UI 层）

- **Subscribe：State Topic**
- **Command Publish：Event Topic**

```
Publish("system/start", {});
Publish("user/logout", {});
```

------

## 六、终极判断法（一句话）

> **能不能回答“现在是多少 / 是什么”？
>  能 → State
>  不能 → Event**

------

## 七、红线提醒（一定要记住）

```
❌ 一个 Topic 同时承担 Event + State
❌ 用 Event 初始化 UI
❌ 用 State 表示“发生过一次”
```

------

## 八、一句话口诀（贴墙版）

> **事件是“发生过”，
>  状态是“现在是”。**



