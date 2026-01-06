#include "Mediator.h"
#include <QDebug>
#include <QMetaType>

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
}

/*
 * OnSubscribe
 */
void Mediator::OnSubscribe(Observe* observer,
                           const QString& tag,
                           PolicyPtr policy)
{
    qDebug() << "[Mediator] OnSubscribe:" << observer << tag;

    auto topic = getOrCreateTopic(tag);
    topic->AddSubscriber(observer, policy);
}

/*
 * OnPublish
 */
void Mediator::OnPublish(const QString& tag,
                         const QVariant& value)
{
    auto topic = getOrCreateTopic(tag);
    topic->Notify(tag, value);
}

/*
 * getOrCreateTopic
 */
std::shared_ptr<Topic> Mediator::getOrCreateTopic(const QString& tag)
{
    QMutexLocker locker(&m_mutex);

    if (!m_topics.contains(tag)) {
        m_topics[tag] = std::make_shared<Topic>(tag);
    }
    return m_topics[tag];
}
