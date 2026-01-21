#include "Observe.h"
#include <QDebug>

Observe::Observe(QObject* parent)
    : QObject(parent)
{
}

Observe::~Observe() = default;

void Observe::Subscribe(const QString& tag, PolicyPtr policy)
{
    qDebug() << "[Observe] Subscribe request:" << this << tag;

    // ✅ policy 直接跟信号走，避免 pendingPolicy 被后一次 Subscribe 覆盖
    emit RequestSubscribe(this, tag, policy);
}

void Observe::Unsubscribe(const QString& tag)
{
    qDebug() << "[Observe] Unsubscribe request:" << this << tag;
    emit RequestUnsubscribe(this, tag);
}

void Observe::Publish(const QString& tag, const QVariant& value)
{
    emit RequestPublish(tag, value);
}

void Observe::OnDataReceived(const QString& tag,
                             const QVariant& value)
{
    // 默认实现：直接转给子类业务入口
    handleData(tag, value);
}
