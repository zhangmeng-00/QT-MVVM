// ISubscriptionPolicy.h
#pragma once
#include <QVariant>

/*
 * ISubscriptionPolicy
 * ============================================================
 * 订阅策略接口：决定是否通知订阅者
 */
class ISubscriptionPolicy {
public:
    virtual ~ISubscriptionPolicy() = default;
    virtual bool ShouldExecute(const QVariant& oldValue,
                               const QVariant& newValue) = 0;
};
