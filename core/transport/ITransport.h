#pragma once
#include <QString>
#include <QVariant>

/*
 * ITransport
 * ============================================================
 * Topic -> Observe 的数据传输接口
 * - dataType/tag 让订阅者知道数据来源
 */
class ITransport {
public:
    virtual ~ITransport() = default;

    virtual void OnDataReceived(const QString& dataType,
                                const QString& tag,
                                const QVariant& data) = 0;
};
