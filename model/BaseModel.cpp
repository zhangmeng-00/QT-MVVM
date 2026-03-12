#include "BaseModel.h"

#include <QDateTime>

BaseModel::BaseModel(QObject* parent, bool useSeparateThread)
    : ActorObserve(parent, useSeparateThread)
{
}

/**
 * @brief 记录一条结构化日志，并通过发布-订阅模式广播出去。
 *
 * 该函数是所有模型类的通用日志记录工具。它不直接将日志写入文件或控制台，
 * 而是创建一个 LogEntry 对象，填充所有相关信息，然后将其发布到
 * "user/logging" 主题。系统中的其他模块（如ViewModel或日志服务）
 * 可以订阅此主题来接收和处理日志。
 *
 * @param modelName 产生日志的模块名称 (例如 "UserModel")。
 * @param level 日志的级别 (例如 Info, Warning, Error)。
 * @param message 具体的日志消息内容。
 */
void BaseModel::log(const QString& modelName,
                    LogLevel level,
                    const QString& message)
{
    // 创建一个日志条目结构体
    LogEntry logEntry;

    // 填充日志信息
    // 设置当前时间戳，格式为 "年-月-日 时:分:秒"
    logEntry.timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    // 设置日志来源的模块名
    logEntry.modelName = modelName;
    // 设置日志级别
    logEntry.logLevel = level;
    // 设置日志消息
    logEntry.logMessage = message;

    // 将结构化的日志条目发布到 "user/logging" 主题
    // 使用 QVariant::fromValue 将自定义的 LogEntry 类型包装起来，以便通过消息总线传递
    Publish("user/logging", QVariant::fromValue<LogEntry>(logEntry));
}
