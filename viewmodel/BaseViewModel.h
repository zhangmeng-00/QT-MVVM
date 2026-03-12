#pragma once
#include <QObject>
#include <QDateTime>

#include "core/actor/ActorObserve.h"
#include "core/model/LogEntry.h"

/*
 * BaseViewModel
 * ============================================================
 * - ViewModel 也是 Observe（订阅总线）
 * - 但它对 View 的输出是：Qt Property + signals
 */
class BaseViewModel : public ActorObserve {
    Q_OBJECT
public:
    explicit BaseViewModel(QObject* parent = nullptr)
        : ActorObserve(parent)
    {}

protected:
    /*
     * log
     * --------------------------------------------------------
     * 默认日志发布：user/logging（用于 ViewModel）
     */
    virtual void log(const QString& modelName,
                     LogLevel level,
                     const QString& message)
    {
        LogEntry logEntry;
        logEntry.timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        logEntry.modelName = modelName;
        logEntry.logLevel = level;
        logEntry.logMessage = message;

        Publish("user/logging", QVariant::fromValue<LogEntry>(logEntry));
    }

protected:
           // 子类实现 ObserveData(tag, value)
};
