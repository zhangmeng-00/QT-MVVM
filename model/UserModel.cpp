#include "UserModel.h"
#include "AlwaysPolicy.h"
#include "Logger.h"
#include "Tags.h"
#include <memory>
#include <QVariant>

void UserModel::SetupSubscriptions()
{
  // 使用带QVariant的订阅，明确指定数据类型
  Subscribe(TAG_USER_SCORE, QVariant(0), std::make_shared<AlwaysPolicy>());
}

void UserModel::ObserveData(const QString &tag, const QVariant &value)
{

    if (tag == TAG_USER_SCORE) {
        int score = value.toInt();
        int newLevel = score / 100;

        if (newLevel != m_level) {
            m_level = newLevel;
            Publish(TAG_USER_LEVEL, QVariant::fromValue(m_level));
            LOG_WARN("UserModel", QString("User level updated to %1").arg(m_level));
        }
    }
}
