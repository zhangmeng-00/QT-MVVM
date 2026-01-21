#pragma once

#include "actor/ActorObserve.h"

/*
 * BaseModel
 * ============================================================
 * 所有业务 Model 的基类
 *
 * 特点：
 * ------------------------------------------------------------
 * - 已经具备 Actor（串行、线程安全）
 * - 只需要实现 ObserveData
 */
class BaseModel : public ActorObserve {
    Q_OBJECT
public:
    explicit BaseModel(QObject* parent = nullptr)
        : ActorObserve(parent)
    {}

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
