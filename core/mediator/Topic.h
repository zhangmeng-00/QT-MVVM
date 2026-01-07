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
 * - 一个 tag 对应一个 Topic
 * - 保存最近一次发布的数据（State）
 * - 支持新订阅者加入时自动补发 lastValue
 */
class Topic : public QObject {
    Q_OBJECT
public:
    explicit Topic(const QString& tag, QObject* parent = nullptr);

    /*
     * AddSubscriber
     * --------------------------------------------------------
     * 添加订阅者
     * 如果 Topic 已有历史值：
     * - 立即按策略补发一次
     */
    void AddSubscriber(Observe* observer, PolicyPtr policy);

    /*
     * Notify
     * --------------------------------------------------------
     * 发布新数据
     */
    void Notify(const QString& tag, const QVariant& value);

private:
    struct SubscriberItem {
        Observe*   observer = nullptr;
        PolicyPtr policy;
    };

private:
    QString m_tag;

    QVariant m_lastValue;      // 最近一次值
    bool     m_hasLastValue{false};

    std::vector<SubscriberItem> m_subscribers;
};
