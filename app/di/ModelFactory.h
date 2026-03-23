#pragma once

#include "IModelFactory.h"
#include <QMap>
#include <QString>

/*
 * ModelFactory - 模型工厂默认实现
 * ============================================================
 * 通过名字创建模型实例
 *
 * 使用方式：
 *   ModelFactory factory;
 *   factory.registerCreator("UserModel", [](){ return std::make_shared<UserModel>(); });
 *   factory.registerCreator("SensorModel", [](){ return std::make_shared<SensorModel>(); });
 */
class ModelFactory : public IModelFactory {
public:
    ModelFactory() = default;

    // 注册创建函数
    void registerCreator(const QString& modelName,
                        std::function<IModelPtr()> creator) override {
        m_creators[modelName] = creator;
    }

    // 通过模型名称创建
    IModelPtr create(const QString& modelName) override {
        auto it = m_creators.find(modelName);
        if (it != m_creators.end()) {
            return it.value()();
        }
        return nullptr;
    }

    // 检查是否已注册
    bool isRegistered(const QString& modelName) const override {
        return m_creators.contains(modelName);
    }

private:
    QMap<QString, std::function<IModelPtr()>> m_creators;
};
