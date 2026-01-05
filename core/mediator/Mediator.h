#pragma once
#include <QObject>
#include <map>
#include <memory>

#include "Topic.h"
#include "Observe.h"
#include "common/TypeKey.h"

/*
 * Mediator
 * ============================================================
 * 中介者 / 数据总线核心
 *
 * 内部规则：
 * ------------------------------------------------------------
 * - Topic Key = typeKey:tag
 * - typeKey 从 QVariant 推导
 * - 支持 any:tag（无类型订阅）
 */
class Mediator : public QObject {
    Q_OBJECT
public:
    explicit Mediator(QObject* parent = nullptr);

    void ConnectObserve(Observe* obs);

private slots:
    void OnSubscribeRequest(QObject* owner,
                            ITransport* transport,
                            const QString& tag);

    void OnPublishRequest(const QString& tag,
                          const QVariant& value);

    void OnObserveDestroyed(QObject* obj);

private:
    std::shared_ptr<Topic> GetOrCreateTopic(const QString& typeKey,
                                            const QString& tag);

private:
    std::map<QString, std::shared_ptr<Topic>> m_topics;
};
