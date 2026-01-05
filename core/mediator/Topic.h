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
 * 管理某一个 typeKey:tag 下的所有订阅者
 */
class Topic {
public:
    Topic(const QString& typeKey,
          const QString& tag);

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

    QString m_typeKey;
    QString m_tag;
    QVariant m_lastValue;
    std::vector<Item> m_items;
};
