#include "BaseModel.h"

#include <QDateTime>

BaseModel::BaseModel(QObject* parent, bool useSeparateThread)
    : ActorObserve(parent, useSeparateThread)
{
}