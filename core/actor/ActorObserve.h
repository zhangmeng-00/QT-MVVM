// ActorObserve.h
#pragma once

#include "../mediator/Observe.h"
#include "Actor.h"

/*
 * ActorObserve
 * ============================================================
 * Qt版 Actor（对标 LabVIEW AF Actor）
 *
 * 核心保证：
 * - Topic 推送到 OnDataReceived（可能来自 Mediator 线程）
 * - 不直接进入业务逻辑
 * - 投递到 Mailbox（Actor::Post）
 * - 最终在 ActorObserve 自己线程串行执行 ObserveData
 */
class ActorObserve : public Observe {
    Q_OBJECT
public:
    explicit ActorObserve(QObject* parent = nullptr);
    ~ActorObserve() override;

    void OnDataReceived(const QString& dataType,
                        const QString& tag,
                        const QVariant& data) override;

private:
    Actor m_actor;
};
