#pragma once
#include <QObject>
#include <QVariant>
#include <memory>

#include "../transport/ITransport.h"
#include "../policy/ISubscriptionPolicy.h"

/*
 * Observe
 * ============================================================
 * 所有 Model / View 的基类
 *
 * 对外 API：
 * ------------------------------------------------------------
 * - Subscribe(tag, policy)
 * - Publish(tag, QVariant)
 */
class Observe : public QObject, public ITransport {
    Q_OBJECT
public:
    explicit Observe(QObject* parent = nullptr);
    virtual ~Observe();

    // 订阅 tag（不关心类型）
    void Subscribe(const QString& tag,
                   std::shared_ptr<ISubscriptionPolicy> policy);

    // 发布 tag + value
    void Publish(const QString& tag,
                 const QVariant& value);

    // Mediator 在订阅时取走策略
    std::shared_ptr<ISubscriptionPolicy> TakePendingPolicy();

    // Topic → Observe 入口
    void OnDataReceived(const QString& typeKey,
                        const QString& tag,
                        const QVariant& value) override;

signals:
    void RequestSubscribe(QObject* owner,
                          ITransport* transport,
                          const QString& tag);

    void RequestPublish(const QString& tag,
                        const QVariant& value);

protected:
    // 业务真正实现的处理函数
    virtual void ObserveData(const QString& tag,
                             const QVariant& value) = 0;

private:
    std::shared_ptr<ISubscriptionPolicy> m_pendingPolicy;
};
