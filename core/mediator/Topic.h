// Topic.h
#pragma once

#include <QString>
#include <QVariant>
#include <QPointer>
#include <vector>
#include <memory>

#include "../transport/ITransport.h"
#include "../policy/ISubscriptionPolicy.h"

/*
 * Topic
 * ============================================================
 * 管理 (dataType, tag) 下的订阅集合
 *
 * 设计点：
 * - owner 用 QPointer 跟踪 Observe 生命周期
 * - Notify 用 index 遍历，允许订阅者在回调中销毁自己
 */
class Topic {
public:
    Topic(const QString& dataType, const QString& tag);

    void AddSubscription(QObject* owner,
                         ITransport* transport,
                         std::shared_ptr<ISubscriptionPolicy> policy);

    void RemoveAllByOwner(QObject* owner);

    void Notify(const QVariant& value);

    bool IsEmpty() const;

private:
    struct Item {
        QPointer<QObject> owner;
        ITransport* transport = nullptr;
        std::shared_ptr<ISubscriptionPolicy> policy;
    };

private:
    QString m_dataType;
    QString m_tag;
    QVariant m_lastValue;
    std::vector<Item> m_items;
};
