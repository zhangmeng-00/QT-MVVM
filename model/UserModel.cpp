#include "UserModel.h"
#include "core/policy/AlwaysPolicy.h"
#include "common/TestStruct.h"

void UserModel::SetupSubscriptions()
{
  Subscribe("user/score", std::make_shared<AlwaysPolicy>());
  Subscribe("sensor/sample", std::make_shared<AlwaysPolicy>());
}

void UserModel::ObserveData(const QString &tag, const QVariant &value)
{

    if (tag == "user/score") {
        int score = value.toInt();
        int newLevel = score / 100;

        if (newLevel != m_level) {
            m_level = newLevel;
            Publish("user/level", m_level);
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
