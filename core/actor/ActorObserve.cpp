#include "actor/ActorObserve.h"

ActorObserve::ActorObserve(QObject* parent)
    : Observe(parent)
    , m_actor(this)
{
}

void ActorObserve::OnDataReceived(const QString& /*typeKey*/,
                                  const QString& tag,
                                  const QVariant& value)
{
    /*
     * 关键点：
     * --------------------------------------------------------
     * - typeKey 只用于路由
     * - 业务层不需要知道 typeKey
     * - 所以这里只把 tag + value 投递给 ObserveData
     */
    m_actor.Post([this, tag, value]() {
        ObserveData(tag, value);   // ✅ 正确：2 个参数
    });
}
