// SensorModel.cpp
#include "SensorModel.h"

SensorModel::SensorModel(QObject* parent)
    : ActorObserve(parent)
{
}

void SensorModel::ObserveData(const QString& dataType,
                              const QString& tag,
                              const QVariant& data)
{
    Q_UNUSED(dataType)
    Q_UNUSED(tag)
    Q_UNUSED(data)
    // 示例：传感器模型收到数据后可做滤波/计算
}
