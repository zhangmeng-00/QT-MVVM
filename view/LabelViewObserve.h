#pragma once

#include <QLabel>
#include "actor/ActorObserve.h"

/*
 * LabelViewObserve
 * ============================================================
 * 将某个 tag 的数据展示到 QLabel
 */
class LabelViewObserve : public ActorObserve {
    Q_OBJECT
public:
    explicit LabelViewObserve(QLabel* label,
                              QObject* parent = nullptr)
        : ActorObserve(parent)
        , m_label(label)
    {}

protected:
    void ObserveData(const QString&,
                     const QVariant& value) override
    {
        if (m_label) {
            m_label->setText(value.toString());
        }
    }

private:
    QLabel* m_label = nullptr;
};
