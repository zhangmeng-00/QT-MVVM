#pragma once

#include <QObject>
#include <QVariant>
#include <memory>
#include <QString>

#include "../policy/ISubscriptionPolicy.h"

/*
 * Observe
 * ============================================================
 * 所有 Model / ViewModel 的基类
 *
 * 职责：
 * - 向 Mediator 订阅 Topic（tag + policy）
 * - 向 Mediator 发布数据（tag + value）
 * - 接收 Mediator 分发的数据（tag + value）
 */
class Observe : public QObject {
    Q_OBJECT
public:
    explicit Observe(QObject* parent = nullptr);
    virtual ~Observe();

    /*
     * Subscribe
     * --------------------------------------------------------
     * 向 Mediator 请求订阅：
     * - tag：例如 "user/score"
     * - policy：订阅策略（Always / ValueChanged 等）
     *
     * ✅ 关键点：policy 直接作为信号参数发送，避免异步串台
     */
    void Subscribe(const QString& tag, PolicyPtr policy);

    /*
     * Publish
     * --------------------------------------------------------
     * 向 Mediator 发布数据（tag + value）
     */
    void Publish(const QString& tag, const QVariant& value);

signals:
    /*
     * RequestSubscribe
     * --------------------------------------------------------
     * 统一订阅信号（带 policy）
     * 使用 QueuedConnection 时，policy 仍然正确对应本次订阅
     */
    void RequestSubscribe(Observe* observer,
                          const QString& tag,
                          PolicyPtr policy);

    /*
     * RequestPublish
     * --------------------------------------------------------
     * 统一发布信号
     */
    void RequestPublish(const QString& tag,
                        const QVariant& value);

public slots:
    /*
     * OnDataReceived
     * --------------------------------------------------------
     * Mediator/Topic -> Observe 的回调入口
     * 必须是 virtual，供 ActorObserve override
     */
    virtual void OnDataReceived(const QString& tag,
                                const QVariant& value);

protected:
    /*
     * ObserveData
     * --------------------------------------------------------
     * 真正业务处理入口（子类实现）
     */
    virtual void ObserveData(const QString& tag,
                             const QVariant& value) = 0;
};
