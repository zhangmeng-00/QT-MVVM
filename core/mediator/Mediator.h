#pragma once

#include <QObject>
#include <QMap>
#include <QMutex>
#include <memory>

#include "Topic.h"
#include "Observe.h"

/*
 * Mediator
 * ============================================================
 * 负责：
 * - 管理 Topic（按 tag 创建/获取）
 * - 处理订阅：把 (observer + policy) 加入 Topic
 * - 处理发布：转发到 Topic->Notify()
 */
class Mediator : public QObject {
    Q_OBJECT
public:
    explicit Mediator(QObject* parent = nullptr);

    /*
     * ConnectObserve
     * --------------------------------------------------------
     * 让某个 Observe 接入 Mediator：
     * - 订阅请求信号
     * - 发布请求信号
     */
    void ConnectObserve(Observe* obs);

private slots:
    /*
     * OnSubscribe
     * --------------------------------------------------------
     * 处理订阅请求（来自任意线程）
     * ✅ policy 直接由信号传入，杜绝异步串台
     */
    void OnSubscribe(Observe* observer,
                     const QString& tag,
                     PolicyPtr policy);

    void OnUnsubscribe(Observe* obs, const QString& tag);

    /*
     * OnPublish
     * --------------------------------------------------------
     * 处理发布请求
     */
    void OnPublish(const QString& tag,
                   const QVariant& value);

private:
    /*
     * getOrCreateTopic
     * --------------------------------------------------------
     * 获取或创建 Topic
     */
    std::shared_ptr<Topic> getOrCreateTopic(const QString& tag);

private:
    QMutex m_mutex; // 保护 topics
    QMap<QString, std::shared_ptr<Topic>> m_topics; // tag -> Topic
};
