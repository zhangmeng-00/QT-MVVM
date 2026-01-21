#pragma once

#include "actor/ActorObserve.h"

/*
 * BaseView
 * ============================================================
 * 所有 View 的基类
 *
 * 特点：
 * ------------------------------------------------------------
 * - UI 操作一定在 UI 线程
 * - 串行执行
 */
class BaseView : public ActorObserve {
    Q_OBJECT
public:
    explicit BaseView(QObject* parent = nullptr)
        : ActorObserve(parent)
    {}

protected:
    void ObserveData(const QString& tag,
                     const QVariant& value) override
    {
        Q_UNUSED(tag)
        Q_UNUSED(value)
        // 子类实现
    }
};
