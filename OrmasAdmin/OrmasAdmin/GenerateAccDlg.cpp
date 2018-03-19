#include "stdafx.h"
#include "GenerateAccDlg.h"
#include "MainForm.h"
#include "ExtraFunctions.h"
#include <QMessageBox>
#include <QProgressDialog>


GenerateAcc::GenerateAcc(BusinessLayer::OrmasBL *ormasBL, QWidget *parent) :QDialog(parent)
{
	setupUi(this);
	setModal(true);
	dialogBL = ormasBL;
	InitComboBox();
	
	QObject::connect(generateBtn, &QPushButton::released, this, &GenerateAcc::Generate);
	QObject::connect(cancelBtn, &QPushButton::released, this, &GenerateAcc::Close);
}

GenerateAcc::~GenerateAcc()
{
	delete account;
	delete coAcc;
	delete status;
	delete caRelation;
}

void GenerateAcc::Generate()
{
	QProgressDialog progressDialog(this);
	std::vector<BusinessLayer::ChartOfAccountsView> coaVector = dialogBL->GetAllDataForClass<BusinessLayer::ChartOfAccountsView>(errorMessage);
	if (coaVector.size() > 0)
	{
		progressDialog.setRange(0, coaVector.size());
		progressDialog.setWindowFlags(Qt::FramelessWindowHint);
		
		int i = 0;
		BusinessLayer::AccountType acType;
		BusinessLayer::Currency currency;
		std::string number = "";
		std::string genAccRawNumber = "";

		status->SetName("OPEN");
		std::string statusFilter = dialogBL->GenerateFilter<BusinessLayer::Status>(status);
		std::vector<BusinessLayer::Status> statusVector = dialogBL->GetAllDataForClass<BusinessLayer::Status>(errorMessage, statusFilter);
		if (0 == statusVector.size())
		{
			QMessageBox::information(NULL, QString(tr("Warning")),
				QString(tr("Status are empty please contact with Administrator")),
				QString(tr("Ok")));
			errorMessage.clear();
			Close();
		}
		status->SetID(statusVector.at(0).GetID());

		for each (auto coaItem in coaVector)
		{
			if (!acType.GetAccountTypeByID(dialogBL->GetOrmasDal(), coaItem.GetAccountTypeID(), errorMessage))
				continue;
			number = std::to_string(coaItem.GetNumber());
			number.append(std::to_string(currency.GetCode()));
			number.append(std::to_string(acType.GetNumber()));
			genAccRawNumber = account->GenerateRawNumber(dialogBL->GetOrmasDal(), errorMessage);
			if (genAccRawNumber.empty())
			{
				QMessageBox::information(NULL, QString(tr("Info")),
					QString(tr("Cannot generate account number! Please contact with Administrator!")),
					QString(tr("Ok")));
				Close();
			}
			number.append(genAccRawNumber);

			account->Clear();
			account->SetNumber(number);
			account->SetStartBalance(0.0);
			account->SetCurrentBalance(0.0);
			account->SetCurrencyID(currencyCmb->currentData().toInt());
			account->SetStatusID(status->GetID());
			account->SetOpenedDate(dialogBL->GetOrmasDal().GetSystemDate());
			account->SetClosedDate("");
			account->SetDetails("Generated by system");

			if (!account->CreateAccount(dialogBL->GetOrmasDal(), errorMessage))
			{
				caRelation->Clear();
				caRelation->SetAccountID(account->GetID());
				caRelation->SetCompanyID(companyCmb->currentData().toInt());
				if (!caRelation->CreateCompanyAccountRelation(dialogBL->GetOrmasDal(), errorMessage))
				{
					QMessageBox::information(NULL, QString(tr("Info")),
						QString(tr("Cannot create relation with generated account and company! Please contact with Administrator!")),
						QString(tr("Ok")));
					Close();
				}
			}
			else
			{
				QMessageBox::information(NULL, QString(tr("Info")),
					QString(tr("Cannot generate account! Please contact with Administrator!")),
					QString(tr("Ok")));
				Close();
			}

			progressDialog.setValue(i);
			i++;

			number = "";
			genAccRawNumber = "";
		}

		QMessageBox::information(NULL, QString(tr("Info")),
			QString(tr("All accounts were successfully generated!")),
			QString(tr("Ok")));
		Close();
	}
}

void GenerateAcc::Close()
{
	this->close();
}

void CreateRtrnDlg::InitComboBox()
{
	std::vector<BusinessLayer::Currency> curVector = dialogBL->GetAllDataForClass<BusinessLayer::Currency>(errorMessage);
	if (!curVector.empty())
	{
		for (unsigned int i = 0; i < curVector.size(); i++)
		{
			currencyCmb->addItem(curVector[i].GetShortName().c_str(), QVariant(curVector[i].GetID()));
		}
	}

	std::vector<BusinessLayer::Company> comVector = dialogBL->GetAllDataForClass<BusinessLayer::Company>(errorMessage);
	if (!comVector.empty())
	{
		for (unsigned int i = 0; i < comVector.size(); i++)
		{
			currencyCmb->addItem(comVector[i].GetName().c_str(), QVariant(comVector[i].GetID()));
		}
	}
}