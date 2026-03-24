#pragma once

#include "ActorObserve.h"

/*
 * SensorModel
 * ============================================================
 * 示例传感器模型
 */
class SensorModel : public ActorObserve {
    Q_OBJECT
public:
    explicit SensorModel(QObject* parent = nullptr)
        : ActorObserve(parent)
    {}

protected:
    void ObserveData(const QString& tag,
                     const QVariant& value) override
    {
        if (tag == "temperature") {
            // 处理温度
        }
    }
};
