#pragma once
#include "ISubscriptionPolicy.h"

/*
 * ValueChangedPolicy
 * ============================================================
 * 只有值变化时才通知
 */
class ValueChangedPolicy : public ISubscriptionPolicy {
public:
    bool ShouldExecute(const QVariant& oldValue,
                       const QVariant& newValue) override {
        return oldValue != newValue;
    }
};
