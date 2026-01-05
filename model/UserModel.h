// UserModel.h
#pragma once

#include "core/actor/ActorObserve.h"

/*
 * UserModel（Actor）
 * ============================================================
 * 订阅 user:score
 * - 计算 level = score/100
 * - level 变化时发布 user:level
 */
class UserModel : public ActorObserve {
    Q_OBJECT
public:
    explicit UserModel(QObject* parent = nullptr);

protected:
    void ObserveData(const QString& dataType,
                     const QString& tag,
                     const QVariant& data) override;

private:
    int m_level = 0;
};
