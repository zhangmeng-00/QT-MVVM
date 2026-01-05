// Actor.cpp
#include "Actor.h"
#include <QMetaObject>

Actor::Actor(QObject* owner)
    : m_owner(owner)
{
}

Actor::~Actor()
{
    // 析构不做跨线程动作
}

void Actor::Post(std::function<void()> fn)
{
    if (!m_owner) return;

    {
        QMutexLocker locker(&m_mutex);
        m_queue.enqueue(std::move(fn));
    }

    // 让 owner 在自己的线程执行 ProcessMailbox（queued，跨线程安全）
    QMetaObject::invokeMethod(m_owner, [this]() {
        this->ProcessMailbox();
    }, Qt::QueuedConnection);
}

void Actor::ProcessMailbox()
{
    {
        QMutexLocker locker(&m_mutex);
        if (m_processing) return;   // 防止重复进入
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

        if (job) job(); // 串行执行
    }
}
