/****************************************************************************
** Meta object code from reading C++ file 'UserViewModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../viewmodel/UserViewModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UserViewModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UserViewModel_t {
    QByteArrayData data[16];
    char stringdata0[210];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UserViewModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UserViewModel_t qt_meta_stringdata_UserViewModel = {
    {
QT_MOC_LITERAL(0, 0, 13), // "UserViewModel"
QT_MOC_LITERAL(1, 14, 16), // "scoreTextChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 16), // "levelTextChanged"
QT_MOC_LITERAL(4, 49, 15), // "userNameChanged"
QT_MOC_LITERAL(5, 65, 21), // "loginStateTextChanged"
QT_MOC_LITERAL(6, 87, 15), // "modeTextChanged"
QT_MOC_LITERAL(7, 103, 16), // "countTextChanged"
QT_MOC_LITERAL(8, 120, 17), // "LoginStateChanged"
QT_MOC_LITERAL(9, 138, 8), // "loggedIn"
QT_MOC_LITERAL(10, 147, 9), // "scoreText"
QT_MOC_LITERAL(11, 157, 9), // "levelText"
QT_MOC_LITERAL(12, 167, 8), // "userName"
QT_MOC_LITERAL(13, 176, 14), // "loginStateText"
QT_MOC_LITERAL(14, 191, 8), // "modeText"
QT_MOC_LITERAL(15, 200, 9) // "countText"

    },
    "UserViewModel\0scoreTextChanged\0\0"
    "levelTextChanged\0userNameChanged\0"
    "loginStateTextChanged\0modeTextChanged\0"
    "countTextChanged\0LoginStateChanged\0"
    "loggedIn\0scoreText\0levelText\0userName\0"
    "loginStateText\0modeText\0countText"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UserViewModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       6,   58, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,
       4,    0,   51,    2, 0x06 /* Public */,
       5,    0,   52,    2, 0x06 /* Public */,
       6,    0,   53,    2, 0x06 /* Public */,
       7,    0,   54,    2, 0x06 /* Public */,
       8,    1,   55,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    9,

 // properties: name, type, flags
      10, QMetaType::QString, 0x00495001,
      11, QMetaType::QString, 0x00495001,
      12, QMetaType::QString, 0x00495001,
      13, QMetaType::QString, 0x00495001,
      14, QMetaType::QString, 0x00495001,
      15, QMetaType::QString, 0x00495001,

 // properties: notify_signal_id
       0,
       1,
       2,
       3,
       4,
       5,

       0        // eod
};

void UserViewModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UserViewModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->scoreTextChanged(); break;
        case 1: _t->levelTextChanged(); break;
        case 2: _t->userNameChanged(); break;
        case 3: _t->loginStateTextChanged(); break;
        case 4: _t->modeTextChanged(); break;
        case 5: _t->countTextChanged(); break;
        case 6: _t->LoginStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UserViewModel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserViewModel::scoreTextChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UserViewModel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserViewModel::levelTextChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UserViewModel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserViewModel::userNameChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (UserViewModel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserViewModel::loginStateTextChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (UserViewModel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserViewModel::modeTextChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (UserViewModel::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserViewModel::countTextChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (UserViewModel::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserViewModel::LoginStateChanged)) {
                *result = 6;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<UserViewModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->scoreText(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->levelText(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->userName(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->loginStateText(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->modeText(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->countText(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject UserViewModel::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseViewModel::staticMetaObject>(),
    qt_meta_stringdata_UserViewModel.data,
    qt_meta_data_UserViewModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UserViewModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserViewModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UserViewModel.stringdata0))
        return static_cast<void*>(this);
    return BaseViewModel::qt_metacast(_clname);
}

int UserViewModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void UserViewModel::scoreTextChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void UserViewModel::levelTextChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void UserViewModel::userNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void UserViewModel::loginStateTextChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void UserViewModel::modeTextChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void UserViewModel::countTextChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void UserViewModel::LoginStateChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
