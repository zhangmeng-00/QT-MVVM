#pragma once

#include "mediator/Observe.h"
#include "actor/Actor.h"

/*
 * ActorObserve
 * ============================================================
 * Observe + Actor（Mailbox）组合
 *
 * 职责：
 * ------------------------------------------------------------
 * - 保证 ObserveData 在对象所属线程串行执行
 * - 屏蔽多线程并发
 * - 不向业务层暴露 typeKey
 */
class ActorObserve : public Observe {
    Q_OBJECT
public:
    explicit ActorObserve(QObject* parent = nullptr);

protected:
    /*
     * Topic → ActorObserve 入口
     * --------------------------------------------------------
     * 该函数可能在 Mediator 线程被调用
     * 所以这里只负责投递到 Mailbox
     */
    void OnDataReceived(const QString& typeKey,
                        const QString& tag,
                        const QVariant& value) override;

private:
    Actor m_actor;
};
