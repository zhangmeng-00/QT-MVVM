#pragma once
#include <QObject>
#include <QMutex>
#include <QQueue>
#include <functional>

/*
 * Actor
 * ============================================================
 * Mailbox 工具类（不继承 QObject）
 *
 * 职责：
 * ------------------------------------------------------------
 * - 提供线程安全的任务队列
 * - 确保任务在 owner(QObject) 所在线程串行执行
 */
class Actor {
public:
    explicit Actor(QObject* owner);
    ~Actor();

    // 投递任务到 Mailbox
    void Post(std::function<void()> fn);

private:
    void ProcessMailbox();

private:
    QObject* m_owner = nullptr;
    QMutex m_mutex;
    QQueue<std::function<void()>> m_queue;
    bool m_processing = false;
};
