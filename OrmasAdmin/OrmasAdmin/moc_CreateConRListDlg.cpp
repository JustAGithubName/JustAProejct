/****************************************************************************
** Meta object code from reading C++ file 'CreateConRListDlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CreateConRListDlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CreateConRListDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CreateConRListDlg_t {
    QByteArrayData data[13];
    char stringdata0[139];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CreateConRListDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CreateConRListDlg_t qt_meta_stringdata_CreateConRListDlg = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CreateConRListDlg"
QT_MOC_LITERAL(1, 18, 16), // "AddProductToList"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 17), // "EditProductInList"
QT_MOC_LITERAL(4, 54, 13), // "DeleteProduct"
QT_MOC_LITERAL(5, 68, 5), // "Close"
QT_MOC_LITERAL(6, 74, 11), // "OpenConRDlg"
QT_MOC_LITERAL(7, 86, 11), // "OpenProdDlg"
QT_MOC_LITERAL(8, 98, 10), // "OpenStsDlg"
QT_MOC_LITERAL(9, 109, 10), // "OpenCurDlg"
QT_MOC_LITERAL(10, 120, 5), // "SetID"
QT_MOC_LITERAL(11, 126, 2), // "ID"
QT_MOC_LITERAL(12, 129, 9) // "childName"

    },
    "CreateConRListDlg\0AddProductToList\0\0"
    "EditProductInList\0DeleteProduct\0Close\0"
    "OpenConRDlg\0OpenProdDlg\0OpenStsDlg\0"
    "OpenCurDlg\0SetID\0ID\0childName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CreateConRListDlg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    2,   67,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   11,   12,

       0        // eod
};

void CreateConRListDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CreateConRListDlg *_t = static_cast<CreateConRListDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->AddProductToList(); break;
        case 1: _t->EditProductInList(); break;
        case 2: _t->DeleteProduct(); break;
        case 3: _t->Close(); break;
        case 4: _t->OpenConRDlg(); break;
        case 5: _t->OpenProdDlg(); break;
        case 6: _t->OpenStsDlg(); break;
        case 7: _t->OpenCurDlg(); break;
        case 8: _t->SetID((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject CreateConRListDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CreateConRListDlg.data,
      qt_meta_data_CreateConRListDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CreateConRListDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CreateConRListDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CreateConRListDlg.stringdata0))
        return static_cast<void*>(const_cast< CreateConRListDlg*>(this));
    if (!strcmp(_clname, "Ui::CreateConRList"))
        return static_cast< Ui::CreateConRList*>(const_cast< CreateConRListDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int CreateConRListDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
