/****************************************************************************
** Meta object code from reading C++ file 'Observe.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "C:/Users/89233/Downloads/MediatorMVAQT_with_pidtest/MediatorMVAQT/core/mediator/Observe.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Observe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Observe_t {
    QByteArrayData data[13];
    char stringdata0[125];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Observe_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Observe_t qt_meta_stringdata_Observe = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Observe"
QT_MOC_LITERAL(1, 8, 16), // "RequestSubscribe"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 8), // "Observe*"
QT_MOC_LITERAL(4, 35, 8), // "observer"
QT_MOC_LITERAL(5, 44, 3), // "tag"
QT_MOC_LITERAL(6, 48, 9), // "PolicyPtr"
QT_MOC_LITERAL(7, 58, 6), // "policy"
QT_MOC_LITERAL(8, 65, 18), // "RequestUnsubscribe"
QT_MOC_LITERAL(9, 84, 4), // "self"
QT_MOC_LITERAL(10, 89, 14), // "RequestPublish"
QT_MOC_LITERAL(11, 104, 5), // "value"
QT_MOC_LITERAL(12, 110, 14) // "OnDataReceived"

    },
    "Observe\0RequestSubscribe\0\0Observe*\0"
    "observer\0tag\0PolicyPtr\0policy\0"
    "RequestUnsubscribe\0self\0RequestPublish\0"
    "value\0OnDataReceived"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Observe[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   34,    2, 0x06 /* Public */,
       8,    2,   41,    2, 0x06 /* Public */,
      10,    2,   46,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    2,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, 0x80000000 | 6,    4,    5,    7,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    9,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    5,   11,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,    5,   11,

       0        // eod
};

void Observe::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Observe *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->RequestSubscribe((*reinterpret_cast< Observe*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< PolicyPtr(*)>(_a[3]))); break;
        case 1: _t->RequestUnsubscribe((*reinterpret_cast< Observe*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->RequestPublish((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 3: _t->OnDataReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
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
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Observe::*)(Observe * , const QString & , PolicyPtr );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Observe::RequestSubscribe)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Observe::*)(Observe * , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Observe::RequestUnsubscribe)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Observe::*)(const QString & , const QVariant & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Observe::RequestPublish)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Observe::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Observe.data,
    qt_meta_data_Observe,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Observe::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Observe::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Observe.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Observe::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Observe::RequestSubscribe(Observe * _t1, const QString & _t2, PolicyPtr _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Observe::RequestUnsubscribe(Observe * _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Observe::RequestPublish(const QString & _t1, const QVariant & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
