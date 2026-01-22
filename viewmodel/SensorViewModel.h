#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QRandomGenerator>
#include <QDebug>

#include "viewmodel/BaseViewModel.h"
#include "core/command/ICommand.h"
#include "core/command/SimpleCommand.h"

class SensorViewModel : public BaseViewModel {
    Q_OBJECT
    Q_PROPERTY(QString temperatureText READ temperatureText NOTIFY temperatureTextChanged)

    // ✅ 新增：给 slider / doubleSpinBox 显示用
    Q_PROPERTY(QString targetTempText READ targetTempText NOTIFY targetTempTextChanged)
    Q_PROPERTY(QString gainText READ gainText NOTIFY gainTextChanged)

public:
    explicit SensorViewModel(QObject* parent = nullptr);

    QString temperatureText() const { return m_temperatureText; }
    QString targetTempText() const { return m_targetTempText; }
    QString gainText() const { return m_gainText; }

    ICommand* publishTemperatureCommand() const { return m_publishTemperatureCommand; }

    // ✅ 新增：参数命令
    ICommand* setTargetTemperatureCommand() const { return m_setTargetTemperatureCommand; }
    ICommand* gainChangedCommand() const { return m_gainChangedCommand; }

signals:
    void temperatureTextChanged();
    void targetTempTextChanged();
    void gainTextChanged();

public slots:
    void publishCommand();

protected:
    void ObserveData(const QString& tag,
                     const QVariant& value) override;

private:
    QString m_temperatureText;

    // ✅ 新增：内部状态
    int m_targetTemp = 30;
    double m_gain = 1.0;

    QString m_targetTempText = "30";
    QString m_gainText = "1.000";

    SimpleCommand* m_publishTemperatureCommand = nullptr;

    // ✅ 新增：参数命令
    SimpleCommand* m_setTargetTemperatureCommand = nullptr;
    SimpleCommand* m_gainChangedCommand = nullptr;
};
