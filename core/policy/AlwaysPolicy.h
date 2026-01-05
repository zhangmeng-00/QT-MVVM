// AlwaysPolicy.h
#pragma once
#include "ISubscriptionPolicy.h"

/*
 * AlwaysPolicy：每次发布都通知
 * 注意：类内定义函数天然 inline，不会产生 LNK2005
 */
class AlwaysPolicy : public ISubscriptionPolicy {
public:
    bool ShouldExecute(const QVariant&, const QVariant&) override { return true; }
};
