#ifndef LOG_ENTRY_H
#define LOG_ENTRY_H

#include <string>

// LogLevel Enum
enum class LogLevel {
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};

// LogEntry Struct
struct LogEntry {
    std::string modelName;
    LogLevel logLevel;
    std::string logMessage;
};

#endif // LOG_ENTRY_H