#include "ActorObserve.h"
#include <QMetaObject>

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
void ActorObserve::OnDataReceived(const QString& tag,
                                  const QVariant& value)
{
    QMetaObject::invokeMethod(
        this,
        "onActorInvoke",
        Qt::QueuedConnection,
        Q_ARG(QString, tag),
        Q_ARG(QVariant, value)
        );
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
