#pragma once

#include <QString>
#include <QVariant>
#include <QObject>

// 前向声明
class Policy;
using PolicyPtr = std::shared_ptr<Policy>;

/*
 * IModel - 通用模型接口
 * ============================================================
 * 所有业务 Model 实现此接口
 * 核心能力：订阅、发布、生命周期管理
 *
 * 使用方式：
 *   class UserModel : public ActorObserve, public IModel { ... };
 */
class IModel {
public:
    virtual ~IModel() = default;

    // ===== 模型标识 =====
    virtual QString modelName() const = 0;

    // ===== 生命周期 =====
    virtual void initialize() = 0;
    virtual void shutdown() = 0;

    // ===== 订阅/发布（核心通信能力）=====
    // 订阅主题
    virtual void Subscribe(const QString& tag, const QVariant& value, PolicyPtr policy) = 0;
    // 取消订阅
    virtual void Unsubscribe(const QString& tag) = 0;
    // 发布主题
    virtual void Publish(const QString& tag, const QVariant& value) = 0;

    // ===== 调试 =====
    virtual QString debugInfo() const = 0;
};

// 智能指针类型别名
using IModelPtr = std::shared_ptr<IModel>;
