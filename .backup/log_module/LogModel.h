#ifndef LOG_MODEL_H
#define LOG_MODEL_H

#include "BaseModel.h"
#include <string>
#include <vector>
#include "LogEntry.h"

class LogModel : public BaseModel {
public:
    LogModel();
    virtual ~LogModel();

    void handleLogging(const LogStruct& logData);
    std::vector<LogStruct> fetchLatestLogs(int count);

    // Add subscription logic for user/logging tag
    void subscribeToLoggingTag();
    // Add logic to publish latest logs
    void publishLatestLogs();

private:
    void initializeDatabase();
    void insertLogIntoDatabase(const LogStruct& logData);
};

#endif // LOG_MODEL_H