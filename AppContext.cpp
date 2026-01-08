#include "AppContext.h"
#include <QDebug>

AppContext& AppContext::instance()
{
    static AppContext ctx;
    return ctx;
}

AppContext::AppContext(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[AppContext] init";

    m_mediator = new Mediator;
    m_mediatorThread = new QThread(this);

    m_mediator->moveToThread(m_mediatorThread);

    // 确保线程结束时安全销毁
    connect(m_mediatorThread, &QThread::finished,
            m_mediator, &QObject::deleteLater);

    m_mediatorThread->start();
}

AppContext::~AppContext()
{
    qDebug() << "[AppContext] shutdown";

    if (m_mediatorThread) {
        m_mediatorThread->quit();
        m_mediatorThread->wait();
    }
}

Mediator* AppContext::mediator() const
{
    return m_mediator;
}

void AppContext::ConnectObserve(Observe* obs)
{
    if (!obs || !m_mediator) return;
    m_mediator->ConnectObserve(obs);
}
