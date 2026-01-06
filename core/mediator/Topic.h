#pragma once

#include <QObject>
#include <QVariant>
#include <QString>
#include <vector>
#include <memory>

#include "../policy/ISubscriptionPolicy.h"

class Observe;

/*
 * Topic
 * ============================================================
 * 每个 tag 一个 Topic
 *
 * 管理：
 * - 订阅者列表（observer + policy）
 * - 最近一次值（用于策略判断 old/new）
 */
class Topic : public QObject {
    Q_OBJECT
public:
    explicit Topic(const QString& tag, QObject* parent = nullptr);

    /*
     * AddSubscriber
     * --------------------------------------------------------
     * 添加订阅者
     */
    void AddSubscriber(Observe* observer, PolicyPtr policy);

    /*
     * Notify
     * --------------------------------------------------------
     * 发布数据，按策略通知所有订阅者
     */
    void Notify(const QString& tag, const QVariant& value);

private:
    struct SubscriberItem {
        Observe*   observer = nullptr; // 不拥有对象生命周期
        PolicyPtr policy;              // 策略
    };

private:
    QString m_tag;
    QVariant m_lastValue;
    bool m_hasLastValue = false;

    std::vector<SubscriberItem> m_subscribers;
};
