#include "LogModel.h"
#include "core/policy/AlwaysPolicy.h"
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <memory>
#include <QThread>

using namespace std;

LogModel::LogModel(QObject* parent, bool useSeparateThread)
    : ActorObserve(parent, useSeparateThread),
      m_dbPath("./logs.db")
{
    qDebug() << "LogModel 构造函数线程ID:" << QThread::currentThreadId();

    // 初始化数据库
    if (!initDatabase()) {
        qCritical() << "Failed to initialize database";
    } else {
        // 如果数据库为空，插入一条初始日志
        QSqlQuery query(m_db);
        query.exec("SELECT COUNT(*) FROM log_entries");
        if (query.next() && query.value(0).toInt() == 0) {
            // 插入初始日志
            LogEntry initEntry;
            initEntry.timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            initEntry.modelName = "System";
            initEntry.logLevel = LogLevel::INFO;
            initEntry.logMessage = "Application started";
            insertLog(initEntry);
        }
        // 注意：不在这里调用 publishLogList()，因为订阅还没建立
        // 订阅建立后会在 SetupSubscriptions 中发布
    }
}

LogModel::~LogModel()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

void LogModel::SetupSubscriptions()
{
    // 使用带QVariant的订阅，明确指定数据类型
    Subscribe("user/logging", QVariant::fromValue(LogEntry()), std::make_shared<AlwaysPolicy>());

    // 订阅建立后，发布当前日志列表
    publishLogList();
}

void LogModel::ObserveData(const QString& tag, const QVariant& value)
{
    qDebug() << "LogModel::ObserveData 线程ID:" << QThread::currentThreadId();

    if (tag == "user/logging") {
        // 从 QVariant 中提取 LogEntry 结构体
        LogEntry logEntry = value.value<LogEntry>();
        // 插入日志到数据库
        if (insertLog(logEntry)) {
            // 插入成功后，查询最新100条日志并发布
            publishLogList();
        }
    }
}

bool LogModel::initDatabase()
{
    // 移除旧的连接（如果存在）
    if (QSqlDatabase::contains("logConnection")) {
        m_db = QSqlDatabase::database("logConnection");
    } else {
        // 添加 SQLite 数据库连接
        m_db = QSqlDatabase::addDatabase("QSQLITE", "logConnection");
        m_db.setDatabaseName(m_dbPath);
    }

    // 打开数据库
    if (!m_db.open()) {
        qCritical() << "Failed to open database:" << m_db.lastError().text();
        return false;
    }

    QSqlQuery query(m_db);

    // 创建 applications 表
    QString createApplicationsTable = R"(
        CREATE TABLE IF NOT EXISTS applications (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            app_name TEXT NOT NULL UNIQUE
        );
    )";
    if (!query.exec(createApplicationsTable)) {
        qCritical() << "Failed to create applications table:" << query.lastError().text();
        return false;
    }

    // 创建 log_levels 表
    QString createLogLevelsTable = R"(
        CREATE TABLE IF NOT EXISTS log_levels (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            level_name TEXT NOT NULL UNIQUE
        );
    )";
    if (!query.exec(createLogLevelsTable)) {
        qCritical() << "Failed to create log_levels table:" << query.lastError().text();
        return false;
    }

    // 创建 log_sources 表
    QString createLogSourcesTable = R"(
        CREATE TABLE IF NOT EXISTS log_sources (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            logger_name TEXT NOT NULL UNIQUE
        );
    )";
    if (!query.exec(createLogSourcesTable)) {
        qCritical() << "Failed to create log_sources table:" << query.lastError().text();
        return false;
    }

    // 创建 log_entries 表
    QString createLogEntriesTable = R"(
        CREATE TABLE IF NOT EXISTS log_entries (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            -- 日志记录时间，默认当前时间本地时间
            timestamp DATETIME DEFAULT (datetime('now','localtime')),
            level_id INTEGER NOT NULL,
            message TEXT NOT NULL,
            source_id INTEGER NOT NULL,
            app_id INTEGER NOT NULL,
            FOREIGN KEY (level_id) REFERENCES log_levels(id),
            FOREIGN KEY (source_id) REFERENCES log_sources(id),
            FOREIGN KEY (app_id) REFERENCES applications(id)
        );
    )";
    if (!query.exec(createLogEntriesTable)) {
        qCritical() << "Failed to create log_entries table:" << query.lastError().text();
        return false;
    }

    // 插入默认的日志级别
    QString insertLogLevels = R"(
        INSERT OR IGNORE INTO log_levels (id, level_name) VALUES
        (1, 'Debug'),
        (2, 'INFO'),
        (3, 'WARN'),
        (4, 'ERROR'),
        (5, 'FATAL');
    )";
    if (!query.exec(insertLogLevels)) {
        qCritical() << "Failed to insert log levels:" << query.lastError().text();
        return false;
    }

    // 插入默认的应用
    QString insertDefaultApp = R"(
        INSERT OR IGNORE INTO applications (app_name) VALUES ('DefaultApp');
    )";
    if (!query.exec(insertDefaultApp)) {
        qCritical() << "Failed to insert default app:" << query.lastError().text();
        return false;
    }

    return true;
}

