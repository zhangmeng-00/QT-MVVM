#pragma once

#include <QObject>
#include <QThread>
#include <memory>

#include "core/mediator/Mediator.h"

class AppContext : public QObject {
    Q_OBJECT
public:
    static AppContext& instance();

    Mediator* mediator() const;

    // 给外部统一接入点
    void ConnectObserve(Observe* obs);

private:
    explicit AppContext(QObject* parent = nullptr);
    ~AppContext();

    Q_DISABLE_COPY(AppContext)

private:
    Mediator* m_mediator{nullptr};
    QThread*  m_mediatorThread{nullptr};
};
