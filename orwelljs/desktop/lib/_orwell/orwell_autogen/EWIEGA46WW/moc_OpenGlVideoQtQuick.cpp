/****************************************************************************
** Meta object code from reading C++ file 'OpenGlVideoQtQuick.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../OpenGlVideoQtQuick.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OpenGlVideoQtQuick.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OpenGlVideoQtQuickRenderer_t {
    QByteArrayData data[3];
    char stringdata0[35];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OpenGlVideoQtQuickRenderer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OpenGlVideoQtQuickRenderer_t qt_meta_stringdata_OpenGlVideoQtQuickRenderer = {
    {
QT_MOC_LITERAL(0, 0, 26), // "OpenGlVideoQtQuickRenderer"
QT_MOC_LITERAL(1, 27, 6), // "render"
QT_MOC_LITERAL(2, 34, 0) // ""

    },
    "OpenGlVideoQtQuickRenderer\0render\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OpenGlVideoQtQuickRenderer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void OpenGlVideoQtQuickRenderer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OpenGlVideoQtQuickRenderer *_t = static_cast<OpenGlVideoQtQuickRenderer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->render(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject OpenGlVideoQtQuickRenderer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_OpenGlVideoQtQuickRenderer.data,
      qt_meta_data_OpenGlVideoQtQuickRenderer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *OpenGlVideoQtQuickRenderer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OpenGlVideoQtQuickRenderer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OpenGlVideoQtQuickRenderer.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QOpenGLFunctions"))
        return static_cast< QOpenGLFunctions*>(this);
    if (!strcmp(_clname, "FrameUpdater"))
        return static_cast< FrameUpdater*>(this);
    return QObject::qt_metacast(_clname);
}

int OpenGlVideoQtQuickRenderer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_OpenGlVideoQtQuick_t {
    QByteArrayData data[11];
    char stringdata0[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OpenGlVideoQtQuick_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OpenGlVideoQtQuick_t qt_meta_stringdata_OpenGlVideoQtQuick = {
    {
QT_MOC_LITERAL(0, 0, 18), // "OpenGlVideoQtQuick"
QT_MOC_LITERAL(1, 19, 8), // "tChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 4), // "sync"
QT_MOC_LITERAL(4, 34, 7), // "cleanup"
QT_MOC_LITERAL(5, 42, 6), // "update"
QT_MOC_LITERAL(6, 49, 19), // "handleWindowChanged"
QT_MOC_LITERAL(7, 69, 13), // "QQuickWindow*"
QT_MOC_LITERAL(8, 83, 3), // "win"
QT_MOC_LITERAL(9, 87, 1), // "t"
QT_MOC_LITERAL(10, 89, 3) // "uri"

    },
    "OpenGlVideoQtQuick\0tChanged\0\0sync\0"
    "cleanup\0update\0handleWindowChanged\0"
    "QQuickWindow*\0win\0t\0uri"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OpenGlVideoQtQuick[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       2,   46, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   40,    2, 0x0a /* Public */,
       4,    0,   41,    2, 0x0a /* Public */,
       5,    0,   42,    2, 0x0a /* Public */,
       6,    1,   43,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,

 // properties: name, type, flags
       9, QMetaType::QReal, 0x00495103,
      10, QMetaType::QString, 0x00095102,

 // properties: notify_signal_id
       0,
       0,

       0        // eod
};

void OpenGlVideoQtQuick::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OpenGlVideoQtQuick *_t = static_cast<OpenGlVideoQtQuick *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->tChanged(); break;
        case 1: _t->sync(); break;
        case 2: _t->cleanup(); break;
        case 3: _t->update(); break;
        case 4: _t->handleWindowChanged((*reinterpret_cast< QQuickWindow*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QQuickWindow* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OpenGlVideoQtQuick::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OpenGlVideoQtQuick::tChanged)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        OpenGlVideoQtQuick *_t = static_cast<OpenGlVideoQtQuick *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< qreal*>(_v) = _t->t(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        OpenGlVideoQtQuick *_t = static_cast<OpenGlVideoQtQuick *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setT(*reinterpret_cast< qreal*>(_v)); break;
        case 1: _t->setUri(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject OpenGlVideoQtQuick::staticMetaObject = {
    { &QQuickItem::staticMetaObject, qt_meta_stringdata_OpenGlVideoQtQuick.data,
      qt_meta_data_OpenGlVideoQtQuick,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *OpenGlVideoQtQuick::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OpenGlVideoQtQuick::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OpenGlVideoQtQuick.stringdata0))
        return static_cast<void*>(this);
    return QQuickItem::qt_metacast(_clname);
}

int OpenGlVideoQtQuick::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void OpenGlVideoQtQuick::tChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
