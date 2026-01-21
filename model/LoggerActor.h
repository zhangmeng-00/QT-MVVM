#pragma once

#include "core/mediator/Observe.h"
#include "core/actor/ActorObserve.h"
#include "common/TypeKey.h"   // 你之前写的 TypeKey
#include "core/policy/AlwaysPolicy.h"
#include <QDateTime>
#include <QDebug>

class LoggerActor : public ActorObserve {
    Q_OBJECT
public:
    explicit LoggerActor(QObject* parent = nullptr)
        : ActorObserve(parent)
    {}

    void Init()
    {
        Subscribe("user/score", std::make_shared<AlwaysPolicy>());
        Subscribe("user/level", std::make_shared<AlwaysPolicy>());
        Subscribe("sensor/temperature", std::make_shared<AlwaysPolicy>());
    }

protected:
    void ObserveData(const QString& tag,
                     const QVariant& value) override
    {
        QString type = TypeKey::FromVariant(value);
        qDebug().noquote()
            << QString("[%1] [LOG] tag=%2 type=%3 value=%4")
                   .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                   .arg(tag)
                   .arg(type)
                   .arg(value.toString());
    }
};
