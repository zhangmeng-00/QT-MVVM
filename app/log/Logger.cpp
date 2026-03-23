#include "Logger.h"
#include <QDateTime>
#include <QMetaObject>
#include <QDebug>

// 静态成员初始化
QObject* Logger::s_mediator = nullptr;
LogLevel Logger::s_minLevel = LogLevel::INFO;
bool Logger::s_initialized = false;

void Logger::initialize(QObject* mediator) {
    s_mediator = mediator;
    s_initialized = true;
    qDebug() << "Logger initialized";
}

void Logger::shutdown() {
    s_mediator = nullptr;
    s_initialized = false;
}

void Logger::setMinLevel(LogLevel level) {
    s_minLevel = level;
}

LogLevel Logger::minLevel() {
    return s_minLevel;
}

bool Logger::shouldLog(LogLevel level) {
    return static_cast<int>(level) >= static_cast<int>(s_minLevel);
}

void Logger::debug(const QString& module, const QString& message) {
    emitLog(LogLevel::Debug, module, message);
}

void Logger::info(const QString& module, const QString& message) {
    emitLog(LogLevel::INFO, module, message);
}

void Logger::warn(const QString& module, const QString& message) {
    emitLog(LogLevel::WARN, module, message);
}

void Logger::error(const QString& module, const QString& message) {
    emitLog(LogLevel::ERROR, module, message);
}

void Logger::logIf(bool condition, LogLevel level,
                   const QString& module, const QString& message) {
    if (condition) {
        emitLog(level, module, message);
    }
}

void Logger::emitLog(LogLevel level, const QString& module, const QString& message) {
    // 级别过滤
    if (!shouldLog(level)) {
        return;
    }

    // 构建日志条目
    LogEntry entry;
    entry.timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    entry.modelName = module;
    entry.logLevel = level;
    entry.logMessage = message;

    // 通过 Mediator 发布（如果已初始化）
    if (s_mediator) {
        QMetaObject::invokeMethod(s_mediator, "Publish",
            Q_ARG(QString, "user/logging"),
            Q_ARG(QVariant, QVariant::fromValue(entry)));
    }

    // 同时输出到控制台（调试用）
    QString levelStr;
    switch (level) {
        case LogLevel::Debug: levelStr = "DEBUG"; break;
        case LogLevel::INFO: levelStr = "INFO"; break;
        case LogLevel::WARN: levelStr = "WARN"; break;
        case LogLevel::ERROR: levelStr = "ERROR"; break;
        case LogLevel::FATAL: levelStr = "FATAL"; break;
        default: levelStr = "UNKNOWN"; break;
    }
    qDebug() << QString("[%1][%2] %3").arg(levelStr, module, message);
}
