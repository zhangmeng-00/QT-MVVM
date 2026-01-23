// core/binding/BindingCommandHelpers.h
#pragma once

#include <QString>
#include <QVariant>
#include <QVariantList>

#include <functional>
#include <type_traits>

/*
 * BindingCommandHelpers
 * ============================================================
 * 目的：让 MainWindow 里不再写一堆 lambda，统一生成：
 * - argMapper:        signalArgs -> finalArgs
 * - canExecArgsGetter:() -> args   （用于 CanExecuteArgs / enabled 刷新）
 *
 * 约定：最终传给 ICommand 的 args 统一为：
 *   args[0] = tag (QString)
 *   args[1] = payload (QVariant)
 *
 * 注意：
 * - CanArgsDynamic 要求 getter 必须返回值（不能是 void）
 *   如果你忘写 return，会在编译期给出清晰的 static_assert 提示。
 */

namespace BindingCommandHelpers {

// ------------------------------------------------------------
// argMapper：signalArgs -> [tag, signalArgs[0]]
// 适用：toggled(bool)、valueChanged(int/double)、textEdited(QString) 等
// ------------------------------------------------------------
inline std::function<QVariantList(const QVariantList&)>
ToTagPayload(const QString& tag)
{
    return [tag](const QVariantList& sArgs) -> QVariantList {
        return QVariantList{ tag, sArgs.value(0) };
    };
}

// ------------------------------------------------------------
// argMapper：signalArgs -> [tag, payloadConst]
// 适用：clicked(bool) 等你不关心 signal 参数的场景
// ------------------------------------------------------------
inline std::function<QVariantList(const QVariantList&)>
ToTagConst(const QString& tag, const QVariant& payload)
{
    return [tag, payload](const QVariantList&) -> QVariantList {
        return QVariantList{ tag, payload };
    };
}

// ------------------------------------------------------------
// canExecArgsGetter：返回固定 [tag, payloadConst]
// 适用：login/logout/publish_score 这类按钮
// ------------------------------------------------------------
inline std::function<QVariantList()>
CanArgsConst(const QString& tag, const QVariant& payload)
{
    return [tag, payload]() -> QVariantList {
        return QVariantList{ tag, payload };
    };
}

// ------------------------------------------------------------
// canExecArgsGetter：动态从 UI 取值生成 [tag, payload]
// 适用：checkbox/combobox/spinbox/lineEdit 等
//
// ✅ getter 必须返回值（不能是 void）
//    典型错误： [this](){ ui->spinBox->value(); }  // 忘写 return -> void
//    正确写法： [this]()->int { return ui->spinBox->value(); }
// ------------------------------------------------------------
template <typename Getter>
inline std::function<QVariantList()>
CanArgsDynamic(const QString& tag, Getter getter)
{
    using Ret = std::decay_t<decltype(getter())>;

    static_assert(!std::is_void_v<Ret>,
                  "CanArgsDynamic: getter must RETURN a value. "
                  "Did you forget 'return' in the lambda?");

    return [tag, getter]() -> QVariantList {
        Ret v = getter();                 // 按值接住，避免引用/临时类型坑
        return QVariantList{ tag, QVariant(v) }; // 基础类型/QString 最稳的构造方式
    };
}

} // namespace BindingCommandHelpers
