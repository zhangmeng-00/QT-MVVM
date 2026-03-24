#pragma once

#include "ActorObserve.h"
#include "IModel.h"

/*
 * BaseModel
 * ============================================================
 * 所有业务 Model 的基类
 *
 * 特点：
 * ------------------------------------------------------------
 * - 已经具备 Actor（串行、线程安全）
 * - 只需要实现 ObserveData
 * - 实现 IModel 接口，支持依赖注入
 */
class BaseModel : public ActorObserve, public IModel {
    Q_OBJECT
public:
    /*
     * BaseModel 构造函数
     * --------------------------------------------------------
     * @param parent: 父对象
     * @param useSeparateThread: 是否使用单独的线程运行
     */
    explicit BaseModel(QObject* parent = nullptr,
                      bool useSeparateThread = false);

    // ===== IModel 接口实现 =====

    // 模型标识（默认使用类名，子类可重写）
    QString modelName() const override {
        return metaObject()->className();
    }

    // 初始化（子类可重写）
    void initialize() override {
        SetupSubscriptions();
    }

    // 关闭（子类可重写）
    void shutdown() override {
        // 默认实现：取消所有订阅
    }

    // 调试信息（子类可重写）
    QString debugInfo() const override {
        return QString("BaseModel: %1").arg(modelName());
    }

protected:
    /*
     * ObserveData
     * --------------------------------------------------------
     * 业务只处理 tag + value
     */
    void ObserveData(const QString& tag,
                     const QVariant& value) override
    {
        Q_UNUSED(tag)
        Q_UNUSED(value)
        // 子类实现
    }
};
