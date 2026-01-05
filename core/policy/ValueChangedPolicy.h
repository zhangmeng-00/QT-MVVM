// ValueChangedPolicy.h
#pragma once
#include "ISubscriptionPolicy.h"

/*
 * ValueChangedPolicy：仅当值变化时通知
 */
class ValueChangedPolicy : public ISubscriptionPolicy {
public:
    bool ShouldExecute(const QVariant& oldValue, const QVariant& newValue) override {
        return oldValue != newValue;
    }
};
