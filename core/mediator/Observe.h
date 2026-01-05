// Observe.h
#pragma once

#include <QObject>
#include <QVariant>
#include <memory>

#include "../transport/ITransport.h"
#include "../policy/ISubscriptionPolicy.h"

/*
 * Observe
 * ============================================================
 * 抽象观察者：提供订阅/发布接口
 *
 * 关键点：
 * 1) policy 不通过 queued signal 传 shared_ptr（避免 Qt 元类型注册问题）
 * 2) policy 暂存在 Observe 中，由 Mediator 取走
 * 3) Observe 析构不 emit，避免退出阶段崩溃
 */
class Observe : public QObject, public ITransport {
    Q_OBJECT
public:
    explicit Observe(QObject* parent = nullptr);
    virtual ~Observe();

    void SubscribeRequest(const QString& dataType,
                          const QString& tag,
                          std::shared_ptr<ISubscriptionPolicy> policy);

    void PublishRequest(const QString& dataType,
                        const QString& tag,
                        const QVariant& data);

    std::shared_ptr<ISubscriptionPolicy> TakePendingPolicy();

    // 可覆盖：ActorObserve 会覆盖该函数，把数据投递到 Mailbox
    void OnDataReceived(const QString& dataType,
                        const QString& tag,
                        const QVariant& data) override;

signals:
    void RequestSubscribe(QObject* owner,
                          ITransport* transport,
                          const QString& dataType,
                          const QString& tag);

    void RequestPublish(const QString& dataType,
                        const QString& tag,
                        const QVariant& data);

protected:
    // 子类实现真正处理逻辑
    virtual void ObserveData(const QString& dataType,
                             const QString& tag,
                             const QVariant& data) = 0;

private:
    std::shared_ptr<ISubscriptionPolicy> m_pendingPolicy;
};
