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
 * ------------------------------------------------------------
 * 新订阅者加入
 * 如果 Topic 已有历史值 → 立即补发
 */
void Topic::AddSubscriber(Observe* observer, PolicyPtr policy)
{
    if (!observer) return;

    SubscriberItem item;
    item.observer = observer;
    item.policy   = policy;

    m_subscribers.push_back(item);

    qDebug() << "[Topic] AddSubscriber tag =" << m_tag
             << "observer =" << observer
             << "count =" << m_subscribers.size();

    /*
     * ✅ 状态补发（关键）
     * --------------------------------------------------------
     * 如果 Topic 之前已经发布过数据
     * 新订阅者立即收到一次 lastValue
     */
    if (m_hasLastValue) {
        bool should = true;

        if (policy) {
            // old == new，用于决定是否补发
            should = policy->ShouldExecute(m_lastValue, m_lastValue);
        }

        if (should) {
            qDebug() << "[Topic] Replay last value to new subscriber, tag ="
                     << m_tag << "value =" << m_lastValue;

            observer->OnDataReceived(m_tag, m_lastValue);
        }
    }
}

/*
 * Notify
 * ------------------------------------------------------------
 * 正常发布流程
 */
void Topic::Notify(const QString& tag, const QVariant& value)
{
    Q_UNUSED(tag);

    qDebug() << "[Topic] Notify tag =" << m_tag
             << "subscriber count =" << m_subscribers.size();

    QVariant oldValue = m_lastValue;
    QVariant newValue = value;

    for (const auto& s : m_subscribers) {
        if (!s.observer) continue;

        bool should = true;
        if (s.policy && m_hasLastValue) {
            should = s.policy->ShouldExecute(oldValue, newValue);
        }

        if (should) {
            s.observer->OnDataReceived(m_tag, newValue);
        }
    }

    // 更新状态
    m_lastValue   = newValue;
    m_hasLastValue = true;
}
