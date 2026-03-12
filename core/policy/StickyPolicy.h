#pragma once
#include "ISubscriptionPolicy.h"

/*
 * StickyPolicy
 * ============================================================
 * Sticky 策略：新订阅者立刻收到当前缓存的状态值
 */
class StickyPolicy : public ISubscriptionPolicy {
public:
    bool ShouldExecute(const QVariant&, const QVariant&) override {
        return true;
    }

    // 重发上一次值给新订阅者
    bool ShouldReplayLastValue() const override {
        return true;
    }
};
