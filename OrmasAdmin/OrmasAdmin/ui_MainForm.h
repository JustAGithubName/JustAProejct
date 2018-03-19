/********************************************************************************
** Form generated from reading UI file 'MainForm.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFORM_H
#define UI_MAINFORM_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAllUsers;
    QAction *actionClients;
    QAction *actionEmployees;
    QAction *actionAccess;
    QAction *actionProductType;
    QAction *actionProducts;
    QAction *actionOrders;
    QAction *actionProductsList;
    QAction *actionOrderList;
    QAction *actionDeleteOrder;
    QAction *actionReturns;
    QAction *actionReturnList;
    QAction *actionDeleteReturn;
    QAction *actionCompany;
    QAction *actionCurrency;
    QAction *actionMeasure;
    QAction *actionLocation;
    QAction *actionRoles;
    QAction *actionStatus;
    QAction *actionAbout;
    QAction *actionProduction;
    QAction *actionAccessItems;
    QAction *actionPrices;
    QAction *actionProductionList;
    QAction *actionWriteOffs;
    QAction *actionWriteOffList;
    QAction *actionUserPhoto;
    QAction *actionProductPhoto;
    QAction *actionPosition;
    QAction *actionRelation;
    QAction *actionRelationType;
    QAction *actionBalances;
    QAction *actionSalary;
    QAction *actionSalaryType;
    QAction *actionPayments;
    QAction *actionPayroll;
    QAction *actionWithdrawal;
    QAction *actionRefund;
    QAction *actionPayslip;
    QAction *actionOrderRaws;
    QAction *actionReceiptRaws;
    QAction *actionTransports;
    QAction *actionReceiptProducts;
    QAction *actionConsumptionRaws;
    QAction *actionWriteOffRaws;
    QAction *actionConsumptionProducts;
    QAction *actionStock;
    QAction *actionInventorization;
    QAction *actionSpecifications;
    QAction *actionSpecificationList;
    QAction *actionJobprice;
    QAction *actionJobsheet;
    QAction *actionPercentRate;
    QAction *actionTimesheet;
    QAction *actionAccounts;
    QAction *actionAccountType;
    QAction *actionChartOfAccounts;
    QAction *actionCompanyAccounts;
    QAction *actionEntry;
    QAction *actionNetCost;
    QAction *actionProductionPlan;
    QAction *actionProductionPlanList;
    QAction *actionSpoilage;
    QAction *actionSpoilageList;
    QAction *actionPurveyors;
    QAction *actionGenerateAccounts;
    QAction *actionGenerateOneAccount;
    QWidget *centrWidget;
    QGridLayout *gridLayout;
    QMdiArea *mdiArea;
    QMenuBar *menuBar;
    QMenu *menuUsers;
    QMenu *menuProducts;
    QMenu *menuOrders;
    QMenu *menuReturns;
    QMenu *menuReferences;
    QMenu *menuHelp;
    QMenu *menuProductions;
    QMenu *menuWriteOffs;
    QMenu *menuAccountings;
    QMenu *menuStock;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        MainWindow->setMinimumSize(QSize(800, 600));
        MainWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        actionAllUsers = new QAction(MainWindow);
        actionAllUsers->setObjectName(QStringLiteral("actionAllUsers"));
        actionClients = new QAction(MainWindow);
        actionClients->setObjectName(QStringLiteral("actionClients"));
        actionEmployees = new QAction(MainWindow);
        actionEmployees->setObjectName(QStringLiteral("actionEmployees"));
        actionAccess = new QAction(MainWindow);
        actionAccess->setObjectName(QStringLiteral("actionAccess"));
        actionProductType = new QAction(MainWindow);
        actionProductType->setObjectName(QStringLiteral("actionProductType"));
        actionProducts = new QAction(MainWindow);
        actionProducts->setObjectName(QStringLiteral("actionProducts"));
        actionOrders = new QAction(MainWindow);
        actionOrders->setObjectName(QStringLiteral("actionOrders"));
        actionProductsList = new QAction(MainWindow);
        actionProductsList->setObjectName(QStringLiteral("actionProductsList"));
        actionOrderList = new QAction(MainWindow);
        actionOrderList->setObjectName(QStringLiteral("actionOrderList"));
        actionDeleteOrder = new QAction(MainWindow);
        actionDeleteOrder->setObjectName(QStringLiteral("actionDeleteOrder"));
        actionReturns = new QAction(MainWindow);
        actionReturns->setObjectName(QStringLiteral("actionReturns"));
        actionReturnList = new QAction(MainWindow);
        actionReturnList->setObjectName(QStringLiteral("actionReturnList"));
        actionDeleteReturn = new QAction(MainWindow);
        actionDeleteReturn->setObjectName(QStringLiteral("actionDeleteReturn"));
        actionCompany = new QAction(MainWindow);
        actionCompany->setObjectName(QStringLiteral("actionCompany"));
        actionCurrency = new QAction(MainWindow);
        actionCurrency->setObjectName(QStringLiteral("actionCurrency"));
        actionMeasure = new QAction(MainWindow);
        actionMeasure->setObjectName(QStringLiteral("actionMeasure"));
        actionLocation = new QAction(MainWindow);
        actionLocation->setObjectName(QStringLiteral("actionLocation"));
        actionRoles = new QAction(MainWindow);
        actionRoles->setObjectName(QStringLiteral("actionRoles"));
        actionStatus = new QAction(MainWindow);
        actionStatus->setObjectName(QStringLiteral("actionStatus"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionProduction = new QAction(MainWindow);
        actionProduction->setObjectName(QStringLiteral("actionProduction"));
        actionAccessItems = new QAction(MainWindow);
        actionAccessItems->setObjectName(QStringLiteral("actionAccessItems"));
        actionPrices = new QAction(MainWindow);
        actionPrices->setObjectName(QStringLiteral("actionPrices"));
        actionProductionList = new QAction(MainWindow);
        actionProductionList->setObjectName(QStringLiteral("actionProductionList"));
        actionWriteOffs = new QAction(MainWindow);
        actionWriteOffs->setObjectName(QStringLiteral("actionWriteOffs"));
        actionWriteOffList = new QAction(MainWindow);
        actionWriteOffList->setObjectName(QStringLiteral("actionWriteOffList"));
        actionUserPhoto = new QAction(MainWindow);
        actionUserPhoto->setObjectName(QStringLiteral("actionUserPhoto"));
        actionProductPhoto = new QAction(MainWindow);
        actionProductPhoto->setObjectName(QStringLiteral("actionProductPhoto"));
        actionPosition = new QAction(MainWindow);
        actionPosition->setObjectName(QStringLiteral("actionPosition"));
        actionRelation = new QAction(MainWindow);
        actionRelation->setObjectName(QStringLiteral("actionRelation"));
        actionRelationType = new QAction(MainWindow);
        actionRelationType->setObjectName(QStringLiteral("actionRelationType"));
        actionBalances = new QAction(MainWindow);
        actionBalances->setObjectName(QStringLiteral("actionBalances"));
        actionSalary = new QAction(MainWindow);
        actionSalary->setObjectName(QStringLiteral("actionSalary"));
        actionSalaryType = new QAction(MainWindow);
        actionSalaryType->setObjectName(QStringLiteral("actionSalaryType"));
        actionPayments = new QAction(MainWindow);
        actionPayments->setObjectName(QStringLiteral("actionPayments"));
        actionPayroll = new QAction(MainWindow);
        actionPayroll->setObjectName(QStringLiteral("actionPayroll"));
        actionWithdrawal = new QAction(MainWindow);
        actionWithdrawal->setObjectName(QStringLiteral("actionWithdrawal"));
        actionRefund = new QAction(MainWindow);
        actionRefund->setObjectName(QStringLiteral("actionRefund"));
        actionPayslip = new QAction(MainWindow);
        actionPayslip->setObjectName(QStringLiteral("actionPayslip"));
        actionOrderRaws = new QAction(MainWindow);
        actionOrderRaws->setObjectName(QStringLiteral("actionOrderRaws"));
        actionReceiptRaws = new QAction(MainWindow);
        actionReceiptRaws->setObjectName(QStringLiteral("actionReceiptRaws"));
        actionTransports = new QAction(MainWindow);
        actionTransports->setObjectName(QStringLiteral("actionTransports"));
        actionReceiptProducts = new QAction(MainWindow);
        actionReceiptProducts->setObjectName(QStringLiteral("actionReceiptProducts"));
        actionConsumptionRaws = new QAction(MainWindow);
        actionConsumptionRaws->setObjectName(QStringLiteral("actionConsumptionRaws"));
        actionWriteOffRaws = new QAction(MainWindow);
        actionWriteOffRaws->setObjectName(QStringLiteral("actionWriteOffRaws"));
        actionConsumptionProducts = new QAction(MainWindow);
        actionConsumptionProducts->setObjectName(QStringLiteral("actionConsumptionProducts"));
        actionStock = new QAction(MainWindow);
        actionStock->setObjectName(QStringLiteral("actionStock"));
        actionInventorization = new QAction(MainWindow);
        actionInventorization->setObjectName(QStringLiteral("actionInventorization"));
        actionSpecifications = new QAction(MainWindow);
        actionSpecifications->setObjectName(QStringLiteral("actionSpecifications"));
        actionSpecificationList = new QAction(MainWindow);
        actionSpecificationList->setObjectName(QStringLiteral("actionSpecificationList"));
        actionJobprice = new QAction(MainWindow);
        actionJobprice->setObjectName(QStringLiteral("actionJobprice"));
        actionJobsheet = new QAction(MainWindow);
        actionJobsheet->setObjectName(QStringLiteral("actionJobsheet"));
        actionPercentRate = new QAction(MainWindow);
        actionPercentRate->setObjectName(QStringLiteral("actionPercentRate"));
        actionTimesheet = new QAction(MainWindow);
        actionTimesheet->setObjectName(QStringLiteral("actionTimesheet"));
        actionAccounts = new QAction(MainWindow);
        actionAccounts->setObjectName(QStringLiteral("actionAccounts"));
        actionAccountType = new QAction(MainWindow);
        actionAccountType->setObjectName(QStringLiteral("actionAccountType"));
        actionChartOfAccounts = new QAction(MainWindow);
        actionChartOfAccounts->setObjectName(QStringLiteral("actionChartOfAccounts"));
        actionCompanyAccounts = new QAction(MainWindow);
        actionCompanyAccounts->setObjectName(QStringLiteral("actionCompanyAccounts"));
        actionEntry = new QAction(MainWindow);
        actionEntry->setObjectName(QStringLiteral("actionEntry"));
        actionNetCost = new QAction(MainWindow);
        actionNetCost->setObjectName(QStringLiteral("actionNetCost"));
        actionProductionPlan = new QAction(MainWindow);
        actionProductionPlan->setObjectName(QStringLiteral("actionProductionPlan"));
        actionProductionPlanList = new QAction(MainWindow);
        actionProductionPlanList->setObjectName(QStringLiteral("actionProductionPlanList"));
        actionSpoilage = new QAction(MainWindow);
        actionSpoilage->setObjectName(QStringLiteral("actionSpoilage"));
        actionSpoilageList = new QAction(MainWindow);
        actionSpoilageList->setObjectName(QStringLiteral("actionSpoilageList"));
        actionPurveyors = new QAction(MainWindow);
        actionPurveyors->setObjectName(QStringLiteral("actionPurveyors"));
        actionGenerateAccounts = new QAction(MainWindow);
        actionGenerateAccounts->setObjectName(QStringLiteral("actionGenerateAccounts"));
        actionGenerateOneAccount = new QAction(MainWindow);
        actionGenerateOneAccount->setObjectName(QStringLiteral("actionGenerateOneAccount"));
        centrWidget = new QWidget(MainWindow);
        centrWidget->setObjectName(QStringLiteral("centrWidget"));
        gridLayout = new QGridLayout(centrWidget);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        mdiArea = new QMdiArea(centrWidget);
        mdiArea->setObjectName(QStringLiteral("mdiArea"));

        gridLayout->addWidget(mdiArea, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centrWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 21));
        menuUsers = new QMenu(menuBar);
        menuUsers->setObjectName(QStringLiteral("menuUsers"));
        menuUsers->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        menuProducts = new QMenu(menuBar);
        menuProducts->setObjectName(QStringLiteral("menuProducts"));
        menuOrders = new QMenu(menuBar);
        menuOrders->setObjectName(QStringLiteral("menuOrders"));
        menuReturns = new QMenu(menuBar);
        menuReturns->setObjectName(QStringLiteral("menuReturns"));
        menuReferences = new QMenu(menuBar);
        menuReferences->setObjectName(QStringLiteral("menuReferences"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuProductions = new QMenu(menuBar);
        menuProductions->setObjectName(QStringLiteral("menuProductions"));
        menuWriteOffs = new QMenu(menuBar);
        menuWriteOffs->setObjectName(QStringLiteral("menuWriteOffs"));
        menuAccountings = new QMenu(menuBar);
        menuAccountings->setObjectName(QStringLiteral("menuAccountings"));
        menuStock = new QMenu(menuBar);
        menuStock->setObjectName(QStringLiteral("menuStock"));
        MainWindow->setMenuBar(menuBar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menuBar->addAction(menuUsers->menuAction());
        menuBar->addAction(menuProducts->menuAction());
        menuBar->addAction(menuOrders->menuAction());
        menuBar->addAction(menuReturns->menuAction());
        menuBar->addAction(menuWriteOffs->menuAction());
        menuBar->addAction(menuProductions->menuAction());
        menuBar->addAction(menuAccountings->menuAction());
        menuBar->addAction(menuStock->menuAction());
        menuBar->addAction(menuReferences->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuUsers->addAction(actionAllUsers);
        menuUsers->addAction(actionClients);
        menuUsers->addAction(actionEmployees);
        menuUsers->addAction(actionPurveyors);
        menuUsers->addSeparator();
        menuUsers->addAction(actionAccess);
        menuUsers->addAction(actionAccessItems);
        menuUsers->addAction(actionUserPhoto);
        menuUsers->addAction(actionPosition);
        menuProducts->addAction(actionProductType);
        menuProducts->addAction(actionProducts);
        menuProducts->addAction(actionProductPhoto);
        menuProducts->addAction(actionSpecifications);
        menuProducts->addAction(actionSpecificationList);
        menuOrders->addAction(actionOrders);
        menuOrders->addAction(actionOrderList);
        menuReturns->addAction(actionReturns);
        menuReturns->addAction(actionReturnList);
        menuReferences->addAction(actionCompany);
        menuReferences->addAction(actionCurrency);
        menuReferences->addAction(actionMeasure);
        menuReferences->addAction(actionLocation);
        menuReferences->addAction(actionRoles);
        menuReferences->addAction(actionStatus);
        menuReferences->addAction(actionRelation);
        menuReferences->addAction(actionRelationType);
        menuHelp->addAction(actionAbout);
        menuProductions->addAction(actionProduction);
        menuProductions->addAction(actionProductionList);
        menuProductions->addAction(actionJobprice);
        menuProductions->addAction(actionJobsheet);
        menuProductions->addAction(actionPercentRate);
        menuProductions->addAction(actionTimesheet);
        menuProductions->addAction(actionProductionPlan);
        menuProductions->addAction(actionProductionPlanList);
        menuProductions->addAction(actionSpoilage);
        menuProductions->addAction(actionSpoilageList);
        menuWriteOffs->addAction(actionWriteOffs);
        menuWriteOffs->addAction(actionWriteOffList);
        menuAccountings->addAction(actionBalances);
        menuAccountings->addAction(actionPayments);
        menuAccountings->addAction(actionRefund);
        menuAccountings->addAction(actionNetCost);
        menuAccountings->addAction(actionPrices);
        menuAccountings->addSeparator();
        menuAccountings->addAction(actionSalary);
        menuAccountings->addAction(actionSalaryType);
        menuAccountings->addAction(actionPayroll);
        menuAccountings->addAction(actionWithdrawal);
        menuAccountings->addAction(actionPayslip);
        menuAccountings->addSeparator();
        menuAccountings->addAction(actionAccounts);
        menuAccountings->addAction(actionAccountType);
        menuAccountings->addAction(actionChartOfAccounts);
        menuAccountings->addAction(actionCompanyAccounts);
        menuAccountings->addAction(actionEntry);
        menuAccountings->addAction(actionGenerateAccounts);
        menuAccountings->addAction(actionGenerateOneAccount);
        menuStock->addAction(actionStock);
        menuStock->addAction(actionOrderRaws);
        menuStock->addAction(actionReceiptRaws);
        menuStock->addAction(actionConsumptionRaws);
        menuStock->addAction(actionWriteOffRaws);
        menuStock->addAction(actionTransports);
        menuStock->addAction(actionReceiptProducts);
        menuStock->addAction(actionConsumptionProducts);
        menuStock->addAction(actionInventorization);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Ormas Admin Panel", 0));
        actionAllUsers->setText(QApplication::translate("MainWindow", "All users", 0));
        actionClients->setText(QApplication::translate("MainWindow", "Clients", 0));
        actionEmployees->setText(QApplication::translate("MainWindow", "Employees", 0));
        actionAccess->setText(QApplication::translate("MainWindow", "Access", 0));
        actionProductType->setText(QApplication::translate("MainWindow", "Products type", 0));
        actionProducts->setText(QApplication::translate("MainWindow", "Products", 0));
        actionOrders->setText(QApplication::translate("MainWindow", "Orders", 0));
        actionProductsList->setText(QApplication::translate("MainWindow", "Products list", 0));
        actionOrderList->setText(QApplication::translate("MainWindow", "Order details", 0));
        actionDeleteOrder->setText(QApplication::translate("MainWindow", "Delete order", 0));
        actionReturns->setText(QApplication::translate("MainWindow", "Returns", 0));
        actionReturnList->setText(QApplication::translate("MainWindow", "Retrun details", 0));
        actionDeleteReturn->setText(QApplication::translate("MainWindow", "Delete return", 0));
        actionCompany->setText(QApplication::translate("MainWindow", "Company", 0));
        actionCurrency->setText(QApplication::translate("MainWindow", "Currency", 0));
        actionMeasure->setText(QApplication::translate("MainWindow", "Measure", 0));
        actionLocation->setText(QApplication::translate("MainWindow", "Location", 0));
        actionRoles->setText(QApplication::translate("MainWindow", "Roles", 0));
        actionStatus->setText(QApplication::translate("MainWindow", "Status", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionProduction->setText(QApplication::translate("MainWindow", "Production", 0));
        actionAccessItems->setText(QApplication::translate("MainWindow", "Access items", 0));
        actionPrices->setText(QApplication::translate("MainWindow", "Prices", 0));
        actionProductionList->setText(QApplication::translate("MainWindow", "Production details", 0));
        actionWriteOffs->setText(QApplication::translate("MainWindow", "Write-offs", 0));
        actionWriteOffList->setText(QApplication::translate("MainWindow", "Write-off details", 0));
        actionUserPhoto->setText(QApplication::translate("MainWindow", "User photo", 0));
        actionProductPhoto->setText(QApplication::translate("MainWindow", "Product photo", 0));
        actionPosition->setText(QApplication::translate("MainWindow", "Position", 0));
        actionRelation->setText(QApplication::translate("MainWindow", "Relation", 0));
        actionRelationType->setText(QApplication::translate("MainWindow", "Relation types", 0));
        actionBalances->setText(QApplication::translate("MainWindow", "Balances", 0));
        actionSalary->setText(QApplication::translate("MainWindow", "Salary", 0));
        actionSalaryType->setText(QApplication::translate("MainWindow", "Salary type", 0));
        actionPayments->setText(QApplication::translate("MainWindow", "Payments", 0));
        actionPayroll->setText(QApplication::translate("MainWindow", "Payroll", 0));
        actionWithdrawal->setText(QApplication::translate("MainWindow", "Withdrawal", 0));
        actionRefund->setText(QApplication::translate("MainWindow", "Refund", 0));
        actionPayslip->setText(QApplication::translate("MainWindow", "Payslip", 0));
        actionOrderRaws->setText(QApplication::translate("MainWindow", "Order raw materials", 0));
        actionReceiptRaws->setText(QApplication::translate("MainWindow", "Receipt raw materials", 0));
        actionTransports->setText(QApplication::translate("MainWindow", "Transports", 0));
        actionReceiptProducts->setText(QApplication::translate("MainWindow", "Receipt products", 0));
        actionConsumptionRaws->setText(QApplication::translate("MainWindow", "Consumption raw materials", 0));
        actionWriteOffRaws->setText(QApplication::translate("MainWindow", "Write-off raw materials", 0));
        actionConsumptionProducts->setText(QApplication::translate("MainWindow", "Consumption products", 0));
        actionStock->setText(QApplication::translate("MainWindow", "Stock", 0));
        actionInventorization->setText(QApplication::translate("MainWindow", "Inventorization", 0));
        actionSpecifications->setText(QApplication::translate("MainWindow", "Specification", 0));
        actionSpecificationList->setText(QApplication::translate("MainWindow", "Specification details", 0));
        actionJobprice->setText(QApplication::translate("MainWindow", "Jobprice", 0));
        actionJobsheet->setText(QApplication::translate("MainWindow", "Jobsheet", 0));
        actionPercentRate->setText(QApplication::translate("MainWindow", "Percent rate", 0));
        actionTimesheet->setText(QApplication::translate("MainWindow", "Timesheet", 0));
        actionAccounts->setText(QApplication::translate("MainWindow", "Accounts", 0));
        actionAccountType->setText(QApplication::translate("MainWindow", "Account type", 0));
        actionChartOfAccounts->setText(QApplication::translate("MainWindow", "Chart of Accounts", 0));
        actionCompanyAccounts->setText(QApplication::translate("MainWindow", "Company accouts", 0));
        actionEntry->setText(QApplication::translate("MainWindow", "Entry", 0));
        actionNetCost->setText(QApplication::translate("MainWindow", "Net cost", 0));
        actionProductionPlan->setText(QApplication::translate("MainWindow", "Production plan", 0));
        actionProductionPlanList->setText(QApplication::translate("MainWindow", "Production plan details", 0));
        actionSpoilage->setText(QApplication::translate("MainWindow", "Spoilage", 0));
        actionSpoilageList->setText(QApplication::translate("MainWindow", "Spoilage details", 0));
        actionPurveyors->setText(QApplication::translate("MainWindow", "Purveyors", 0));
        actionGenerateAccounts->setText(QApplication::translate("MainWindow", "Generate accounts", 0));
        actionGenerateOneAccount->setText(QApplication::translate("MainWindow", "Generate one account", 0));
        menuUsers->setTitle(QApplication::translate("MainWindow", "Users", 0));
        menuProducts->setTitle(QApplication::translate("MainWindow", "Products", 0));
        menuOrders->setTitle(QApplication::translate("MainWindow", "Orders", 0));
        menuReturns->setTitle(QApplication::translate("MainWindow", "Returns", 0));
        menuReferences->setTitle(QApplication::translate("MainWindow", "Reference", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        menuProductions->setTitle(QApplication::translate("MainWindow", "Production", 0));
        menuWriteOffs->setTitle(QApplication::translate("MainWindow", "Write-offs", 0));
        menuAccountings->setTitle(QApplication::translate("MainWindow", "Accounting", 0));
        menuStock->setTitle(QApplication::translate("MainWindow", "Stock", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFORM_H
