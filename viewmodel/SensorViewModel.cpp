#include "SensorViewModel.h"
#include "core/policy/AlwaysPolicy.h"

SensorViewModel::SensorViewModel(QObject* parent)
    : BaseViewModel(parent)
{
    m_publishTemperatureCommand = new SimpleCommand(
        [this] { this->publishCommand(); },
        [] { return true; },
        this
        );

    // ✅ slider::valueChanged(int) —— args[0] 是 int
    m_setTargetTemperatureCommand = new SimpleCommand(
        [this](const QVariantList& args) {
            const int v = args.value(0).toInt();
            m_targetTemp = v;
            const QString text = QString::number(v);
            if (m_targetTempText != text) {
                m_targetTempText = text;
                emit targetTempTextChanged();
            }

            // 需要的话也可以 Publish 给下游模块
            Publish("sensor/target_temp", m_targetTemp);
        },
        nullptr,
        this
        );

    // ✅ doubleSpinBox::valueChanged(double) —— args[0] 是 double
    m_gainChangedCommand = new SimpleCommand(
        [this](const QVariantList& args) {
            const double g = args.value(0).toDouble();
            m_gain = g;

            const QString text = QString::number(g, 'f', 3);
            if (m_gainText != text) {
                m_gainText = text;
                emit gainTextChanged();
            }

            Publish("sensor/gain", m_gain);
        },
        nullptr,
        this
        );
}

void SensorViewModel::publishCommand()
{
    int temperature = QRandomGenerator::global()->bounded(20, 80);
    qDebug() << "[VM] publish temperature =" << temperature;
    Publish("sensor/temperature", temperature);
}

void SensorViewModel::ObserveData(const QString& tag, const QVariant& value)
{
    if (tag == "sensor/temperature") {
        m_temperatureText = QString("%1 °C").arg(value.toInt());
        emit temperatureTextChanged();

        // 你原来这段我保留（演示用途）
        Publish("user/level", 3);
        Subscribe("user/level", std::make_shared<AlwaysPolicy>());
        return;
    }

    if (tag == "user/level") {
        qDebug() << "user/level" << value;
        return;
    }
}
