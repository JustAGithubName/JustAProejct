/****************************************************************************
** Meta object code from reading C++ file 'CreateEmpDlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CreateEmpDlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CreateEmpDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CreateEmpDlg_t {
    QByteArrayData data[11];
    char stringdata0[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CreateEmpDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CreateEmpDlg_t qt_meta_stringdata_CreateEmpDlg = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CreateEmpDlg"
QT_MOC_LITERAL(1, 13, 14), // "CreateEmployee"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 12), // "EditEmployee"
QT_MOC_LITERAL(4, 42, 14), // "DeleteEmployee"
QT_MOC_LITERAL(5, 57, 5), // "Close"
QT_MOC_LITERAL(6, 63, 11), // "OpenRoleDlg"
QT_MOC_LITERAL(7, 75, 10), // "OpenPsnDlg"
QT_MOC_LITERAL(8, 86, 5), // "SetID"
QT_MOC_LITERAL(9, 92, 2), // "ID"
QT_MOC_LITERAL(10, 95, 9) // "childName"

    },
    "CreateEmpDlg\0CreateEmployee\0\0EditEmployee\0"
    "DeleteEmployee\0Close\0OpenRoleDlg\0"
    "OpenPsnDlg\0SetID\0ID\0childName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CreateEmpDlg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    2,   55,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    9,   10,

       0        // eod
};

void CreateEmpDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CreateEmpDlg *_t = static_cast<CreateEmpDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->CreateEmployee(); break;
        case 1: _t->EditEmployee(); break;
        case 2: _t->DeleteEmployee(); break;
        case 3: _t->Close(); break;
        case 4: _t->OpenRoleDlg(); break;
        case 5: _t->OpenPsnDlg(); break;
        case 6: _t->SetID((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject CreateEmpDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CreateEmpDlg.data,
      qt_meta_data_CreateEmpDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CreateEmpDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CreateEmpDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CreateEmpDlg.stringdata0))
        return static_cast<void*>(const_cast< CreateEmpDlg*>(this));
    if (!strcmp(_clname, "Ui::CreateEmployee"))
        return static_cast< Ui::CreateEmployee*>(const_cast< CreateEmpDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int CreateEmpDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
    return _id;
}
QT_END_MOC_NAMESPACE
