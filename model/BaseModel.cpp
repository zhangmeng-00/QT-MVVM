#include "BaseModel.h"

BaseModel::BaseModel(QObject* parent, bool useSeparateThread)
    : ActorObserve(parent, useSeparateThread)
{
}

void BaseModel::log(const QString& modelName,
                    LogLevel level,
                    const QString& message)
{
    LogEntry logEntry;
    logEntry.modelName = modelName;
    logEntry.logLevel = level;
    logEntry.logMessage = message;

    Publish("user/logging", QVariant::fromValue<LogEntry>(logEntry));
}
