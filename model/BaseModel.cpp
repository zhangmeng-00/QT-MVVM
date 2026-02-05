#include "BaseModel.h"

#include <QDateTime>

BaseModel::BaseModel(QObject* parent, bool useSeparateThread)
    : ActorObserve(parent, useSeparateThread)
{
}

void BaseModel::log(const QString& modelName,
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
