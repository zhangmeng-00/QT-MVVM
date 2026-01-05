// Mediator.h
#pragma once

#include <QObject>
#include <map>
#include <memory>

#include "Topic.h"
#include "Observe.h"

/*
 * Mediator
 * ============================================================
 * 中介者 / 总线（对标 MVA 的 mediated data bus）
 *
 * 线程：
 * - Mediator 通常 moveToThread(mediatorThread)
 * - Subscribe/Publish 都用 queued 进入 mediator 线程
 *
 * 生命周期：
 * - 监听 Observe::destroyed 自动清理订阅
 */
class Mediator : public QObject {
    Q_OBJECT
public:
    explicit Mediator(QObject* parent = nullptr);

    void ConnectObserve(Observe* obs);

private slots:
    void OnSubscribeRequest(QObject* owner,
                            ITransport* transport,
                            const QString& dataType,
                            const QString& tag);

    void OnPublishRequest(const QString& dataType,
                          const QString& tag,
                          const QVariant& data);

    void OnObserveDestroyed(QObject* obj);

private:
    std::shared_ptr<Topic> GetOrCreateTopic(const QString& dataType,
                                            const QString& tag);

private:
    std::map<QString, std::shared_ptr<Topic>> m_topics; // key="type:tag"
};
