#include "Mediator.h"

Mediator::Mediator(QObject* parent)
    : QObject(parent)
{
}

void Mediator::ConnectObserve(Observe* obs)
{
    connect(obs, &Observe::RequestSubscribe,
            this, &Mediator::OnSubscribeRequest,
            Qt::QueuedConnection);

    connect(obs, &Observe::RequestPublish,
            this, &Mediator::OnPublishRequest,
            Qt::QueuedConnection);

    connect(obs, &QObject::destroyed,
            this, &Mediator::OnObserveDestroyed,
            Qt::QueuedConnection);
}

void Mediator::OnSubscribeRequest(QObject* owner,
                                  ITransport* transport,
                                  const QString& tag)
{
    auto* obs = qobject_cast<Observe*>(owner);
    if (!obs) return;

    auto policy = obs->TakePendingPolicy();
    if (!policy) return;

    GetOrCreateTopic("any", tag)
        ->AddSubscription(owner, transport, policy);
}

void Mediator::OnPublishRequest(const QString& tag,
                                const QVariant& value)
{
    QString typeKey = TypeKey::FromVariant(value);

    GetOrCreateTopic(typeKey, tag)->Notify(value);
    GetOrCreateTopic("any", tag)->Notify(value);
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
Mediator::GetOrCreateTopic(const QString& typeKey,
                           const QString& tag)
{
    QString key = typeKey + ":" + tag;
    auto it = m_topics.find(key);
    if (it == m_topics.end()) {
        it = m_topics.emplace(
                         key,
                         std::make_shared<Topic>(typeKey, tag)
                         ).first;
    }
    return it->second;
}
