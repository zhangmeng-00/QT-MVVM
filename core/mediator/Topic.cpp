#include "Topic.h"
#include "Observe.h"
#include <QDebug>
#include <algorithm>

Topic::Topic(const QString& tag, QObject* parent)
    : QObject(parent)
    , m_tag(tag)
{
}

bool Topic::containsLocked(Observe* observer) const
{
    for (const auto& s : m_subscribers) {
        if (s.observer == observer) return true;
    }
    return false;
}

bool Topic::HasSubscriber() const
{
    QMutexLocker locker(&m_mutex);
    return !m_subscribers.empty();
}

void Topic::cleanupDeadSubscribersLocked()
{
    m_subscribers.erase(
        std::remove_if(m_subscribers.begin(), m_subscribers.end(),
                       [](const SubscriberItem& s) { return s.observer.isNull(); }),
        m_subscribers.end()
        );
}

void Topic::AddSubscriber(Observe* observer, PolicyPtr policy)
{
    if (!observer) return;

    {
        QMutexLocker lk(&m_mutex);
        cleanupDeadSubscribersLocked();

        // 去重：防止重复订阅导致count虚高
        if (!containsLocked(observer)) {
            SubscriberItem item;
            item.observer = observer;
            item.policy   = policy;
            m_subscribers.push_back(item);

            qDebug() << "[Topic] AddSubscriber tag =" << m_tag
                     << "observer =" << observer
                     << "count =" << (int)m_subscribers.size();
        } else {
            // 已订阅：更新policy（可选）
            for (auto& s : m_subscribers) {
                if (s.observer == observer) {
                    s.policy = policy;
                    break;
                }
            }
        }

        // observer 析构自动移除（避免野指针）
        // 注意：重复 connect 也没关系，但更严谨可以加 Qt::UniqueConnection（lambda不支持唯一）
        QObject::connect(observer, &QObject::destroyed, this, [this, observer] {
            this->RemoveSubscriber(observer);
        });

        // ✅ Replay lastValue：新订阅者默认应该拿到当前状态
        // // 重要：replay 不走 ValueChangedPolicy old==new 判断，否则新订阅会收不到
        // if (m_hasLastValue) {
        //     qDebug() << "[Topic] Replay last value to new subscriber, tag ="
        //              << m_tag << "value =" << m_lastValue;

        //     // 这里直接发：订阅即同步状态
        //     observer->OnDataReceived(m_tag, m_lastValue);
        // }
    }
}

void Topic::RemoveSubscriber(Observe* observer)
{
    if (!observer) return;

    QMutexLocker lk(&m_mutex);
    cleanupDeadSubscribersLocked();

    auto before = m_subscribers.size();
    m_subscribers.erase(
        std::remove_if(m_subscribers.begin(), m_subscribers.end(),
                       [&](const SubscriberItem& s) { return s.observer == observer; }),
        m_subscribers.end()
        );

    if (m_subscribers.size() != before) {
        qDebug() << "[Topic] RemoveSubscriber tag =" << m_tag
                 << "observer =" << observer
                 << "count =" << (int)m_subscribers.size();
    }
}

void Topic::Notify(const QVariant& value)
{
    // 快照：避免回调里再subscribe/unsubscribe导致迭代器问题
    std::vector<SubscriberItem> snapshot;
    QVariant oldValue;
    QVariant newValue = value;
    bool hadLast = false;

    {
        QMutexLocker lk(&m_mutex);
        cleanupDeadSubscribersLocked();

        qDebug() << "[Topic] Notify tag =" << m_tag
                 << "subscriber count =" << (int)m_subscribers.size();

        snapshot = m_subscribers;
        oldValue = m_lastValue;
        hadLast  = m_hasLastValue;

        // 更新状态
        m_lastValue = newValue;
        m_hasLastValue = true;
    }

    // 解锁后回调订阅者（避免死锁&并发修改）
    for (const auto& s : snapshot) {
        Observe* obs = s.observer.data();
        if (!obs) continue;

        bool should = true;

        // 你的 policy 语义是“是否执行”
        // 这里保留：首次发布时 hadLast=false，你可以选择让 policy 自己处理 invalid oldValue
        if (s.policy && hadLast) {
            should = s.policy->ShouldExecute(oldValue, newValue);
        }

        if (should) {
            obs->OnDataReceived(m_tag, newValue);
        }
    }
}
