#pragma once

#include "LogEntry.h"
#include <QString>
#include <QObject>
#include <memory>

/*
 * Logger - 全局日志服务
 * ============================================================
 * 统一日志记录入口，支持：
 * - 日志级别过滤
 * - 多输出目标（Mediator、文件、控制台）
 *
 * 使用方式：
 *   Logger::initialize(mediator);
 *   Logger::info("ModuleName", "message");
 *   Logger::error("Modbus", "Connection failed");
 *
 * 便捷宏（推荐）：
 *   LOG_DEBUG("Module", "message")
 *   LOG_INFO("Module", "message")
 *   LOG_WARN("Module", "message")
 *   LOG_ERROR("Module", "message")
 */
class Logger : public QObject {
    Q_OBJECT
public:
    // 初始化（连接 Mediator）
    static void initialize(QObject* mediator);

    // 静态便捷方法
    static void debug(const QString& module, const QString& message);
    static void info(const QString& module, const QString& message);
    static void warn(const QString& module, const QString& message);
    static void error(const QString& module, const QString& message);

    // 条件日志
    static void logIf(bool condition, LogLevel level,
                      const QString& module, const QString& message);

    // 设置最小日志级别
    static void setMinLevel(LogLevel level);
    static LogLevel minLevel();

    // 检查是否应该记录
    static bool shouldLog(LogLevel level);

    // 清理
    static void shutdown();

signals:
    // 直接发送日志信号（用于 UI 实时显示）
    void logEmitted(const LogEntry& entry);

private:
    static void emitLog(LogLevel level, const QString& module, const QString& message);

private:
    static QObject* s_mediator;
    static LogLevel s_minLevel;
    static bool s_initialized;
};

// 便捷宏
#define LOG_DEBUG(module, message) Logger::debug(module, message)
#define LOG_INFO(module, message) Logger::info(module, message)
#define LOG_WARN(module, message) Logger::warn(module, message)
#define LOG_ERROR(module, message) Logger::error(module, message)
