/********************************************************************************
** Form generated from reading UI file 'CreateRoleDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEROLEDLG_H
#define UI_CREATEROLEDLG_H

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
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_CreateRole
{
public:
    QGridLayout *gridLayout;
    QLabel *nameLb;
    QLabel *label_2;
    QLineEdit *nameEdit;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QTextEdit *commentTextEdit;

    void setupUi(QDialog *CreateRole)
    {
        if (CreateRole->objectName().isEmpty())
            CreateRole->setObjectName(QStringLiteral("CreateRole"));
        CreateRole->resize(400, 150);
        CreateRole->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        CreateRole->setModal(false);
        gridLayout = new QGridLayout(CreateRole);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(11, 11, 11, 11);
        nameLb = new QLabel(CreateRole);
        nameLb->setObjectName(QStringLiteral("nameLb"));
        nameLb->setMinimumSize(QSize(100, 0));

        gridLayout->addWidget(nameLb, 0, 0, 1, 1);

        label_2 = new QLabel(CreateRole);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        nameEdit = new QLineEdit(CreateRole);
        nameEdit->setObjectName(QStringLiteral("nameEdit"));

        gridLayout->addWidget(nameEdit, 0, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okBtn = new QPushButton(CreateRole);
        okBtn->setObjectName(QStringLiteral("okBtn"));

        horizontalLayout->addWidget(okBtn);

        cancelBtn = new QPushButton(CreateRole);
        cancelBtn->setObjectName(QStringLiteral("cancelBtn"));

        horizontalLayout->addWidget(cancelBtn);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 2);

        commentTextEdit = new QTextEdit(CreateRole);
        commentTextEdit->setObjectName(QStringLiteral("commentTextEdit"));

        gridLayout->addWidget(commentTextEdit, 1, 1, 1, 1);


        retranslateUi(CreateRole);

        QMetaObject::connectSlotsByName(CreateRole);
    } // setupUi

    void retranslateUi(QDialog *CreateRole)
    {
        CreateRole->setWindowTitle(QApplication::translate("CreateRole", "Create/Update role", 0));
        nameLb->setText(QApplication::translate("CreateRole", "Name:", 0));
        label_2->setText(QApplication::translate("CreateRole", "Comment:", 0));
#ifndef QT_NO_TOOLTIP
        nameEdit->setToolTip(QApplication::translate("CreateRole", "Must not be empty", 0));
#endif // QT_NO_TOOLTIP
        okBtn->setText(QApplication::translate("CreateRole", "OK", 0));
        cancelBtn->setText(QApplication::translate("CreateRole", "Cancel", 0));
#ifndef QT_NO_TOOLTIP
        commentTextEdit->setToolTip(QApplication::translate("CreateRole", "Can leave it an empty", 0));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class CreateRole: public Ui_CreateRole {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEROLEDLG_H
