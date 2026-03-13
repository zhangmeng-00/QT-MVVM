#pragma once

#include <QString>
#include <QVariant>
#include <QVariantList>
#include <functional>

/*
 * BindingEventsHelpers
 * ============================================================
 * 目的：让 BindingEvents 的调用更简洁，提供常用的 payload 映射函数
 *
 * 使用方式：
 *   using namespace BindingEventsHelpers;
 *   BindingEvents::BindEventToPublish(ui->btn, &QPushButton::clicked,
 *       vm, "tag", ConstPayload(true));
 */

namespace BindingEventsHelpers {

// ------------------------------------------------------------
// 固定 payload：不管 signal 参数是什么，都返回固定值
// ------------------------------------------------------------
inline std::function<QVariant(const QVariantList&)>
ConstPayload(const QVariant& value)
{
    return [value](const QVariantList&) -> QVariant {
        return value;
    };
}

// ------------------------------------------------------------
// 信号参数直接作为 payload（取第一个参数）
// ------------------------------------------------------------
inline std::function<QVariant(const QVariantList&)>
AsPayload()
{
    return [](const QVariantList& args) -> QVariant {
        return args.value(0);
    };
}

// ------------------------------------------------------------
// bool 转换 (toggled(bool))
// ------------------------------------------------------------
inline std::function<QVariant(const QVariantList&)>
FromBool()
{
    return [](const QVariantList& args) -> QVariant {
        if (args.isEmpty()) return QVariant(false);
        return args.value(0).toBool();
    };
}

// ------------------------------------------------------------
// int 转换 (valueChanged(int), currentIndexChanged(int))
// ------------------------------------------------------------
inline std::function<QVariant(const QVariantList&)>
FromInt()
{
    return [](const QVariantList& args) -> QVariant {
        if (args.isEmpty()) return QVariant(0);
        return args.value(0).toInt();
    };
}

// ------------------------------------------------------------
// double 转换 (valueChanged(double))
// ------------------------------------------------------------
inline std::function<QVariant(const QVariantList&)>
FromDouble()
{
    return [](const QVariantList& args) -> QVariant {
        if (args.isEmpty()) return QVariant(0.0);
        return args.value(0).toDouble();
    };
}

// ------------------------------------------------------------
// QString 转换 (textEdited(QString), textChanged(QString))
// ------------------------------------------------------------
inline std::function<QVariant(const QVariantList&)>
FromString()
{
    return [](const QVariantList& args) -> QVariant {
        if (args.isEmpty()) return QString();
        return args.value(0).toString();
    };
}

// ------------------------------------------------------------
// 自定义转换函数
// ------------------------------------------------------------
template <typename Func>
inline std::function<QVariant(const QVariantList&)>
Map(Func&& mapper)
{
    return [mapper = std::forward<Func>(mapper)](const QVariantList& args) -> QVariant {
        return mapper(args);
    };
}

} // namespace BindingEventsHelpers
