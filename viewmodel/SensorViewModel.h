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

public:
    explicit SensorViewModel(QObject* parent = nullptr);

    QString temperatureText() const { return m_temperatureText; }

    // ✅ 新增：给 UI 用的 Command
    ICommand* publishTemperatureCommand() const { return m_publishTemperatureCommand; }

signals:
    void temperatureTextChanged();

public slots:
    // 🔴 原有逻辑，完全不动
    void publishCommand();

protected:
    void ObserveData(const QString& tag,
                     const QVariant& value) override;

private:
    QString m_temperatureText;

    // ✅ 新增
    SimpleCommand* m_publishTemperatureCommand = nullptr;
};
