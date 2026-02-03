#pragma once

#include "actor/ActorObserve.h"

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
    
    ~BaseModel() override;

private:
    // 单独线程（如果使用）
    QThread* m_thread = nullptr;

protected:
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
