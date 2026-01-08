#pragma once

#include "core/actor/ActorObserve.h"
#include "common/TypeKey.h"
#include "core/policy/AlwaysPolicy.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>

class SQLiteRecorderActor : public ActorObserve {
    Q_OBJECT
public:
    explicit SQLiteRecorderActor(const QString& dbPath,
                                 QObject* parent = nullptr);

    ~SQLiteRecorderActor();

    void Init();

protected:
    void ObserveData(const QString& tag,
                     const QVariant& value) override;

private:
    void initDatabase();

private:
    QString       m_dbPath;
    QSqlDatabase  m_db;
};
