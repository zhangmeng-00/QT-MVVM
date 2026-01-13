#include "SensorViewModel.h"
#include "core/policy/AlwaysPolicy.h"

SensorViewModel::SensorViewModel(QObject* parent)
    : BaseViewModel(parent)
{
    // Command 只是“包一层”，不改逻辑
    m_publishTemperatureCommand = new SimpleCommand(
        [this] {
            this->publishCommand();   // 👈 你原来的 slot
        },
        [] {
            return true;
        },
        this
        );
    //Subscribe("sensor/temperature",std::make_shared<AlwaysPolicy>());
}

void SensorViewModel::publishCommand()
{
    int temperature = QRandomGenerator::global()->bounded(20, 80);
    qDebug() << "[VM] publish temperature =" << temperature;
    Publish("sensor/temperature", temperature);
}

void SensorViewModel::ObserveData(const QString& tag,
                                  const QVariant& value)
{
    if (tag == "sensor/temperature") {
        m_temperatureText = QString("%1 °C").arg(value.toInt());
        emit temperatureTextChanged();
        Publish("user/level", 3);
        Subscribe("user/level",std::make_shared<AlwaysPolicy>());
    }
    if (tag == "user/level") {
        qDebug() <<"user/level"<<value;
    }
}

