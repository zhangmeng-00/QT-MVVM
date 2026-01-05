#pragma once
#include <QVariant>
#include <QMetaType>
#include <QString>

/*
 * TypeKey
 * ============================================================
 * 框架内部的“类型裁判”
 *
 * 职责：
 * ------------------------------------------------------------
 * - 把 QVariant 映射为有限、稳定的类型 key
 * - 支持 Qt 内建类型 + 自定义类型
 *
 * 规则：
 * ------------------------------------------------------------
 * - int        → "int"
 * - double     → "double"
 * - QString    → "string"
 * - bool       → "bool"
 * - 自定义类型 → "custom:TypeName"
 *
 * 最终 Topic Key：
 * ------------------------------------------------------------
 *   <typeKey>:<tag>
 *   例如：
 *     int:temperature
 *     custom:SensorData:sensor
 */
struct TypeKey
{
    static QString FromVariant(const QVariant& v)
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        int id = v.typeId();
#else
        int id = v.type();
#endif
        switch (id) {
        case QMetaType::Int:       return "int";
        case QMetaType::Double:    return "double";
        case QMetaType::QString:   return "string";
        case QMetaType::Bool:      return "bool";
        case QMetaType::LongLong:  return "int64";
        case QMetaType::ULongLong: return "uint64";
        case QMetaType::Float:     return "float";
        default:
            // 自定义类型，要求已注册到 Qt MetaType
            return QString("custom:%1")
                .arg(QString::fromLatin1(v.typeName()));
        }
    }
};
