#include "Mediator.h"
#include <QDebug>
#include <QMetaType>
#include "TopicRegistry.h"

/*
 * 构造函数
 */
Mediator::Mediator(QObject* parent)
    : QObject(parent)
{
    /*
     * Qt5 queued connection 传递 PolicyPtr（std::shared_ptr）必须注册元类型
     * 放在这里注册，确保 Mediator 初始化后一定可用
     */
    qRegisterMetaType<PolicyPtr>("PolicyPtr");
}

/*
 * ConnectObserve
 */
void Mediator::ConnectObserve(Observe* obs)
{
    if (!obs) return;

    qDebug() << "[Mediator] ConnectObserve:" << obs;

    /*
     * ✅ 统一用 QueuedConnection：
     * - Observe 可能在 UI 线程
     * - Mediator 可能在独立线程
     */
    connect(obs, &Observe::RequestSubscribe,
            this, &Mediator::OnSubscribe,
            Qt::QueuedConnection);

    connect(obs, &Observe::RequestPublish,
            this, &Mediator::OnPublish,
            Qt::QueuedConnection);
    connect(obs, &Observe::RequestUnsubscribe,
            this, &Mediator::OnUnsubscribe,
            Qt::QueuedConnection);

}
/*
 * OnSubscribe
 */
void Mediator::OnSubscribe(Observe* observer,
                           const QString& tag,
                           PolicyPtr policy)
{
    auto topic = getOrCreateTopic(tag);
    topic->AddSubscriber(observer, policy);

    // ⭐ Sticky Topic：新订阅者立刻收到当前状态
    if (TopicRegistry::instance().IsSticky(tag)) {
        QMutexLocker locker(&m_mutex);
        if (m_stateCache.contains(tag)) {
            observer->OnDataReceived(tag, m_stateCache[tag]);
        }
    }
}

/*
 * OnUnsubscribe
 */
void Mediator::OnUnsubscribe(Observe* obs, const QString& tag)
{
    QMutexLocker locker(&m_mutex);

    auto it = m_topics.find(tag);
    if (it == m_topics.end())
        return;

    it.value()->RemoveSubscriber(obs);   // ✅ QMap 正确访问

    if (!it.value()->HasSubscriber()) {
        qDebug() << "[Mediator] Remove empty topic:" << tag;
        m_topics.erase(it);
    }
}


/*
 * OnPublish
 */
void Mediator::OnPublish(const QString& tag,
                         const QVariant& value)
{
    // ⭐ 如果是 Sticky Topic，则缓存最后一次状态
    if (TopicRegistry::instance().IsSticky(tag)) {
        QMutexLocker locker(&m_mutex);
        m_stateCache[tag] = value;
    }

    // 正常事件分发
    auto topic = getOrCreateTopic(tag);
    topic->Notify(value);
}


/*
 * getOrCreateTopic
 */
std::shared_ptr<Topic> Mediator::getOrCreateTopic(const QString& tag)
{
    QMutexLocker locker(&m_mutex);

    auto it = m_topics.find(tag);
    if (it != m_topics.end()) {
        return it.value();   // ✅
    }

    auto topic = std::make_shared<Topic>(tag);
    m_topics.insert(tag, topic);
    return topic;
}
