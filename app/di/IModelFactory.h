#pragma once

#include "core/model/IModel.h"
#include <QString>
#include <functional>
#include <memory>

/*
 * IModelFactory - 通用模型工厂
 * ============================================================
 * 通过名字创建模型实例
 *
 * 使用方式：
 *   class ModelFactory : public IModelFactory { ... };
 *   factory->registerCreator("UserModel", [](){ return std::make_shared<UserModel>(); });
 *   auto model = factory->create("UserModel");
 */
class IModelFactory {
public:
    virtual ~IModelFactory() = default;

    // 通过模型名称创建
    virtual IModelPtr create(const QString& modelName) = 0;

    // 注册创建函数
    virtual void registerCreator(const QString& modelName,
                                std::function<IModelPtr()> creator) = 0;

    // 检查是否已注册
    virtual bool isRegistered(const QString& modelName) const = 0;

    // 常用便捷方法：按名字获取并转换为具体类型
    template<typename T>
    T* get(const QString& modelName) {
        IModelPtr model = create(modelName);
        return dynamic_cast<T*>(model.get());
    }
};

// 智能指针类型别名
using IModelFactoryPtr = std::shared_ptr<IModelFactory>;
