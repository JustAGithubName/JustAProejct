#ifndef CREATEWOFFDLG_H
#define CREATEWOFFDLG_H

#include "ui_CreateWOffDlg.h"
#include "OrmasBL.h"

class CreateWOffDlg : public QDialog, public Ui::CreateWriteOff
{
	Q_OBJECT
public:
	CreateWOffDlg(BusinessLayer::OrmasBL *ormasBL, bool updateFlag, QWidget *parent = 0);
	~CreateWOffDlg();
	BusinessLayer::OrmasBL *dialogBL;
	bool FillDlgElements(QTableView*);
	std::string errorMessage;
	private slots:
	void CreateWriteOff();
	void EditWriteOff();
	void DeleteWriteOff(){};
	void Close();
	void OpenCltDlg();
	void OpenEmpDlg();
	void OpenStsDlg();
	void OpenCurDlg();
	void OpenWOffListDlg();
	public slots:
	void SetID(int ID, QString childName);
private:
	BusinessLayer::WriteOff *writeOff = new BusinessLayer::WriteOff();
	void SetWriteOffParams(int, QString, int, int, double, int, int);
	void FillEditElements(int, QString, int, int, double, int, int);
	QDoubleValidator *vDouble = nullptr;
	QIntValidator *vInt = nullptr;
};
#endif //CREATEWOFFDLG_H