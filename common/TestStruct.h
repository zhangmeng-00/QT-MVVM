#pragma once
#include <QString>
#include <QDateTime>
#include <QMetaType>

// 你要发布的数据结构
struct SensorSample
{
    QString  sensorId;
    double   temperature = 0.0;
    double   pressure    = 0.0;
    QDateTime timestamp;

    SensorSample() = default;
    SensorSample(QString id, double t, double p)
        : sensorId(std::move(id)), temperature(t), pressure(p), timestamp(QDateTime::currentDateTime())
    {}
};

Q_DECLARE_METATYPE(SensorSample)
