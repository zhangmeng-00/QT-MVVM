#pragma once
#include <QObject>
#include "actor/ActorObserve.h"

/*
 * BaseViewModel
 * ============================================================
 * - ViewModel 也是 Observe（订阅总线）
 * - 但它对 View 的输出是：Qt Property + signals
 */
class BaseViewModel : public ActorObserve {
    Q_OBJECT
public:
    explicit BaseViewModel(QObject* parent = nullptr)
        : ActorObserve(parent)
    {}

protected:
           // 子类实现 ObserveData(tag, value)
};
