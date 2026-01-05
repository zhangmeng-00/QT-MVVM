// Topic.cpp
#include "Topic.h"
#include <algorithm>

Topic::Topic(const QString& dataType, const QString& tag)
    : m_dataType(dataType), m_tag(tag)
{
}

void Topic::AddSubscription(QObject* owner,
                            ITransport* transport,
                            std::shared_ptr<ISubscriptionPolicy> policy)
{
    Item it;
    it.owner = owner;
    it.transport = transport;
    it.policy = std::move(policy);
    m_items.push_back(std::move(it));
}

void Topic::RemoveAllByOwner(QObject* owner)
{
    m_items.erase(std::remove_if(m_items.begin(), m_items.end(),
                                 [&](const Item& i){ return i.owner == owner; }),
                  m_items.end());
}

void Topic::Notify(const QVariant& value)
{
    for (int i = 0; i < static_cast<int>(m_items.size()); ) {

        // owner 已析构，清理
        if (m_items[i].owner.isNull()) {
            m_items.erase(m_items.begin() + i);
            continue;
        }

        if (m_items[i].policy && m_items[i].policy->ShouldExecute(m_lastValue, value)) {
            if (m_items[i].transport) {
                m_items[i].transport->OnDataReceived(m_dataType, m_tag, value);
            }
        }

        ++i;
    }

    m_lastValue = value;
}

bool Topic::IsEmpty() const
{
    return m_items.empty();
}
