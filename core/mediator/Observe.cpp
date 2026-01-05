#include "Observe.h"

Observe::Observe(QObject* parent)
    : QObject(parent)
{
}

Observe::~Observe() = default;

void Observe::Subscribe(const QString& tag,
                        std::shared_ptr<ISubscriptionPolicy> policy)
{
    m_pendingPolicy = std::move(policy);
    emit RequestSubscribe(this, this, tag);
}

void Observe::Publish(const QString& tag,
                      const QVariant& value)
{
    emit RequestPublish(tag, value);
}

std::shared_ptr<ISubscriptionPolicy> Observe::TakePendingPolicy()
{
    auto p = m_pendingPolicy;
    m_pendingPolicy.reset();
    return p;
}

void Observe::OnDataReceived(const QString&,
                             const QString& tag,
                             const QVariant& value)
{
    ObserveData(tag, value);
}
