/****************************************************************************
** Meta object code from reading C++ file 'Mediator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "C:/Users/89233/Downloads/MediatorMVAQT_with_pidtest/MediatorMVAQT/core/mediator/Mediator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Mediator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Mediator_t {
    QByteArrayData data[12];
    char stringdata0[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Mediator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Mediator_t qt_meta_stringdata_Mediator = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Mediator"
QT_MOC_LITERAL(1, 9, 11), // "OnSubscribe"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 8), // "Observe*"
QT_MOC_LITERAL(4, 31, 8), // "observer"
QT_MOC_LITERAL(5, 40, 3), // "tag"
QT_MOC_LITERAL(6, 44, 9), // "PolicyPtr"
QT_MOC_LITERAL(7, 54, 6), // "policy"
QT_MOC_LITERAL(8, 61, 13), // "OnUnsubscribe"
QT_MOC_LITERAL(9, 75, 3), // "obs"
QT_MOC_LITERAL(10, 79, 9), // "OnPublish"
QT_MOC_LITERAL(11, 89, 5) // "value"

    },
    "Mediator\0OnSubscribe\0\0Observe*\0observer\0"
    "tag\0PolicyPtr\0policy\0OnUnsubscribe\0"
    "obs\0OnPublish\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Mediator[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   29,    2, 0x08 /* Private */,
       8,    2,   36,    2, 0x08 /* Private */,
      10,    2,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, 0x80000000 | 6,    4,    5,    7,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    9,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    5,   11,

       0        // eod
};

void Mediator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Mediator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnSubscribe((*reinterpret_cast< Observe*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< PolicyPtr(*)>(_a[3]))); break;
        case 1: _t->OnUnsubscribe((*reinterpret_cast< Observe*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->OnPublish((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Observe* >(); break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PolicyPtr >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Observe* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Mediator::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Mediator.data,
    qt_meta_data_Mediator,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Mediator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Mediator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Mediator.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Mediator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
