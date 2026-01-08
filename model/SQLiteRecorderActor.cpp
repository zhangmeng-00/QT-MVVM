#include "SQLiteRecorderActor.h"

SQLiteRecorderActor::SQLiteRecorderActor(const QString& dbPath,
                                         QObject* parent)
    : ActorObserve(parent)
    , m_dbPath(dbPath)
{
    initDatabase();
}

SQLiteRecorderActor::~SQLiteRecorderActor()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

void SQLiteRecorderActor::initDatabase()
{
    // 每个 Recorder 一个独立 connection name
    QString connName = QString("recorder_%1")
                           .arg(reinterpret_cast<quintptr>(this));

    m_db = QSqlDatabase::addDatabase("QSQLITE", connName);
    m_db.setDatabaseName(m_dbPath);

    if (!m_db.open()) {
        qDebug() << "[SQLiteRecorder] open failed:"
                 << m_db.lastError().text();
        return;
    }

    QSqlQuery q(m_db);
    q.exec(
        "CREATE TABLE IF NOT EXISTS trace ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "time_str TEXT,"
        "timestamp_ms INTEGER,"
        "topic TEXT,"
        "type TEXT,"
        "value TEXT"
        ")"
        );
}

void SQLiteRecorderActor::Init()
{
    qDebug() << "[SQLiteRecorder] Init";

    Subscribe("user/score", std::make_shared<AlwaysPolicy>());
    Subscribe("user/level", std::make_shared<AlwaysPolicy>());
    Subscribe("sensor/temperature", std::make_shared<AlwaysPolicy>());
}

void SQLiteRecorderActor::ObserveData(const QString& tag,
                                      const QVariant& value)
{
    if (!m_db.isOpen())
        return;

    QDateTime now = QDateTime::currentDateTime();

    QString timeStr = now.toString("yyyy-MM-dd HH:mm:ss.zzz");
    qint64  tsMs    = now.toMSecsSinceEpoch();
    QString type    = TypeKey::FromVariant(value);

    QSqlQuery q(m_db);
    q.prepare(
        "INSERT INTO trace "
        "(time_str, timestamp_ms, topic, type, value) "
        "VALUES (?, ?, ?, ?, ?)"
        );

    q.addBindValue(timeStr);
    q.addBindValue(tsMs);
    q.addBindValue(tag);
    q.addBindValue(type);
    q.addBindValue(value.toString());

    if (!q.exec()) {
        qDebug() << "[SQLiteRecorder] insert failed:"
                 << q.lastError().text();
    }
}
