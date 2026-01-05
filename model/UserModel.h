#pragma once

#include "actor/ActorObserve.h"

/*
 * UserModel
 * ============================================================
 * - 订阅 score
 * - 根据 score 计算 level
 * - 发布 level
 */
class UserModel : public ActorObserve {
    Q_OBJECT
public:
    explicit UserModel(QObject* parent = nullptr)
        : ActorObserve(parent)
    {}

protected:
    void ObserveData(const QString& tag,
                     const QVariant& value) override
    {
        if (tag == "score") {
            int score = value.toInt();
            int newLevel = score / 100;

            if (newLevel != m_level) {
                m_level = newLevel;
                Publish("level", m_level);
            }
        }
    }

private:
    int m_level = 0;
};
