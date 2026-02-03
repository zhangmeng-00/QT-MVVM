#pragma once

#include "actor/ActorObserve.h"
#include "LogEntry.h"

/*
 * BaseModel
 * ============================================================
 * 所有业务 Model 的基类
 *
 * 特点：
 * ------------------------------------------------------------
 * - 已经具备 Actor（串行、线程安全）
 * - 只需要实现 ObserveData
 */
class BaseModel : public ActorObserve {
    Q_OBJECT
public:
    /*
     * BaseModel 构造函数
     * --------------------------------------------------------
     * @param parent: 父对象
     * @param useSeparateThread: 是否使用单独的线程运行
     */
    explicit BaseModel(QObject* parent = nullptr,
                      bool useSeparateThread = false);

protected:
    /*
     * log
     * --------------------------------------------------------
     * 默认日志发布：user/logging
     * 子类可重写
     */
    virtual void log(const QString& modelName,
                     LogLevel level,
                     const QString& message);

    /*
     * ObserveData
     * --------------------------------------------------------
     * 业务只处理 tag + value
     */
    void ObserveData(const QString& tag,
                     const QVariant& value) override
    {
        Q_UNUSED(tag)
        Q_UNUSED(value)
        // 子类实现
    }
};
