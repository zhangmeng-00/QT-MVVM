// LabelViewObserve.cpp
#include "LabelViewObserve.h"

LabelViewObserve::LabelViewObserve(QLabel* label, QObject* parent)
    : ActorObserve(parent)
    , m_label(label)
{
}

void LabelViewObserve::ObserveData(const QString& dataType,
                                   const QString& tag,
                                   const QVariant& data)
{
    Q_UNUSED(dataType)
    Q_UNUSED(tag)

    if (m_label) {
        m_label->setText(data.toString());
    }
}
