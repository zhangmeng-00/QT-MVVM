// Mediator.cpp
#include "Mediator.h"

Mediator::Mediator(QObject* parent)
    : QObject(parent)
{
}

void Mediator::ConnectObserve(Observe* obs)
{
    if (!obs) return;

    connect(obs, &Observe::RequestSubscribe,
            this, &Mediator::OnSubscribeRequest,
            Qt::QueuedConnection);

    connect(obs, &Observe::RequestPublish,
            this, &Mediator::OnPublishRequest,
            Qt::QueuedConnection);

    // Observe 析构时，自动清理订阅（避免析构 emit queued）
    connect(obs, &QObject::destroyed,
            this, &Mediator::OnObserveDestroyed,
            Qt::QueuedConnection);
}

void Mediator::OnSubscribeRequest(QObject* owner,
                                  ITransport* transport,
                                  const QString& dataType,
                                  const QString& tag)
{
    auto* obs = qobject_cast<Observe*>(owner);
    if (!obs) return;

    auto policy = obs->TakePendingPolicy();
    if (!policy) return;

    auto topic = GetOrCreateTopic(dataType, tag);
    topic->AddSubscription(owner, transport, policy);
}

void Mediator::OnPublishRequest(const QString& dataType,
                                const QString& tag,
                                const QVariant& data)
{
    auto topic = GetOrCreateTopic(dataType, tag);
    topic->Notify(data);
}

void Mediator::OnObserveDestroyed(QObject* obj)
{
    for (auto it = m_topics.begin(); it != m_topics.end(); ) {
        it->second->RemoveAllByOwner(obj);

        if (it->second->IsEmpty())
            it = m_topics.erase(it);
        else
            ++it;
    }
}

std::shared_ptr<Topic>
Mediator::GetOrCreateTopic(const QString& dataType, const QString& tag)
{
    const QString key = dataType + ":" + tag;

    auto it = m_topics.find(key);
    if (it == m_topics.end()) {
        auto topic = std::make_shared<Topic>(dataType, tag);
        m_topics[key] = topic;
        return topic;
    }
    return it->second;
}
