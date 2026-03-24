#include "Logger.h"
#include <QDateTime>
#include <QDebug>

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

void Logger::setMinLevel(LogLevel level) {
    m_minLevel = level;
}

bool Logger::shouldLog(LogLevel level) const {
    return static_cast<int>(level) >= static_cast<int>(m_minLevel);
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

    // 通过继承的 Publish 发布到 Mediator
    Publish("user/logging", QVariant::fromValue(entry));

    // 发送信号（用于 UI 实时显示）
    emit logEmitted(entry);

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