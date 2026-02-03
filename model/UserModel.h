#pragma once

#include "actor/ActorObserve.h"
#include <QDebug>

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


// protected:
//     void ObserveData(const QString& tag,
//                      const QVariant& value) override
//     {
//         qDebug() << "[UserModel] ObserveData CALLED, tag =" << tag
//                  << "value =" << value;

//         // 暂时直接 return，不做任何逻辑
//     }
    virtual void SetupSubscriptions() override;
protected:
    void ObserveData(const QString& tag,
                     const QVariant& value) override;

private:
    int m_level = 0;
};
