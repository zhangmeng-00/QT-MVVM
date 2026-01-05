// Actor.h
#pragma once

#include <QObject>
#include <QMutex>
#include <QQueue>
#include <functional>

/*
 * Actor（Mailbox Helper）
 * ============================================================
 * 注意：Actor 本身不继承 QObject（避免 QObject 多继承）
 *
 * 使用方式：
 * - Actor 绑定一个 owner(QObject*)
 * - Post(fn) 会把任务放入线程安全队列
 * - 使用 invokeMethod 让 owner 在其线程中处理队列
 *
 * 保证：
 * - 任务串行执行（同一时刻只有一个处理循环）
 */
class Actor {
public:
    explicit Actor(QObject* owner);
    ~Actor();

    void Post(std::function<void()> fn);

private:
    void ProcessMailbox();

private:
    QObject* m_owner = nullptr;
    QMutex m_mutex;
    QQueue<std::function<void()>> m_queue;
    bool m_processing = false;
};
