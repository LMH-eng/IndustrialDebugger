/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.11)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "E:/IndustrialDebugger/src/MainWindow/MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.11. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[19];
    char stringdata0[320];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 23), // "onNetworkConnectClicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 26), // "onNetworkDisconnectClicked"
QT_MOC_LITERAL(4, 63, 21), // "onNetworkDataReceived"
QT_MOC_LITERAL(5, 85, 4), // "data"
QT_MOC_LITERAL(6, 90, 14), // "onNetworkError"
QT_MOC_LITERAL(7, 105, 5), // "error"
QT_MOC_LITERAL(8, 111, 23), // "onSerialPortOpenClicked"
QT_MOC_LITERAL(9, 135, 24), // "onSerialPortCloseClicked"
QT_MOC_LITERAL(10, 160, 24), // "onSerialPortDataReceived"
QT_MOC_LITERAL(11, 185, 17), // "onSerialPortError"
QT_MOC_LITERAL(12, 203, 20), // "onRefreshSerialPorts"
QT_MOC_LITERAL(13, 224, 17), // "onSendDataClicked"
QT_MOC_LITERAL(14, 242, 17), // "onClearLogClicked"
QT_MOC_LITERAL(15, 260, 18), // "onExportLogClicked"
QT_MOC_LITERAL(16, 279, 18), // "onImportLogClicked"
QT_MOC_LITERAL(17, 298, 15), // "onFormatChanged"
QT_MOC_LITERAL(18, 314, 5) // "index"

    },
    "MainWindow\0onNetworkConnectClicked\0\0"
    "onNetworkDisconnectClicked\0"
    "onNetworkDataReceived\0data\0onNetworkError\0"
    "error\0onSerialPortOpenClicked\0"
    "onSerialPortCloseClicked\0"
    "onSerialPortDataReceived\0onSerialPortError\0"
    "onRefreshSerialPorts\0onSendDataClicked\0"
    "onClearLogClicked\0onExportLogClicked\0"
    "onImportLogClicked\0onFormatChanged\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    1,   86,    2, 0x08 /* Private */,
       6,    1,   89,    2, 0x08 /* Private */,
       8,    0,   92,    2, 0x08 /* Private */,
       9,    0,   93,    2, 0x08 /* Private */,
      10,    1,   94,    2, 0x08 /* Private */,
      11,    1,   97,    2, 0x08 /* Private */,
      12,    0,  100,    2, 0x08 /* Private */,
      13,    0,  101,    2, 0x08 /* Private */,
      14,    0,  102,    2, 0x08 /* Private */,
      15,    0,  103,    2, 0x08 /* Private */,
      16,    0,  104,    2, 0x08 /* Private */,
      17,    1,  105,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onNetworkConnectClicked(); break;
        case 1: _t->onNetworkDisconnectClicked(); break;
        case 2: _t->onNetworkDataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->onNetworkError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->onSerialPortOpenClicked(); break;
        case 5: _t->onSerialPortCloseClicked(); break;
        case 6: _t->onSerialPortDataReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 7: _t->onSerialPortError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->onRefreshSerialPorts(); break;
        case 9: _t->onSendDataClicked(); break;
        case 10: _t->onClearLogClicked(); break;
        case 11: _t->onExportLogClicked(); break;
        case 12: _t->onImportLogClicked(); break;
        case 13: _t->onFormatChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
