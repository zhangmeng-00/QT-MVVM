#include "SensorViewModel.h"
#include "AlwaysPolicy.h"
#include "Logger.h"
#include "Tags.h"

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
        Publish(TAG_SENSOR_TEMPERATURE, temp);
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
        Publish(TAG_SENSOR_TARGET_TEMP, value);
    }
}

void SensorViewModel::onValueChangedDouble(double value, const QString& senderId)
{
    qDebug() << "[SensorVM] onValueChangedDouble:" << senderId << value;

    if (senderId == "doubleSpinBoxGain") {
        m_gain = value;
        m_gainText = QString::number(value, 'f', 3);
        emit gainTextChanged();
        Publish(TAG_SENSOR_GAIN, value);
        LOG_INFO("SensorViewModel", QString("Gain changed to %1").arg(value));
    }
}

void SensorViewModel::SetupSubscriptions()
{
    // 使用带QVariant的订阅，明确指定数据类型
    Subscribe(TAG_SENSOR_TEMPERATURE, QVariant(0), std::make_shared<AlwaysPolicy>());

    // 新增：UI事件发布的tag
    Subscribe(TAG_SENSOR_PUBLISH_TEMP, QVariant(false), std::make_shared<AlwaysPolicy>());
    Subscribe(TAG_SENSOR_TARGET_TEMP, QVariant(0), std::make_shared<AlwaysPolicy>());
    Subscribe(TAG_SENSOR_GAIN, QVariant(0.0), std::make_shared<AlwaysPolicy>());
}

void SensorViewModel::ObserveData(const QString& tag, const QVariant& value)
{
    if (tag == TAG_SENSOR_TEMPERATURE) {
        m_temperatureText = QString("%1 °C").arg(value.toInt());
        emit temperatureTextChanged();

        // 你原来这段我保留（演示用途）
        Publish(TAG_USER_LEVEL, 3);
        // 使用带QVariant的订阅，明确指定数据类型
        Subscribe(TAG_USER_LEVEL, QVariant(0), std::make_shared<AlwaysPolicy>(true));
        return;
    }

    if (tag == TAG_USER_LEVEL) {
        qDebug() << TAG_USER_LEVEL << value;
        return;
    }

    // 处理UI事件发布的tag
    if (tag == TAG_SENSOR_PUBLISH_TEMP) {
        // 收到按钮点击，随机生成温度
        int temp = QRandomGenerator::global()->bounded(20, 40);
        Publish(TAG_SENSOR_TEMPERATURE, QVariant(temp));
        LOG_INFO("SensorViewModel", QString("Publishing temperature %1").arg(temp));
        return;
    }

    if (tag == TAG_SENSOR_TARGET_TEMP) {
        m_targetTempText = QString::number(value.toInt());
        emit targetTempTextChanged();
        return;
    }

    if (tag == TAG_SENSOR_GAIN) {
        m_gainText = QString::number(value.toDouble(), 'f', 2);
        emit gainTextChanged();
        return;
    }
}
