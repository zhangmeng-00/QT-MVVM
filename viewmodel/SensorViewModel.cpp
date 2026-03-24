#include "SensorViewModel.h"
#include "AlwaysPolicy.h"
#include "Logger.h"

SensorViewModel::SensorViewModel(QObject* parent)
    : BaseViewModel(parent)
{
}

// ========== 事件虚函数实现 ==========

void SensorViewModel::onClicked(const QString& senderId)
{
    qDebug() << "[SensorVM] onClicked:" << senderId;

    if (senderId == "btnPublishTemperature") {
        int temp = QRandomGenerator::global()->bounded(20, 40);
        Publish("sensor/temperature", temp);
        LOG_INFO("SensorViewModel", QString("Publishing temperature %1").arg(temp));
    }
}

void SensorViewModel::onValueChanged(int value, const QString& senderId)
{
    qDebug() << "[SensorVM] onValueChanged int:" << senderId << value;

    if (senderId == "horizontalSliderTemp") {
        m_targetTemp = value;
        m_targetTempText = QString::number(value);
        emit targetTempTextChanged();
        Publish("sensor/target_temperature", value);
    }
}

void SensorViewModel::onValueChangedDouble(double value, const QString& senderId)
{
    qDebug() << "[SensorVM] onValueChangedDouble:" << senderId << value;

    if (senderId == "doubleSpinBoxGain") {
        m_gain = value;
        m_gainText = QString::number(value, 'f', 3);
        emit gainTextChanged();
        Publish("sensor/gain", value);
        LOG_INFO("SensorViewModel", QString("Gain changed to %1").arg(value));
    }
}

void SensorViewModel::SetupSubscriptions()
{
    // 使用带QVariant的订阅，明确指定数据类型
    Subscribe("sensor/temperature", QVariant(0), std::make_shared<AlwaysPolicy>());

    // 新增：UI事件发布的tag
    Subscribe("sensor/publish_temperature", QVariant(false), std::make_shared<AlwaysPolicy>());
    Subscribe("sensor/target_temperature", QVariant(0), std::make_shared<AlwaysPolicy>());
    Subscribe("sensor/gain", QVariant(0.0), std::make_shared<AlwaysPolicy>());
}

void SensorViewModel::ObserveData(const QString& tag, const QVariant& value)
{
    if (tag == "sensor/temperature") {
        m_temperatureText = QString("%1 °C").arg(value.toInt());
        emit temperatureTextChanged();

        // 你原来这段我保留（演示用途）
        Publish("user/level", 3);
        // 使用带QVariant的订阅，明确指定数据类型
        Subscribe("user/level", QVariant(0), std::make_shared<AlwaysPolicy>());
        return;
    }

    if (tag == "user/level") {
        qDebug() << "user/level" << value;
        return;
    }

    // 处理UI事件发布的tag
    if (tag == "sensor/publish_temperature") {
        // 收到按钮点击，随机生成温度
        int temp = QRandomGenerator::global()->bounded(20, 40);
        Publish("sensor/temperature", QVariant(temp));
        LOG_INFO("SensorViewModel", QString("Publishing temperature %1").arg(temp));
        return;
    }

    if (tag == "sensor/target_temperature") {
        m_targetTempText = QString::number(value.toInt());
        emit targetTempTextChanged();
        return;
    }

    if (tag == "sensor/gain") {
        m_gainText = QString::number(value.toDouble(), 'f', 2);
        emit gainTextChanged();
        return;
    }
}
