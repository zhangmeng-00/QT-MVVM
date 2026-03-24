#pragma once

#include <QString>

struct ModbusConfig {
    QString host = "127.0.0.1";
    int port = 502;
    int timeout = 1000;
    int pollInterval = 200;
};

struct LoggingConfig {
    QString minLevel = "Info";   // Debug, Info, Warn, Error
    int maxCount = 1000;         // max entries kept in memory
    QString databasePath = "logs.db";
};

struct AppConfig {
    ModbusConfig modbus;
    LoggingConfig logging;
    QString appName = "MediatorMVAQt";
    QString version = "1.0.0";
};
