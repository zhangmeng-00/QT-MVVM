/****************************************************************************
** Meta object code from reading C++ file 'SensorViewModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../viewmodel/SensorViewModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SensorViewModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SensorViewModel_t {
    QByteArrayData data[14];
    char stringdata0[198];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SensorViewModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SensorViewModel_t qt_meta_stringdata_SensorViewModel = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SensorViewModel"
QT_MOC_LITERAL(1, 16, 22), // "temperatureTextChanged"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 21), // "targetTempTextChanged"
QT_MOC_LITERAL(4, 62, 15), // "gainTextChanged"
QT_MOC_LITERAL(5, 78, 18), // "SetupSubscriptions"
QT_MOC_LITERAL(6, 97, 9), // "onClicked"
QT_MOC_LITERAL(7, 107, 8), // "senderId"
QT_MOC_LITERAL(8, 116, 14), // "onValueChanged"
QT_MOC_LITERAL(9, 131, 5), // "value"
QT_MOC_LITERAL(10, 137, 20), // "onValueChangedDouble"
QT_MOC_LITERAL(11, 158, 15), // "temperatureText"
QT_MOC_LITERAL(12, 174, 14), // "targetTempText"
QT_MOC_LITERAL(13, 189, 8) // "gainText"

    },
    "SensorViewModel\0temperatureTextChanged\0"
    "\0targetTempTextChanged\0gainTextChanged\0"
    "SetupSubscriptions\0onClicked\0senderId\0"
    "onValueChanged\0value\0onValueChangedDouble\0"
    "temperatureText\0targetTempText\0gainText"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SensorViewModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       3,   66, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,
       4,    0,   51,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   52,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
       6,    1,   53,    2, 0x02 /* Public */,
       8,    2,   56,    2, 0x02 /* Public */,
      10,    2,   61,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    9,    7,
    QMetaType::Void, QMetaType::Double, QMetaType::QString,    9,    7,

 // properties: name, type, flags
      11, QMetaType::QString, 0x00495001,
      12, QMetaType::QString, 0x00495001,
      13, QMetaType::QString, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,

       0        // eod
};

void SensorViewModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SensorViewModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->temperatureTextChanged(); break;
        case 1: _t->targetTempTextChanged(); break;
        case 2: _t->gainTextChanged(); break;
        case 3: _t->SetupSubscriptions(); break;
        case 4: _t->onClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->onValueChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->onValueChangedDouble((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SensorViewModel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SensorViewModel::temperatureTextChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SensorViewModel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SensorViewModel::targetTempTextChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SensorViewModel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SensorViewModel::gainTextChanged)) {
                *result = 2;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<SensorViewModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->temperatureText(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->targetTempText(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->gainText(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject SensorViewModel::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseViewModel::staticMetaObject>(),
    qt_meta_stringdata_SensorViewModel.data,
    qt_meta_data_SensorViewModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SensorViewModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SensorViewModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SensorViewModel.stringdata0))
        return static_cast<void*>(this);
    return BaseViewModel::qt_metacast(_clname);
}

int SensorViewModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseViewModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void SensorViewModel::temperatureTextChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SensorViewModel::targetTempTextChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SensorViewModel::gainTextChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
