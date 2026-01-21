#pragma once

#include "core/actor/ActorObserve.h"
#include "common/TypeKey.h"
#include "core/policy/AlwaysPolicy.h"
#include <QFile>
#include <qfileinfo.h>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

class RecorderActor : public ActorObserve {
    Q_OBJECT
public:
    explicit RecorderActor(const QString& filePath,
                           QObject* parent = nullptr)
        : ActorObserve(parent)
        , m_file(filePath)
    {
        bool isNew = !QFileInfo(filePath).exists();

        if (!m_file.open(QIODevice::WriteOnly
                         | QIODevice::Append
                         | QIODevice::Text)) {
            qDebug() << "[Recorder] open failed:" << m_file.errorString();
            return;
        }

        m_stream.setDevice(&m_file);

        if (isNew) {
            m_stream << "timestamp,topic,type,value\n";
            m_stream.flush();
        }
    }

    ~RecorderActor() {
        m_stream.flush();
        m_file.close();
    }

    void Init()
    {
        qDebug() << "[Recorder] Init called" << this;

        Subscribe("user/score", std::make_shared<AlwaysPolicy>());
        Subscribe("user/level", std::make_shared<AlwaysPolicy>());
        Subscribe("sensor/temperature", std::make_shared<AlwaysPolicy>());
    }


protected:
    void ObserveData(const QString& tag,
                     const QVariant& value) override
    {
        qDebug() << "[Recorder] received:" << tag << value;
        QString timeStr = QDateTime::currentDateTime()
                              .toString("yyyy-MM-dd HH:mm:ss.zzz");

        QString type = TypeKey::FromVariant(value);

        m_stream
            << timeStr << ","
            << tag << ","
            << type << ","
            << value.toString() << "\n";

        m_stream.flush();   // 🔒 核心
    }

private:
    QFile m_file;
    QTextStream m_stream;
};
