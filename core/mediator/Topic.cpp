#include "Topic.h"
#include <algorithm>

Topic::Topic(const QString& typeKey,
             const QString& tag)
    : m_typeKey(typeKey)
    , m_tag(tag)
{
}

void Topic::AddSubscription(QObject* owner,
                            ITransport* transport,
                            std::shared_ptr<ISubscriptionPolicy> policy)
{
    m_items.push_back({ owner, transport, std::move(policy) });
}

void Topic::RemoveAllByOwner(QObject* owner)
{
    m_items.erase(
        std::remove_if(
            m_items.begin(), m_items.end(),
            [&](const Item& i){ return i.owner == owner; }
            ),
        m_items.end()
        );
}

void Topic::Notify(const QVariant& value)
{
    for (size_t i = 0; i < m_items.size(); ) {
        if (m_items[i].owner.isNull()) {
            m_items.erase(m_items.begin() + i);
            continue;
        }

        if (!m_items[i].policy ||
            m_items[i].policy->ShouldExecute(m_lastValue, value)) {
            m_items[i].transport->OnDataReceived(m_typeKey, m_tag, value);
        }
        ++i;
    }
    m_lastValue = value;
}

bool Topic::IsEmpty() const
{
    return m_items.empty();
}
