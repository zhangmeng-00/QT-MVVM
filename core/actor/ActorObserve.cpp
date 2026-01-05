// ActorObserve.cpp
#include "ActorObserve.h"

ActorObserve::ActorObserve(QObject* parent)
    : Observe(parent)
    , m_actor(this) // Actor 绑定 owner=this，用于 invokeMethod 在本线程处理
{
}

ActorObserve::~ActorObserve() = default;

void ActorObserve::OnDataReceived(const QString& dataType,
                                  const QString& tag,
                                  const QVariant& data)
{
    // 拷贝数据到任务（QVariant 隐式共享，拷贝成本低）
    const QString dt = dataType;
    const QString tg = tag;
    const QVariant dv = data;

    // 投递到 mailbox：确保 ObserveData 在本线程串行执行
    m_actor.Post([this, dt, tg, dv]() {
        this->ObserveData(dt, tg, dv);
    });
}
