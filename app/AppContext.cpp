#include "AppContext.h"

#include "core/mediator/Mediator.h"
#include "core/mediator/Observe.h"


AppContext& AppContext::instance()
{
    static AppContext ctx;
    return ctx;
}

AppContext::AppContext(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[AppContext] init";

    // 1) Mediator + 线程
    m_mediator = new Mediator;
    m_mediatorThread = new QThread(this);

    m_mediator->moveToThread(m_mediatorThread);

    // 确保线程结束时安全销毁 mediator（在其所在线程）
    connect(m_mediatorThread, &QThread::finished,
            m_mediator, &QObject::deleteLater);

    m_mediatorThread->start();

    // 2) 模块系统（可选）
    // m_moduleRegistry.reset(new ModuleRegistry());
}

AppContext::~AppContext()
{
    qDebug() << "[AppContext] shutdown";

    // 可选：停止模块（如果你启用了模块系统）
    // StopModules();

    shutdownMediatorThread();
}

Mediator* AppContext::mediator() const
{
    return m_mediator;
}

void AppContext::ConnectObserve(Observe* obs)
{
    if (!obs || !m_mediator) return;

    // ⚠️ 关键：不要跨线程直接调用 mediator
    // 统一切到 mediator 线程执行
    InvokeOnMediator([this, obs]() {
        m_mediator->ConnectObserve(obs);
        // ✅ 关键：连接后订阅
        obs->SetupSubscriptions();
    });
}

void AppContext::shutdownMediatorThread()
{
    if (!m_mediatorThread) return;

    // 退出线程（mediator deleteLater 会在 finished 后触发）
    m_mediatorThread->quit();
    m_mediatorThread->wait();
}
