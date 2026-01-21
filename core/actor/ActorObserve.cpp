#include "ActorObserve.h"
#include <QMetaObject>
#include <QThread>

/*
 * 构造函数
 */
ActorObserve::ActorObserve(QObject* parent)
    : Observe(parent)
{
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
