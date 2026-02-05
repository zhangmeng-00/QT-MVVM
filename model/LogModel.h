#pragma once

#include "BaseModel.h"
#include "LogEntry.h"
#include "actor/ActorObserve.h"
#include <QObject>
#include <QString>
#include <QVariant>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>

/*
 * LogModel
 * ============================================================
 * 日志模块，负责处理日志记录和存储
 * 
 * 功能：
 * -----------------------------------------------------------
 * - 订阅 user/logging 标签，接收日志数据
 * - 将日志数据存储到 SQLite 数据库
 * - 查询最新100条日志数据并发布为 user/logList
 */
class LogModel : public ActorObserve {
    Q_OBJECT
public:
    /*
     * LogModel 构造函数
     * --------------------------------------------------------
     * @param parent: 父对象
     * @param useSeparateThread: 是否使用单独的线程运行
     */
    explicit LogModel(QObject* parent = nullptr,
                     bool useSeparateThread = false);
    ~LogModel() override;

protected:
    // 设置订阅
    void SetupSubscriptions() override;
    // 处理订阅数据
    void ObserveData(const QString& tag, const QVariant& value) override;

private:
    // 初始化数据库
    bool initDatabase();
    // 插入日志记录
    bool insertLog(const LogEntry& logEntry);
    // 查询最新100条日志
    QList<LogEntry> queryLatestLogs();
    // 发布日志列表
    void publishLogList();

private:
    QSqlDatabase m_db;
    QString m_dbPath;
};