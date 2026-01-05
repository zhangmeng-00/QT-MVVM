// SensorModel.h
#pragma once

#include "core/actor/ActorObserve.h"

/*
 * SensorModel（Actor）
 * ============================================================
 * 演示：作为 Actor 运行在独立线程
 * - 可以订阅/发布 sensor:* 数据
 */
class SensorModel : public ActorObserve {
    Q_OBJECT
public:
    explicit SensorModel(QObject* parent = nullptr);

protected:
    void ObserveData(const QString& dataType,
                     const QString& tag,
                     const QVariant& data) override;
};
