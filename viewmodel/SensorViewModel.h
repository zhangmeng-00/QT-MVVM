#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <QRandomGenerator>
#include <QDebug>

#include "BaseViewModel.h"

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

signals:
    void temperatureTextChanged();
    void targetTempTextChanged();
    void gainTextChanged();

public:
    // 事件虚函数重写
    Q_INVOKABLE void onClicked(const QString& senderId) override;
    Q_INVOKABLE void onValueChanged(int value, const QString& senderId) override;
    Q_INVOKABLE void onValueChangedDouble(double value, const QString& senderId) override;

public slots:
    void SetupSubscriptions() override;

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
};
