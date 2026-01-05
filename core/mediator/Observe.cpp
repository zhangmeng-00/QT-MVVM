// Observe.cpp
#include "Observe.h"

Observe::Observe(QObject* parent)
    : QObject(parent)
{
}

Observe::~Observe() = default;

void Observe::SubscribeRequest(const QString& dataType,
                               const QString& tag,
                               std::shared_ptr<ISubscriptionPolicy> policy)
{
    m_pendingPolicy = std::move(policy);
    emit RequestSubscribe(this, this, dataType, tag);
}

void Observe::PublishRequest(const QString& dataType,
                             const QString& tag,
                             const QVariant& data)
{
    emit RequestPublish(dataType, tag, data);
}

std::shared_ptr<ISubscriptionPolicy> Observe::TakePendingPolicy()
{
    auto p = m_pendingPolicy;
    m_pendingPolicy.reset();
    return p;
}

void Observe::OnDataReceived(const QString& dataType,
                             const QString& tag,
                             const QVariant& data)
{
    ObserveData(dataType, tag, data);
}
