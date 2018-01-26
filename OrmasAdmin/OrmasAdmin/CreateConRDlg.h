#ifndef CREATECONRDLG_H
#define CREATECONRDLG_H

#include "ui_CreateConRDlg.h"
#include "OrmasBL.h"

class CreateConRDlg : public QDialog, public Ui::CreateConsumeRaw
{
	Q_OBJECT
public:
	CreateConRDlg(BusinessLayer::OrmasBL *ormasBL, bool updateFlag, QWidget *parent = 0);
	~CreateConRDlg();
	BusinessLayer::OrmasBL *dialogBL;
	bool FillDlgElements(QTableView*);
	std::string errorMessage;
	private slots:
	void CreateConsumeRaw();
	void EditConsumeRaw();
	void DeleteConsumeRaw(){};
	void Close();
	void OpenEmpDlg();
	void OpenSkEmpDlg();
	void OpenStsDlg();
	void OpenCurDlg();
	void OpenConRListDlg();
	void StatusWasChenged();
	public slots:
	void SetID(int ID, QString childName);
signals:
	void CloseCreatedForms();
private:
	BusinessLayer::ConsumeRaw *consumeRaw = new BusinessLayer::ConsumeRaw();
	void SetConsumeRawParams(int, QString, QString, int, int, double, int, int, int = 0);
	void FillEditElements(int, QString, QString, int, int, double, int, int);
	QDoubleValidator *vDouble = nullptr;
	QIntValidator *vInt = nullptr;
	std::map<std::string, int> statusMap;
};
#endif //CREATECONRDLG_H