#include "UserModel.h"
#include "core/policy/AlwaysPolicy.h"
#include "common/TestStruct.h"
#include <memory>
#include <QVariant>

void UserModel::SetupSubscriptions()
{
  // 使用带QVariant的订阅，明确指定数据类型
  Subscribe("user/score", QVariant(0), std::make_shared<AlwaysPolicy>());
  Subscribe("sensor/sample", QVariant::fromValue(SensorSample()), std::make_shared<AlwaysPolicy>());
}

void UserModel::ObserveData(const QString &tag, const QVariant &value)
{

    if (tag == "user/score") {
        int score = value.toInt();
        int newLevel = score / 100;

        if (newLevel != m_level) {
            m_level = newLevel;
            Publish("user/level", QVariant::fromValue(m_level));
            log("UserModel",
                LogLevel::WARN,
                QString("User level updated to %1").arg(m_level));
        }
    }
    if (tag == "sensor/sample") {
        SensorSample sample = value.value<SensorSample>();
        qDebug() << "[SomeModel] sample:"
                 << sample.sensorId
                 << sample.temperature
                 << sample.pressure
                 << sample.timestamp;
    }
}
