/********************************************************************************
** Form generated from reading UI file 'CreateWOffRListDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEWOFFRLISTDLG_H
#define UI_CREATEWOFFRLISTDLG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CreateWOffRList
{
public:
    QGridLayout *gridLayout;
    QLabel *countLb;
    QLineEdit *productEdit;
    QPushButton *productBtn;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *addBtn;
    QPushButton *cancelBtn;
    QLineEdit *countEdit;
    QWidget *editSectionWgt;
    QGridLayout *gridLayout_3;
    QPushButton *currencyBtn;
    QLineEdit *currencyEdit;
    QLineEdit *sumEdit;
    QLabel *sumLb;
    QPushButton *statusBtn;
    QLineEdit *statusEdit;
    QPushButton *writeOffRawBtn;
    QLineEdit *writeOffRawEdit;

    void setupUi(QDialog *CreateWOffRList)
    {
        if (CreateWOffRList->objectName().isEmpty())
            CreateWOffRList->setObjectName(QStringLiteral("CreateWOffRList"));
        CreateWOffRList->resize(396, 221);
        CreateWOffRList->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        CreateWOffRList->setModal(false);
        gridLayout = new QGridLayout(CreateWOffRList);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(11, 11, 11, 17);
        countLb = new QLabel(CreateWOffRList);
        countLb->setObjectName(QStringLiteral("countLb"));
        countLb->setMinimumSize(QSize(150, 0));

        gridLayout->addWidget(countLb, 1, 0, 1, 1);

        productEdit = new QLineEdit(CreateWOffRList);
        productEdit->setObjectName(QStringLiteral("productEdit"));
        productEdit->setReadOnly(true);

        gridLayout->addWidget(productEdit, 0, 1, 1, 1);

        productBtn = new QPushButton(CreateWOffRList);
        productBtn->setObjectName(QStringLiteral("productBtn"));

        gridLayout->addWidget(productBtn, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        addBtn = new QPushButton(CreateWOffRList);
        addBtn->setObjectName(QStringLiteral("addBtn"));

        horizontalLayout->addWidget(addBtn);

        cancelBtn = new QPushButton(CreateWOffRList);
        cancelBtn->setObjectName(QStringLiteral("cancelBtn"));

        horizontalLayout->addWidget(cancelBtn);


        gridLayout->addLayout(horizontalLayout, 3, 0, 1, 2);

        countEdit = new QLineEdit(CreateWOffRList);
        countEdit->setObjectName(QStringLiteral("countEdit"));

        gridLayout->addWidget(countEdit, 1, 1, 1, 1);

        editSectionWgt = new QWidget(CreateWOffRList);
        editSectionWgt->setObjectName(QStringLiteral("editSectionWgt"));
        gridLayout_3 = new QGridLayout(editSectionWgt);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        currencyBtn = new QPushButton(editSectionWgt);
        currencyBtn->setObjectName(QStringLiteral("currencyBtn"));

        gridLayout_3->addWidget(currencyBtn, 3, 0, 1, 1);

        currencyEdit = new QLineEdit(editSectionWgt);
        currencyEdit->setObjectName(QStringLiteral("currencyEdit"));
        currencyEdit->setReadOnly(true);

        gridLayout_3->addWidget(currencyEdit, 3, 1, 1, 1);

        sumEdit = new QLineEdit(editSectionWgt);
        sumEdit->setObjectName(QStringLiteral("sumEdit"));

        gridLayout_3->addWidget(sumEdit, 2, 1, 1, 1);

        sumLb = new QLabel(editSectionWgt);
        sumLb->setObjectName(QStringLiteral("sumLb"));
        sumLb->setMinimumSize(QSize(150, 0));

        gridLayout_3->addWidget(sumLb, 2, 0, 1, 1);

        statusBtn = new QPushButton(editSectionWgt);
        statusBtn->setObjectName(QStringLiteral("statusBtn"));

        gridLayout_3->addWidget(statusBtn, 1, 0, 1, 1);

        statusEdit = new QLineEdit(editSectionWgt);
        statusEdit->setObjectName(QStringLiteral("statusEdit"));
        statusEdit->setReadOnly(true);

        gridLayout_3->addWidget(statusEdit, 1, 1, 1, 1);

        writeOffRawBtn = new QPushButton(editSectionWgt);
        writeOffRawBtn->setObjectName(QStringLiteral("writeOffRawBtn"));
        writeOffRawBtn->setMinimumSize(QSize(150, 0));

        gridLayout_3->addWidget(writeOffRawBtn, 0, 0, 1, 1);

        writeOffRawEdit = new QLineEdit(editSectionWgt);
        writeOffRawEdit->setObjectName(QStringLiteral("writeOffRawEdit"));
        writeOffRawEdit->setReadOnly(true);

        gridLayout_3->addWidget(writeOffRawEdit, 0, 1, 1, 1);


        gridLayout->addWidget(editSectionWgt, 2, 0, 1, 2);

        QWidget::setTabOrder(productBtn, productEdit);
        QWidget::setTabOrder(productEdit, countEdit);
        QWidget::setTabOrder(countEdit, writeOffRawBtn);
        QWidget::setTabOrder(writeOffRawBtn, writeOffRawEdit);
        QWidget::setTabOrder(writeOffRawEdit, statusBtn);
        QWidget::setTabOrder(statusBtn, statusEdit);
        QWidget::setTabOrder(statusEdit, sumEdit);
        QWidget::setTabOrder(sumEdit, currencyBtn);
        QWidget::setTabOrder(currencyBtn, currencyEdit);
        QWidget::setTabOrder(currencyEdit, addBtn);
        QWidget::setTabOrder(addBtn, cancelBtn);

        retranslateUi(CreateWOffRList);

        QMetaObject::connectSlotsByName(CreateWOffRList);
    } // setupUi

    void retranslateUi(QDialog *CreateWOffRList)
    {
        CreateWOffRList->setWindowTitle(QApplication::translate("CreateWOffRList", "Create/Update raw in write-off list", 0));
        countLb->setText(QApplication::translate("CreateWOffRList", "Count:", 0));
        productBtn->setText(QApplication::translate("CreateWOffRList", "Select product", 0));
        addBtn->setText(QApplication::translate("CreateWOffRList", "Add", 0));
        cancelBtn->setText(QApplication::translate("CreateWOffRList", "Cancel", 0));
#ifndef QT_NO_TOOLTIP
        countEdit->setToolTip(QApplication::translate("CreateWOffRList", "<html><head/><body><p>For example: 100</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        currencyBtn->setText(QApplication::translate("CreateWOffRList", "Select currency", 0));
#ifndef QT_NO_TOOLTIP
        sumEdit->setToolTip(QApplication::translate("CreateWOffRList", "<html><head/><body><p>For example: 300 (pedends on currency, in that case 300 USD)</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        sumLb->setText(QApplication::translate("CreateWOffRList", "Sum:", 0));
        statusBtn->setText(QApplication::translate("CreateWOffRList", "Select status", 0));
        writeOffRawBtn->setText(QApplication::translate("CreateWOffRList", "Select write-off raw", 0));
        writeOffRawEdit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CreateWOffRList: public Ui_CreateWOffRList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEWOFFRLISTDLG_H
