#ifndef MAINFORM_H
#define MAINFORM_H

#include "ui_MainForm.h"
#include "OrmasBL.h"
#include "DataForm.h"

class MainForm : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

public:
	MainForm(BusinessLayer::OrmasBL *ormasBL, BusinessLayer::User* user);
	~MainForm();
	BusinessLayer::OrmasBL *oBL;
	QMdiSubWindow* GetWindowByName(QString);
	std::string errorMessage = "";
	std::vector<int> rights;
private slots :
	void OpenUserForm();
	void OpenClientForm();
	void OpenEmployeeForm();
	void OpenAccessForm();
	void OpenAccessItemForm();
	void OpenUserPhotoForm(); 
	void OpenPositionForm();

	void OpenProductForm();
	void OpenProductTypeForm();
	void OpenProductPhotoForm();

	void OpenOrderForm();	
	void OpenOrderListForm();

	void OpenReturnForm();
	void OpenReturnListForm();

	void OpenProductionForm();
	void OpenProductionListForm();

	void OpenWriteOffForm();
	void OpenWriteOffListForm();

	void OpenBalanceForm();
	void OpenPaymentForm();
	void OpenPriceForm();
	void OpenSalaryForm();
	void OpenSalaryTypeForm();

	void OpenCompanyForm();
	void OpenCurrencyForm();
	void OpenMeasureForm();
	void OpenLocationForm();
	void OpenRoleForm();
	void OpenStatusForm();
	void OpenRelationForm();
	void OpenRelationTypeForm();
	
	void OpenAboutForm();
private:
	void CreateConnections();
	BusinessLayer::User *loggedUser = nullptr;
	BusinessLayer::Access *userAccess = nullptr;
	void SetMenuItemsByAccess(std::vector<int> rights);
	void SetAllMenuInvisible();
	void SetAllMenuVisible();
};
#endif //MAINFORM_h