#pragma once
#include <QString>
#include <QVariant>

/*
 * ITransport
 * ============================================================
 * Topic → Observe 的数据传输接口
 *
 * 注意：
 * - typeKey 是框架内部使用
 * - 业务层通常不关心 typeKey
 */
class ITransport {
public:
    virtual ~ITransport() = default;

    virtual void OnDataReceived(const QString& typeKey,
                                const QString& tag,
                                const QVariant& value) = 0;
};