bool LogModel::insertLog(const LogEntry& logEntry)
{
    if (!m_db.isOpen()) {
        if (!initDatabase()) {
            return false;
        }
    }

    QSqlQuery query(m_db);

    // 开始事务
    if (!m_db.transaction()) {
        qCritical() << "Failed to start transaction:" << m_db.lastError().text();
        return false;
    }

    // 1. 获取或插入 application（使用默认应用）
    int appId = 1; // 默认应用ID

    // 2. 获取或插入 log_source
    int sourceId = 0;
    query.prepare("SELECT id FROM log_sources WHERE logger_name = ?");
    query.addBindValue(logEntry.modelName);
    if (query.exec() && query.next()) {
        sourceId = query.value(0).toInt();
    } else {
        query.prepare("INSERT INTO log_sources (logger_name) VALUES (?)");
        query.addBindValue(logEntry.modelName);
        if (!query.exec()) {
            qCritical() << "Failed to insert log_source:" << query.lastError().text();
            m_db.rollback();
            return false;
        }
        sourceId = query.lastInsertId().toInt();
    }

    // 3. 获取 log_level
    int levelId = static_cast<int>(logEntry.logLevel) + 1; // 日志级别从1开始

    // 4. 插入 log_entry
    query.prepare(R"(
        INSERT INTO log_entries (level_id, message, source_id, app_id)
        VALUES (?, ?, ?, ?)
    )");
    query.addBindValue(levelId);
    query.addBindValue(logEntry.logMessage);
    query.addBindValue(sourceId);
    query.addBindValue(appId);

    if (!query.exec()) {
        qCritical() << "Failed to insert log_entry:" << query.lastError().text();
        m_db.rollback();
        return false;
    }

    // 提交事务
    if (!m_db.commit()) {
        qCritical() << "Failed to commit transaction:" << m_db.lastError().text();
        return false;
    }

    return true;
}

QList<LogEntry> LogModel::queryLatestLogs()
{
    QList<LogEntry> logs;

    if (!m_db.isOpen()) {
        if (!initDatabase()) {
            return logs;
        }
    }

    QSqlQuery query(m_db);
    // 查询最新100条日志，按时间倒序，关联相关表
    QString querySql = R"(
        SELECT le.timestamp, ls.logger_name, ll.id - 1, le.message
        FROM log_entries le
        JOIN log_sources ls ON le.source_id = ls.id
        JOIN log_levels ll ON le.level_id = ll.id
        ORDER BY le.timestamp DESC
        LIMIT 100
    )";

    if (!query.exec(querySql)) {
        qCritical() << "Failed to query logs:" << query.lastError().text();
        return logs;
    }

    while (query.next()) {
        LogEntry logEntry;
        logEntry.timestamp = query.value(0).toString();
        logEntry.modelName = query.value(1).toString();
        logEntry.logLevel = static_cast<LogLevel>(query.value(2).toInt());
        logEntry.logMessage = query.value(3).toString();
        logs.append(logEntry);
    }

    return logs;
}

void LogModel::publishLogList()
{
    QList<LogEntry> logs = queryLatestLogs();
    // 将日志列表转换为 QVariant 并发布
    QVariant logListVariant = QVariant::fromValue(logs);
    Publish("user/logList", logListVariant);
}
