#pragma once

#include <QString>

/*
 * AppConfig - 应用程序配置结构体
 * ============================================================
 * 集中管理所有配置项
 *
 * 使用方式：
 *   AppConfig config;
 *   config.modbus.host = "192.168.1.100";
 *   config.modbus.port = 502;
 */
struct ModbusConfig {
    QString host = "127.0.0.1";
    int port = 502;
    int timeout = 1000;      // 读取超时(ms)
    int pollInterval = 200;  // 轮询间隔(ms)
};

struct LoggingConfig {
    QString minLevel = "Info";  // Debug, Info, Warn, Error
    int maxCount = 1000;        // 内存中保留最大条数
    QString databasePath = "logs.db";
};

struct AppConfig {
    ModbusConfig modbus;
    LoggingConfig logging;
    QString appName = "MediatorMVAQt";
    QString version = "1.0.0";
};
