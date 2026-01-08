#pragma once

#include <QObject>
#include <QVariant>
#include <QString>
#include <QPointer>
#include <QMutex>
#include <vector>
#include <memory>

#include "../policy/ISubscriptionPolicy.h"

class Observe;

/*
 * Replay rule:
 * ------------------------------------------------------------
 * - New subscriber will ALWAYS receive lastValue once
 * - Replay is NOT filtered by ValueChangedPolicy
 * - Policy only affects future Notify(old,new)
 */

/*
 * Topic
 * ============================================================
 * - 一个 tag 对应一个 Topic
 * - 保存最近一次发布的数据（State）
 * - 支持新订阅者加入时自动补发 lastValue（Replay）
 * - 支持退订 + observer 析构自动清理（避免悬空指针）
 */
class Topic : public QObject {
    Q_OBJECT
public:
    explicit Topic(const QString& tag, QObject* parent = nullptr);

    void AddSubscriber(Observe* observer, PolicyPtr policy);
    void RemoveSubscriber(Observe* observer);

    // 你原先是 Notify(tag,value)，tag 实际没用到，这里建议简化
    void Notify(const QVariant& value);
    bool HasSubscriber() const;
private:
    struct SubscriberItem {
        QPointer<Observe> observer;   // QObject销毁自动变nullptr
        PolicyPtr policy;
    };

    void cleanupDeadSubscribersLocked();
    bool containsLocked(Observe* observer) const;

private:
    QString m_tag;

    // 如果你保证 Topic 永远只在 Mediator 线程访问，这把锁也可以去掉；
    // 但为了“动态VM/复杂场景”稳定，建议保留
    mutable QMutex m_mutex;

    QVariant m_lastValue;
    bool     m_hasLastValue{false};

    std::vector<SubscriberItem> m_subscribers;
};
