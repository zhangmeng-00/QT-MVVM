#pragma once
#include "ISubscriptionPolicy.h"

/*
 * AlwaysPolicy
 * ============================================================
 * 每次发布都通知
 */
class AlwaysPolicy : public ISubscriptionPolicy {
public:
    bool ShouldExecute(const QVariant&, const QVariant&) override {
        return true;
    }
};
