#include "Topic.h"
#include "Observe.h"
#include <QDebug>

Topic::Topic(const QString& tag, QObject* parent)
    : QObject(parent)
    , m_tag(tag)
{
}

/*
 * AddSubscriber
 */
void Topic::AddSubscriber(Observe* observer, PolicyPtr policy)
{
    if (!observer) return;

    // policy 允许为空：为空则默认 Always（可按你需要改成拒绝）
    if (!policy) {
        qDebug() << "[Topic] AddSubscriber policy is null, tag =" << m_tag
                 << "observer =" << observer;
    }

    SubscriberItem item;
    item.observer = observer;
    item.policy = std::move(policy);

    m_subscribers.push_back(item);

    qDebug() << "[Topic] AddSubscriber tag =" << m_tag
             << "observer =" << observer
             << "count =" << (int)m_subscribers.size();
}

/*
 * Notify
 */
void Topic::Notify(const QString& tag, const QVariant& value)
{
    Q_UNUSED(tag);

    qDebug() << "[Topic] Notify tag =" << m_tag
             << "subscriber count =" << (int)m_subscribers.size();

    // old/new 用于策略判断
    QVariant oldValue = m_lastValue;
    QVariant newValue = value;

    for (const auto& s : m_subscribers) {
        if (!s.observer) continue;

        // 如果没提供策略，默认执行（相当于 AlwaysPolicy）
        bool should = true;
        if (s.policy) {
            if (m_hasLastValue) {
                should = s.policy->ShouldExecute(oldValue, newValue);
            } else {
                // 第一次发布：通常选择通知（你也可以改成 false）
                should = true;
            }
        }

        if (should) {
            // 直接调用回调入口（如果 observer 是 ActorObserve，会转发到 Mailbox）
            s.observer->OnDataReceived(m_tag, newValue);
        }
    }

    // 更新 lastValue
    m_lastValue = newValue;
    m_hasLastValue = true;
}
