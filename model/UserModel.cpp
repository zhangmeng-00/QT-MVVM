// UserModel.cpp
#include "UserModel.h"

UserModel::UserModel(QObject* parent)
    : ActorObserve(parent)
{
}

void UserModel::ObserveData(const QString& dataType,
                            const QString& tag,
                            const QVariant& data)
{
    if (dataType == "user" && tag == "score") {
        int score = data.toInt();
        int level = score / 100;

        if (level != m_level) {
            m_level = level;
            PublishRequest("user", "level", m_level);
        }
    }
}
