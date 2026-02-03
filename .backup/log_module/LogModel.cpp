#include "LogModel.h"
#include "LogEntry.h"
#include <iostream>
#include <sqlite3.h>

LogModel::LogModel() {
    initializeDatabase();
}

LogModel::~LogModel() {
    // Cleanup if necessary
}

void LogModel::initializeDatabase() {
    sqlite3* db;
    char* errorMessage = nullptr;

    int rc = sqlite3_open("log_database.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* createTableQuery = "CREATE TABLE IF NOT EXISTS log_entries ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
                                  "level_id INTEGER,"
                                  "message TEXT,"
                                  "source_id INTEGER,"
                                  "app_id INTEGER);";

    rc = sqlite3_exec(db, createTableQuery, nullptr, nullptr, &errorMessage);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    sqlite3_close(db);
}

void LogModel::handleLogging(const LogStruct& logData) {
    insertLogIntoDatabase(logData);
}

void LogModel::insertLogIntoDatabase(const LogStruct& logData) {
    sqlite3* db;
    char* errorMessage = nullptr;

    int rc = sqlite3_open("log_database.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* insertQuery = "INSERT INTO log_entries (level_id, message, source_id, app_id) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    rc = sqlite3_prepare_v2(db, insertQuery, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, static_cast<int>(logData.logLevel));
    sqlite3_bind_text(stmt, 2, logData.logMessage.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, 0); // Placeholder for source_id
    sqlite3_bind_int(stmt, 4, 0); // Placeholder for app_id

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

std::vector<LogStruct> LogModel::fetchLatestLogs(int count) {
    sqlite3* db;
    std::vector<LogStruct> logs;

    int rc = sqlite3_open("log_database.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return logs;
    }

    const char* selectQuery = "SELECT level_id, message FROM log_entries ORDER BY timestamp DESC LIMIT ?;";
    sqlite3_stmt* stmt;

    rc = sqlite3_prepare_v2(db, selectQuery, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return logs;
    }

    sqlite3_bind_int(stmt, 1, count);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        LogStruct log;
        log.logLevel = static_cast<LogLevel>(sqlite3_column_int(stmt, 0));
        log.logMessage = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        logs.push_back(log);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return logs;
}