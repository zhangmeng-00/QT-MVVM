#include "AppContext.h"

#include <QMutexLocker>

AppContext& AppContext::instance()
{
    static AppContext ctx;
    return ctx;
}

AppContext::AppContext(QObject* parent)
    : QObject(parent)
{
    m_mediator = new Mediator();
    m_mediatorThread = new QThread(this);

    m_mediator->moveToThread(m_mediatorThread);
    connect(m_mediatorThread, &QThread::finished, m_mediator, &QObject::deleteLater);

    m_mediatorThread->start();
}

AppContext::~AppContext()
{
    shutdownMediatorThread();
}

Mediator* AppContext::mediator() const
{
    return m_mediator;
}

void AppContext::ConnectObserve(Observe* obs)
{
    if (!obs || !m_mediator) {
        return;
    }

    {
        QMutexLocker locker(&m_observerMutex);
        if (m_connectedObservers.contains(obs)) {
            return;
        }
        m_connectedObservers.insert(obs);
    }

    connect(obs, &QObject::destroyed, this, [this, obs]() {
        QMutexLocker locker(&m_observerMutex);
        m_connectedObservers.remove(obs);
    });

    const auto connectTask = [this, obs]() {
        if (m_mediator) {
            m_mediator->ConnectObserve(obs);
        }
    };

    bool connected = false;
    if (QThread::currentThread() == m_mediator->thread()) {
        connectTask();
        connected = true;
    } else {
        connected = QMetaObject::invokeMethod(
            m_mediator,
            connectTask,
            Qt::BlockingQueuedConnection);
    }

    if (!connected) {
        QMutexLocker locker(&m_observerMutex);
        m_connectedObservers.remove(obs);
        return;
    }

    const auto setupTask = [obs]() {
        obs->SetupSubscriptions();
    };

    if (QThread::currentThread() == obs->thread()) {
        setupTask();
    } else {
        QMetaObject::invokeMethod(obs, setupTask, Qt::QueuedConnection);
    }
}

void AppContext::shutdownMediatorThread()
{
    {
        QMutexLocker locker(&m_observerMutex);
        m_connectedObservers.clear();
    }

    if (!m_mediatorThread) {
        m_mediator = nullptr;
        return;
    }

    if (m_mediator) {
        QMetaObject::invokeMethod(m_mediator, "deleteLater", Qt::QueuedConnection);
    }

    if (m_mediatorThread->isRunning()) {
        m_mediatorThread->quit();
        if (!m_mediatorThread->wait(3000)) {
            qWarning() << "[AppContext] mediator thread quit timeout, waiting until stop";
            m_mediatorThread->wait();
        }
    }

    m_mediator = nullptr;
    m_mediatorThread = nullptr;
}
