#include "Actor.h"
#include <QMetaObject>

Actor::Actor(QObject* owner)
    : m_owner(owner)
{
}

Actor::~Actor() = default;

void Actor::Post(std::function<void()> fn)
{
    if (!m_owner) return;

    {
        QMutexLocker locker(&m_mutex);
        m_queue.enqueue(std::move(fn));
    }

    // 确保在 owner 所在线程执行
    QMetaObject::invokeMethod(
        m_owner,
        [this]() { ProcessMailbox(); },
        Qt::QueuedConnection
        );
}

void Actor::ProcessMailbox()
{
    {
        QMutexLocker locker(&m_mutex);
        if (m_processing) return;
        m_processing = true;
    }

    while (true) {
        std::function<void()> job;
        {
            QMutexLocker locker(&m_mutex);
            if (m_queue.isEmpty()) {
                m_processing = false;
                break;
            }
            job = m_queue.dequeue();
        }
        if (job) job();
    }
}
