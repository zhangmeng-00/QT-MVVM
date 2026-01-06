#pragma once

#include <QObject>
#include <QString>
#include <QVariant>

#include "viewmodel/BaseViewModel.h"

/*
 * SensorViewModel
 * ============================================================
 * Subscribe:
 * - sensor/temperature
 *
 * Output:
 * - temperatureText (formatted)
 */
class SensorViewModel : public BaseViewModel {
    Q_OBJECT
    Q_PROPERTY(QString temperatureText READ temperatureText NOTIFY temperatureTextChanged)

public:
    explicit SensorViewModel(QObject* parent = nullptr)
        : BaseViewModel(parent)
    {
    }

    QString temperatureText() const
    {
        return m_temperatureText;
    }

signals:
    void temperatureTextChanged();

public slots:
    void publishCommand()
    {
        int temp = qrand() % 40;
        Publish("sensor/temperature", temp);
    }

protected:
    void ObserveData(const QString& tag,
                     const QVariant& value) override
    {
        if (tag == "sensor/temperature") {
            m_temperatureText =
                QString("%1 degC").arg(value.toInt());
            emit temperatureTextChanged();
        }
    }

private:
    QString m_temperatureText;
};
