#pragma once
#include "ISubscriptionPolicy.h"

/*
 * StickyPolicy
 * ============================================================
 * Sticky 策略：新订阅者立刻收到当前缓存的状态值
 */
class StickyPolicy : public ISubscriptionPolicy {
public:
    explicit StickyPolicy(bool replayLastValue = true)
        : ISubscriptionPolicy(replayLastValue) {}

    bool ShouldExecute(const QVariant&, const QVariant&) override {
        return true;
    }
};
