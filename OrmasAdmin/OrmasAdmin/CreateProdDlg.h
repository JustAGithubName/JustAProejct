#ifndef CREATEPRODDLG_H
#define CREATEPRODDLG_H
#include "ui_CreateProdDlg.h"
#include "OrmasBL.h"

class CreateProdDlg : public QDialog, public Ui::CreateProduct
{
	Q_OBJECT
public:
	CreateProdDlg(BusinessLayer::OrmasBL *ormasBL, bool updateFlag, QWidget *parent = 0);
	~CreateProdDlg(){};
	BusinessLayer::OrmasBL *dialogBL;
	bool FillDlgElements(QTableView*);
	std::string errorMessage;
	private slots:
	void CreateProduct();
	void EditProduct();
	void DeleteProduct(){};
	void Close();
	void OpenCmpDlg();
	void OpenMsrDlg();
	void OpenPrdTpDlg();
	void OpenCurDlg();
	public slots:
	void SetID(int ID, QString childName);
private:
	BusinessLayer::Product *product = new BusinessLayer::Product();
	void SetProductParams(int, QString, double, int, double, int, int, int, int = 0);
	void FillEditElements(int, QString, double, int, double, int, int, int);
};
#endif //CREATEPRODDLG_H