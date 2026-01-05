// LabelViewObserve.h
#pragma once

#include <QLabel>
#include "core/actor/ActorObserve.h"

/*
 * LabelViewObserve（UI Actor）
 * ============================================================
 * View 在 UI 线程，不 moveToThread
 * 但仍使用 ActorObserve 的 Mailbox，保证更新串行、可控
 */
class LabelViewObserve : public ActorObserve {
    Q_OBJECT
public:
    explicit LabelViewObserve(QLabel* label, QObject* parent = nullptr);

protected:
    void ObserveData(const QString& dataType,
                     const QString& tag,
                     const QVariant& data) override;

private:
    QLabel* m_label = nullptr;
};
