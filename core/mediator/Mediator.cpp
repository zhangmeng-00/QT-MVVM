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

    // 连接带value的订阅信号
    connect(obs, SIGNAL(RequestSubscribe(Observe*,QString,QVariant,PolicyPtr)),
            this, SLOT(OnSubscribe(Observe*,QString,QVariant,PolicyPtr)),
            Qt::QueuedConnection);

    connect(obs, &Observe::RequestPublish,
            this, &Mediator::OnPublish,
            Qt::QueuedConnection);
    connect(obs, &Observe::RequestUnsubscribe,
            this, &Mediator::OnUnsubscribe,
            Qt::QueuedConnection);

}

/*
 * OnSubscribe - 统一处理函数（带value）
 */
void Mediator::OnSubscribe(Observe* observer,
                           const QString& tag,
                           const QVariant& value,
                           PolicyPtr policy)
{
    // 获取类型名用于索引
    QString typeName = value.typeName();
    if (typeName.isEmpty()) {
        typeName = "__empty__"; // 空类型使用默认键
    }

    qDebug() << "[Mediator] OnSubscribe:" << tag << "type:" << typeName;

    auto topic = getOrCreateTopic(typeName, tag);
    topic->AddSubscriber(observer, policy);

    // ⭐ Sticky Policy：新订阅者立刻收到当前状态
    if (policy && policy->ShouldReplayLastValue()) {
        QVariant cached;
        bool has = false;

        {
            QMutexLocker locker(&m_mutex);
            auto typeIt = m_stateCache.find(typeName);
            if (typeIt != m_stateCache.end()) {
                auto it = typeIt.value().find(tag);
                if (it != typeIt.value().end()) {
                    cached = it.value();
                    has = true;
                }
            }
        }

        if (has && observer) {
            const QString t = tag;
            const QVariant v = cached;

            // ✅ 回到 observer 所在线程
            QMetaObject::invokeMethod(
                observer,
                [observer, t, v](){ observer->OnDataReceived(t, v); },
                Qt::QueuedConnection
                );
        }
    }

}

/*
 * OnUnsubscribe - 需要获取typeName才能正确退订
 * 注意：由于信号不携带value，这里暂时无法确定typeName
 * 解决方案：遍历所有typeName查找匹配的tag
 */
void Mediator::OnUnsubscribe(Observe* obs, const QString& tag)
{
    QMutexLocker locker(&m_mutex);

    // 遍历所有typeName查找匹配的topic
    for (auto typeIt = m_topics.begin(); typeIt != m_topics.end(); ++typeIt) {
        auto it = typeIt.value().find(tag);
        if (it != typeIt.value().end()) {
            it.value()->RemoveSubscriber(obs);

            if (!it.value()->HasSubscriber()) {
                qDebug() << "[Mediator] Remove empty topic:" << tag << "type:" << typeIt.key();
                typeIt.value().erase(it);
            }

            // 如果该typeName下没有更多topic，清理外层Map
            if (typeIt.value().isEmpty()) {
                m_topics.erase(typeIt);
            }
            return;
        }
    }
}


/*
 * OnPublish
 */
void Mediator::OnPublish(const QString& tag,
                         const QVariant& value)
{
    // 获取类型名用于索引
    QString typeName = value.typeName();
    if (typeName.isEmpty()) {
        typeName = "__empty__";
    }

    // ⭐ 修复：先尝试精确匹配，如果没找到则遍历所有typeName查找匹配的tag
    // 这样可以兼容旧版订阅（QVariant为空）和新版订阅
    auto topic = findTopic(typeName, tag);
    if (!topic) {
        // 精确匹配没找到，遍历所有typeName查找匹配的tag
        QMutexLocker locker(&m_mutex);
        for (auto typeIt = m_topics.begin(); typeIt != m_topics.end(); ++typeIt) {
            auto it = typeIt.value().find(tag);
            if (it != typeIt.value().end()) {
                topic = it.value();
                qDebug() << "[Mediator] Publish fallback: found topic in type:" << typeIt.key()
                         << "for tag:" << tag;
                break;
            }
        }
    }

    // 如果仍然没有topic，创建一个新的
    if (!topic) {
        topic = getOrCreateTopic(typeName, tag);
    }

    // ⭐ 如果有 Sticky 订阅者，则缓存最后一次状态
    if (topic->HasStickySubscriber()) {
        QMutexLocker locker(&m_mutex);
        m_stateCache[typeName][tag] = value;
    }

    // 正常事件分发
    topic->Notify(value);
}


/*
 * findTopic - 查找Topic（不创建）
 */
std::shared_ptr<Topic> Mediator::findTopic(const QString& typeName,
                                            const QString& tag)
{
    QMutexLocker locker(&m_mutex);

    auto typeIt = m_topics.find(typeName);
    if (typeIt != m_topics.end()) {
        auto it = typeIt.value().find(tag);
        if (it != typeIt.value().end()) {
            return it.value();
        }
    }
    return nullptr;
}


/*
 * getOrCreateTopic - 按typeName + tag索引
 */
std::shared_ptr<Topic> Mediator::getOrCreateTopic(const QString& typeName,
                                                   const QString& tag)
{
    QMutexLocker locker(&m_mutex);

    // 第一层：查找typeName
    auto typeIt = m_topics.find(typeName);
    if (typeIt != m_topics.end()) {
        // 第二层：查找tag
        auto it = typeIt.value().find(tag);
        if (it != typeIt.value().end()) {
            return it.value();
        }
    }

    // 创建新的Topic
    auto topic = std::make_shared<Topic>(tag);

    // 插入两层Map
    m_topics[typeName][tag] = topic;

    return topic;
}
