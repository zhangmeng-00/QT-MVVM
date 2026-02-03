#include "ActorObserve.h"
#include <QMetaObject>
#include <QThread>

/*
 * 构造函数
 */
ActorObserve::ActorObserve(QObject* parent, bool useSeparateThread)
    : Observe(parent)
{
    if (useSeparateThread) {
        // 创建并启动单独的线程
        m_thread = new QThread(this);
        // 将当前对象移到新线程
        moveToThread(m_thread);
        // 启动线程
        m_thread->start();
    }
}

ActorObserve::~ActorObserve()
{
    if (m_thread && m_thread->isRunning()) {
        // 请求线程退出
        m_thread->quit();
        // 等待线程结束
        m_thread->wait();
        // 线程会被父对象（this）销毁，所以不需要手动delete
    }
}

/*
 * OnDataReceived
 * ------------------------------------------------------------
 * 这里可能在 Mediator / 发布线程
 * 只负责投递，不做业务
 */
void ActorObserve::handleData(const QString& tag,
                                  const QVariant& value)
{
    // 如果已经在对象线程，直接调用
    if (QThread::currentThread() == thread()) {
        ObserveData(tag, value);
    } else {
        // 否则，强制投递回对象线程
        QMetaObject::invokeMethod(
            this,
            [this, tag, value]() {
                ObserveData(tag, value);
            },
            Qt::QueuedConnection
            );
    }
}

/*
 * onActorInvoke
 * ------------------------------------------------------------
 * 一定在对象所属线程执行
 * 这里才调用真正的业务逻辑
 */
void ActorObserve::onActorInvoke(QString tag,
                                 QVariant value)
{
    ObserveData(tag, value);
}
