/// <summary> Datebase Access Layer </summary>
///
/// <remarks>
/// Developers: Daler Muhamedov
/// 
/// Copyright (c) 2016 ORION MANAGEMENT & ANALYSIS SYSTEM Company. All rights reserved.
/// </remarks>

#include "stdafx.h"
#include "OrmasDAL.h"
#include <boost/lexical_cast.hpp> 


namespace DataLayer{

	OrmasDal::OrmasDal(ConnectionString connection, PGconn *dbConnection)
	{
		this->connectionString = connection;
		this->dbConnection = dbConnection;
	}

	OrmasDal::OrmasDal(const OrmasDal &ormasDAL)
	{
		dbConnection = ormasDAL.dbConnection; connectionString = ormasDAL.connectionString;
	}

	OrmasDal::OrmasDal()
	{

	}
	OrmasDal::~OrmasDal()
	{

	}

	PGconn* OrmasDal::GetConnection()
	{
		if (PQstatus(dbConnection) != CONNECTION_OK)
		{
			PQfinish(dbConnection);
			return nullptr;
		}
		return dbConnection;
	}

	bool OrmasDal::ConnectToDB(std::string dbname, std::string username, std::string password, std::string host, int port)
	{
		SetDBParams(dbname, username, password, host, port);
		dbConnection = PQconnectdb(connectionString.GetConString().c_str());
		if (PQstatus(dbConnection) != CONNECTION_OK)
		{
			PQfinish(dbConnection);
			return false;
		}
		return true;
	}

	void OrmasDal::InitFromConfigFile(std::string path)
	{
		connectionString.InitFromConfigFile(path);
	}

	void OrmasDal::SetDBParams(std::string dbname, std::string username, std::string password, std::string host, int port)
	{
		connectionString.SetDBParams(dbname, username, password, host, port);
	}

	int OrmasDal::GenerateID()
	{
		int id = 0;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
			return id; // return id as 0, just use 0 for logical statements in your functions, for example for canceling some actions
		PGresult * result;
		result = PQexec(dbConnection, "SELECT nextval('\"OrmasSchema\".id_seq');");
		if (PQresultStatus(result) == PGRES_TUPLES_OK)
		{
			if (PQntuples(result) > 0)
			{
				id = std::stoi(PQgetvalue(result, 0, 0) == "" ? 0 : PQgetvalue(result, 0, 0));
				PQclear(result);
				return id;
			}
			PQclear(result);
			return id;
		}
		PQclear(result);
		return id;
	}

	int OrmasDal::GenerateAccountID()
	{
		int id = 0;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
			return id; // return id as 0, just use 0 for logical statements in your functions, for example for canceling some actions
		PGresult * result;
		result = PQexec(dbConnection, "SELECT nextval('\"OrmasSchema\".acc_seq');");
		if (PQresultStatus(result) == PGRES_TUPLES_OK)
		{
			if (PQntuples(result) > 0)
			{
				id = std::stoi(PQgetvalue(result, 0, 0) == "" ? 0 : PQgetvalue(result, 0, 0));
				PQclear(result);
				return id;
			}
			PQclear(result);
			return id;
		}
		PQclear(result);
		return id;
	}
	
	std::string OrmasDal::GetSystemDateTime()
	{
		std::string currentDate = "";
		if (PQstatus(dbConnection) == CONNECTION_BAD)
			return currentDate;
		PGresult * result;
		result = PQexec(dbConnection, "SELECT to_char(now()::timestamp(0), 'dd.MM.yyyy hh24:mm')");
		if (PQresultStatus(result) == PGRES_TUPLES_OK)
		{
			if (PQntuples(result) > 0)
			{
				currentDate = PQgetvalue(result, 0, 0);
				PQclear(result);
				return currentDate;
			}
			PQclear(result);
			return currentDate;
		}
		PQclear(result);
		return currentDate;
	}

	std::string OrmasDal::GetSystemDate()
	{
		std::string currentDate = "";
		if (PQstatus(dbConnection) == CONNECTION_BAD)
			return currentDate;
		PGresult * result;
		result = PQexec(dbConnection, "SELECT to_char(DATE(now()::timestamp(0)), 'dd.MM.yyyy')");
		if (PQresultStatus(result) == PGRES_TUPLES_OK)
		{
			if (PQntuples(result) > 0)
			{
				currentDate = PQgetvalue(result, 0, 0);
				PQclear(result);
				return currentDate;
			}
			PQclear(result);
			return currentDate;
		}
		PQclear(result);
		return currentDate;
	}

	//transaction functions
	bool OrmasDal::StartTransaction(std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;

		//start transaction
		std::string sqlCommand = "BEGIN;";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			return false;
		}
		PQclear(result);
		return true;
	}
	
	bool OrmasDal::CommitTransaction(std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;

		//start transaction
		std::string sqlCommand = "COMMIT;";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			return false;
		}
		PQclear(result);
		return true;
	}
	
	bool OrmasDal::CancelTransaction(std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;

		//start transaction
		std::string sqlCommand = "ROLLBACK;";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			return false;
		}
		PQclear(result);
		return true;
	}
	
	// View section --------------------------------------------------------------------------------------
	//Get access items from DB
	std::vector<accessItemsCollection> OrmasDal::GetAccessItems(std::string& errorMessage, std::string filter)
	{
		accessItemsCollection rowTuple;
		std::vector<accessItemsCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".access_items_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY access_item_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int accessItemID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string itemEng = PQgetvalue(result, i, 1);
						std::string itemRu = PQgetvalue(result, i, 2);
						std::string division = PQgetvalue(result, i, 3);
						rowTuple = std::make_tuple(accessItemID, itemEng, itemRu, division);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for access items, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all access rights from DB
	std::vector<accessesViewCollection> OrmasDal::GetAccesses(std::string& errorMessage, std::string filter)
	{
		accessesViewCollection rowTuple;
		std::vector<accessesViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".accesses_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY access_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{

						int accessID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string roleName = PQgetvalue(result, i, 1);
						std::string itemEng = PQgetvalue(result, i, 2);
						std::string itemRu = PQgetvalue(result, i, 3);
						std::string division = PQgetvalue(result, i, 4);						
						int roleID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						int accessItemID = std::stoi(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						rowTuple = std::make_tuple(accessID, roleName, itemEng, itemRu, division, roleID, accessItemID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for access, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	
	// Get all account types from DB
	std::vector<accountTypeCollection> OrmasDal::GetAccountType(std::string& errorMessage, std::string filter)
	{
		accountTypeCollection rowTuple;
		std::vector<accountTypeCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".account_type_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY account_type_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{

						int accountTypeID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string accountTypeName = PQgetvalue(result, i, 1);
						int accountTypeNumber = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string comment = PQgetvalue(result, i, 3);
						rowTuple = std::make_tuple(accountTypeID, accountTypeName, accountTypeNumber, comment);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for account type, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all accounts from DB
	std::vector<accountsCollection> OrmasDal::GetAccounts(std::string& errorMessage, std::string filter)
	{
		accountsCollection rowTuple;
		std::vector<accountsCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".accounts_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY account_number ASC;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int accountID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string accountNumber = PQgetvalue(result, i, 1);
						double startBalance = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						double currentBalance = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						rowTuple = std::make_tuple(accountID, accountNumber, startBalance, currentBalance);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for accounts, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all account history from DB
	std::vector<accountHistoryCollection> OrmasDal::GetAccountHistory(std::string& errorMessage, std::string filter)
	{
		accountHistoryCollection rowTuple;
		std::vector<accountHistoryCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".account_history_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY account_history_id ASC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int hAID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int accountID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string accountNumber = PQgetvalue(result, i, 2);
						double startBalance = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						double currentBalance = std::stod(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						std::string fromDate = PQgetvalue(result, i, 5);
						std::string tillDate = PQgetvalue(result, i, 6);
						rowTuple = std::make_tuple(hAID, accountID, accountNumber, startBalance, currentBalance, fromDate, tillDate);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for account history, please contact with appliction provider!";
			}
		}
		return resultVector;
	}
	
	// Balance-Payment relation
	std::vector<balancePaymentCollection> OrmasDal::GetBalancePayment(std::string& errorMessage, std::string filter)
	{
		balancePaymentCollection rowTuple;
		std::vector<balancePaymentCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".balance_payment_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY balance_payment_id;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int balancePaymentID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int balanceID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int paymentID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						rowTuple = std::make_tuple(balancePaymentID, balanceID, paymentID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for balance-payment relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}


	// Balance-Payslip relation
	std::vector<balancePayslipCollection> OrmasDal::GetBalancePayslip(std::string& errorMessage, std::string filter)
	{
		balancePayslipCollection rowTuple;
		std::vector<balancePayslipCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".balance_payslip_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY balance_payslip_id;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int balancePaysliptID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int balanceID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int payslipID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						rowTuple = std::make_tuple(balancePaysliptID, balanceID, payslipID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for balance-payslip relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Balance-Refund relation
	std::vector<balanceRefundCollection> OrmasDal::GetBalanceRefund(std::string& errorMessage, std::string filter)
	{
		balanceRefundCollection rowTuple;
		std::vector<balanceRefundCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".balance_refund_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY balance_refund_id;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int balanceRefundID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int balanceID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int refundID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						rowTuple = std::make_tuple(balanceRefundID, balanceID, refundID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for balance-refund relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Balance-Withdrawal relation
	std::vector<balanceWithdrawalCollection> OrmasDal::GetBalanceWithdrawal(std::string& errorMessage, std::string filter)
	{
		balanceWithdrawalCollection rowTuple;
		std::vector<balanceWithdrawalCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".balance_withdrawal_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY balance_withdrawal_id;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int balanceWithdrawalID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int balanceID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int withdrawalID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						rowTuple = std::make_tuple(balanceWithdrawalID, balanceID, withdrawalID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for balance-withdrawal relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all balances from DB
	std::vector<balancesViewCollection> OrmasDal::GetBalances(std::string& errorMessage, std::string filter)
	{
		balancesViewCollection rowTuple;
		std::vector<balancesViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".balances_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY balance_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{

						int balanceID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string clientName = PQgetvalue(result, i, 1);
						std::string clientSurname = PQgetvalue(result, i, 2);
						double balanceValue = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = PQgetvalue(result, i, 4);
						int userID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						int accountID = std::stoi(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						rowTuple = std::make_tuple(balanceID, clientName, clientSurname, balanceValue, currencyName, userID, accountID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for balance, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get chart of account from DB
	std::vector<chartOfAccountsViewCollection> OrmasDal::GetChartOfAccounts(std::string& errorMessage, std::string filter)
	{
		chartOfAccountsViewCollection rowTuple;
		std::vector<chartOfAccountsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".chart_of_accounts_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY number_of_account ASC;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int chartOfAccountID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string numberOfAccount = PQgetvalue(result, i, 1);
						std::string accountName = PQgetvalue(result, i, 2);
						std::string accountTypeName = PQgetvalue(result, i, 3);
						int accountTypeID = std::stoi(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						rowTuple = std::make_tuple(chartOfAccountID, numberOfAccount, accountName, accountTypeName, accountTypeID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for chart of accounts, please contact with appliction provider!";
			}
		}
		return resultVector;
	}
	
	// Get all clients from DB
	std::vector<clientsViewCollection> OrmasDal::GetClients(std::string& errorMessage, std::string filter)
	{
		clientsViewCollection rowTuple;
		std::vector<clientsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".clients_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY user_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{

						int userID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string name = PQgetvalue(result, i, 1);
						std::string surname = PQgetvalue(result, i, 2);
						std::string phone = PQgetvalue(result, i, 3);
						std::string country = PQgetvalue(result, i, 4);
						std::string region = PQgetvalue(result, i, 5);
						std::string city = PQgetvalue(result, i, 6);
						std::string address = PQgetvalue(result, i, 7);
						std::string firm = PQgetvalue(result, i, 8);
						std::string firmNumber = PQgetvalue(result, i, 9);
						std::string roleName = PQgetvalue(result, i, 10);
						std::string password = PQgetvalue(result, i, 11);
						std::string email = PQgetvalue(result, i, 12);
						std::string actStr = (PQgetvalue(result, i, 13));
						bool activated = (actStr.compare("t") == 0 ? true : false);
						int roleID = std::stoi(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						int locationID = std::stoi(std::string(PQgetvalue(result, i, 15)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 15)));
						rowTuple = std::make_tuple(userID, name, surname, phone, country, region, city, address, firm, firmNumber, 
							roleName, password, email, activated, roleID, locationID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for clients, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all companies from DB
	std::vector<companiesCollection> OrmasDal::GetCompanies(std::string& errorMessage, std::string filter)
	{
		companiesCollection rowTuple;
		std::vector<companiesCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".companies_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY company_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						
						int companyID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string companyName = PQgetvalue(result, i, 1);
						std::string companyAddress = PQgetvalue(result, i, 2);
						std::string companyPhone = PQgetvalue(result, i, 3);
						std::string comment = PQgetvalue(result, i, 4);
						rowTuple = std::make_tuple(companyID, companyName, companyAddress, companyPhone, comment);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for company, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all company-account link from DB
	std::vector<companyAccountViewCollection> OrmasDal::GetCompanyAccount(std::string& errorMessage, std::string filter)
	{
		companyAccountViewCollection rowTuple;
		std::vector<companyAccountViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".company_account_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY company_account_id;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int id = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string companyName = PQgetvalue(result, i, 1);
						std::string accountNumber = PQgetvalue(result, i, 2);
						int companyID = std::stoi(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						int accountID = std::stoi(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						rowTuple = std::make_tuple(id, companyName, accountNumber, companyID, accountID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for company-account link, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all company employee
	std::vector<companyEmployeeViewCollection> OrmasDal::GetCompanyEmployee(std::string& errorMessage, std::string filter)
	{
		companyEmployeeViewCollection rowTuple;
		std::vector<companyEmployeeViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".company_employee_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY company_employee_id;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int companyEmployeeID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string companyName = PQgetvalue(result, i, 1);
						std::string employeeName = PQgetvalue(result, i, 2);
						std::string employeeSurname = PQgetvalue(result, i, 3);
						std::string employeePhone = PQgetvalue(result, i, 4);
						int companyID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						rowTuple = std::make_tuple(companyEmployeeID, companyName, employeeName, employeeSurname, employeePhone, 
							companyID, employeeID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for company-employee relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get consume product list
	std::vector<consumeProductListViewCollection> OrmasDal::GetConsumeProductList(std::string& errorMessage, std::string filter)
	{
		consumeProductListViewCollection rowTuple;
		std::vector<consumeProductListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".consume_product_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY consume_product_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int clID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int consumeProductID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(clID, consumeProductID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for consume product list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get consume products
	std::vector<consumeProductsViewCollection> OrmasDal::GetConsumeProducts(std::string& errorMessage, std::string filter)
	{
		consumeProductsViewCollection rowTuple;
		std::vector<consumeProductsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".consume_products_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY consume_product_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int consumeProductID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string consumeProductDate = PQgetvalue(result, i, 1);
						std::string consumeProductExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePosition = PQgetvalue(result, i, 8);
						std::string stockEmployeeName = PQgetvalue(result, i, 9);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 10);
						std::string stockEmployeePhone = PQgetvalue(result, i, 11);
						std::string stockEmployeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(consumeProductID, consumeProductDate, consumeProductExecutionDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName, 
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for consume product, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get consume raw list
	std::vector<consumeRawListViewCollection> OrmasDal::GetConsumeRawList(std::string& errorMessage, std::string filter)
	{
		consumeRawListViewCollection rowTuple;
		std::vector<consumeRawListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".consume_raw_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY consume_raw_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int clID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int consumeRawID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(clID, consumeRawID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for consume raw list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get consume raws
	std::vector<consumeRawsViewCollection> OrmasDal::GetConsumeRaws(std::string& errorMessage, std::string filter)
	{
		consumeRawsViewCollection rowTuple;
		std::vector<consumeRawsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".consume_raws_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY consume_raw_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int consumeRawID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string consumeRawDate = PQgetvalue(result, i, 1);
						std::string consumeRawExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePosition = PQgetvalue(result, i, 8);
						std::string stockEmployeeName = PQgetvalue(result, i, 9);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 10);
						std::string stockEmployeePhone = PQgetvalue(result, i, 11);
						std::string stockEmployeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(consumeRawID, consumeRawDate, consumeRawExecutionDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for consume raw, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get all currencies
	std::vector<currenciesCollection> OrmasDal::GetCurrencies(std::string& errorMessage, std::string filter)
	{
		currenciesCollection rowTuple;
		std::vector<currenciesCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".currencies_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY currency_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int currencyCode = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string currencyShortName = PQgetvalue(result, i, 2);
						std::string currencyName = PQgetvalue(result, i, 3);
						int currencyUnit = std::stoi(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						std::string cMT = (PQgetvalue(result, i, 5));
						bool currencyMainTrade = (cMT.compare("t") == 0 ? true : false);
						rowTuple = std::make_tuple(currencyID, currencyCode, currencyShortName, currencyName, currencyUnit, currencyMainTrade);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for currency, please contact with appliction provider!";
			}
		}
		return resultVector;
	}
	
	//Get all division-account relation
	std::vector<divisionAccountViewCollection> OrmasDal::GetDivisionAccount(std::string& errorMessage, std::string filter)
	{
		divisionAccountViewCollection rowTuple;
		std::vector<divisionAccountViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".division_account_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY division_account_id;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int divisionAccountID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string divisionName = PQgetvalue(result, i, 1);
						std::string accountNumber = PQgetvalue(result, i, 2);
						std::string accountName = PQgetvalue(result, i, 3);
						std::string accountCode = PQgetvalue(result, i, 4);
						int divisionID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						int accountID = std::stoi(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						rowTuple = std::make_tuple(divisionAccountID, divisionName, accountNumber, accountName, accountCode, divisionID, accountID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for division-account relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get all division-employee relation
	std::vector<divisionEmployeeCollection> OrmasDal::GetDivisionEmployee(std::string& errorMessage, std::string filter)
	{
		divisionEmployeeCollection rowTuple;
		std::vector<divisionEmployeeCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".division_employee_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY division_employee_id;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int divisionEmployeeID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int divisionID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string contractStr = PQgetvalue(result, i, 3);
						bool isContract = (contractStr.compare("t") == 0 ? true : false);
						rowTuple = std::make_tuple(divisionEmployeeID, divisionID, employeeID, isContract);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for division-employee relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get all divisions
	std::vector<divisionsCollection> OrmasDal::GetDivisions(std::string& errorMessage, std::string filter)
	{
		divisionsCollection rowTuple;
		std::vector<divisionsCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".divisions_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY division_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int divisionID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string divisionName = PQgetvalue(result, i, 1);
						std::string divisionCode = PQgetvalue(result, i, 2);
						rowTuple = std::make_tuple(divisionID, divisionName, divisionCode);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for division, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all employees from DB
	std::vector<employeesViewCollection> OrmasDal::GetEmployees(std::string& errorMessage, std::string filter)
	{
		employeesViewCollection rowTuple;
		std::vector<employeesViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".employees_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY user_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int userID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string name = PQgetvalue(result, i, 1);
						std::string surname = PQgetvalue(result, i, 2);
						std::string positionName = PQgetvalue(result, i, 3);
						std::string phone = PQgetvalue(result, i, 4);
						std::string address = PQgetvalue(result, i, 5);
						std::string birthDate = PQgetvalue(result, i, 6);
						std::string roleName = PQgetvalue(result, i, 7);
						std::string hireDate = PQgetvalue(result, i, 8);
						std::string password = PQgetvalue(result, i, 9);
						std::string email = PQgetvalue(result, i, 10);
						std::string actStr = (PQgetvalue(result, i, 11));
						bool activated = (actStr.compare("t") == 0 ? true : false);
						int roleID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int positionID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						int divisionEmployeeID = std::stoi(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						int divisionID = std::stoi(std::string(PQgetvalue(result, i, 15)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 15)));
						std::string contractStr = (PQgetvalue(result, i, 16));
						bool isContract = (contractStr.compare("t") == 0 ? true : false);
						rowTuple = std::make_tuple(userID, name, surname, positionName, phone, address, birthDate, roleName, hireDate,
							password, email, activated, roleID, positionID, divisionEmployeeID, divisionID, isContract);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for employee, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all entries from DB
	std::vector<entriesViewCollection> OrmasDal::GetEntries(std::string& errorMessage, std::string filter)
	{
		entriesViewCollection rowTuple;
		std::vector<entriesViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".entries_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY entry_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int entryID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string entryDate = PQgetvalue(result, i, 1);
						std::string debitingAccountNumber = PQgetvalue(result, i, 2);
						double value = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string creditingAccountNumber = PQgetvalue(result, i, 4);
						int debitingAccountID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						int creditingAccountID = std::stoi(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						std::string description = PQgetvalue(result, i, 7);
						rowTuple = std::make_tuple(entryID, entryDate, debitingAccountNumber, value, creditingAccountNumber, 
							debitingAccountID, creditingAccountID, description);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for entries, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get entry routing
	std::vector<entryRoutingCollection> OrmasDal::GetEntryRouting(std::string& errorMessage, std::string filter)
	{
		entryRoutingCollection rowTuple;
		std::vector<entryRoutingCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".entry_routing_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY entry_routing_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int entryRoutingID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string operation = PQgetvalue(result, i, 1);
						int debit = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						int credit = std::stoi(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						rowTuple = std::make_tuple(entryRoutingID, operation, debit, credit);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for entry routing, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all entry-subaccount link from DB
	std::vector<entrySubaccountCollection> OrmasDal::GetEntrySubaccount(std::string& errorMessage, std::string filter)
	{
		entrySubaccountCollection rowTuple;
		std::vector<entrySubaccountCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".entry_subaccount_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY entry_subaccount_id;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int entrySubaccountID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int entryID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int subaccountID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						rowTuple = std::make_tuple(entrySubaccountID, entryID, subaccountID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for entry-subaccount link, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all financial report
	std::vector<financialReportCollection> OrmasDal::GetFinancialReport(std::string& errorMessage, std::string filter)
	{
		financialReportCollection rowTuple;
		std::vector<financialReportCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".financial_report_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY financial_report_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());
			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int financialReportID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						double acc_44010 = std::stod(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						double acc_55010 = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						double acc_552 = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						double acc_55270 = std::stod(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						double acc_553 = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						double acc_55321 = std::stod(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						double acc_44020_90 = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double acc_66010_66110 = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						double acc_66020_66120 = std::stod(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						double acc_66040_66140 = std::stod(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						double acc_66050_66150 = std::stod(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						double acc_66060_66160 = std::stod(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						double acc_66130 = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double acc_66070_66170 = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						double tax = std::stod(std::string(PQgetvalue(result, i, 15)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 15)));
						std::string fromDate = PQgetvalue(result, i, 16);
						std::string tillDate = PQgetvalue(result, i, 17);
						rowTuple = std::make_tuple(financialReportID, acc_44010, acc_55010, acc_552, acc_55270, acc_553, acc_55321,
							acc_44020_90, acc_66010_66110, acc_66020_66120, acc_66040_66140, acc_66050_66150, acc_66060_66160,
							acc_66130, acc_66070_66170, tax, fromDate, tillDate);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for entry-subaccount link, please contact with appliction provider!";
			}
		}
		return resultVector;
	}


	//Get inventorization list
	std::vector<inventorizationListViewCollection> OrmasDal::GetInventorizationList(std::string& errorMessage, std::string filter)
	{
		inventorizationListViewCollection rowTuple;
		std::vector<inventorizationListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".inventorization_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY inventorization_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int ilID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int inventorizationID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(ilID, inventorizationID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for inventorization list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get inventorizations
	std::vector<inventorizationsViewCollection> OrmasDal::GetInventorizations(std::string& errorMessage, std::string filter)
	{
		inventorizationsViewCollection rowTuple;
		std::vector<inventorizationsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".inventorizations_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY inventorization_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int inventorizationID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string inventorizationDate = PQgetvalue(result, i, 1);
						std::string inventorizationExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePosition = PQgetvalue(result, i, 8);
						std::string stockEmployeeName = PQgetvalue(result, i, 9);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 10);
						std::string stockEmployeePhone = PQgetvalue(result, i, 11);
						std::string stockEmployeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(inventorizationID, inventorizationDate, inventorizationExecutionDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for inventorization, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get jobprice
	std::vector<jobpriceViewCollection> OrmasDal::GetJobprice(std::string& errorMessage, std::string filter)
	{
		jobpriceViewCollection rowTuple;
		std::vector<jobpriceViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".jobprice_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY jobprice_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int jobpriceID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string productName = PQgetvalue(result, i, 1);
						double value = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string currencyName = PQgetvalue(result, i, 3);
						double volume = std::stod(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						std::string measureName = PQgetvalue(result, i, 5);
						std::string positionName = PQgetvalue(result, i, 6);
						int productID = std::stoi(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						int measureID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						int positionID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						rowTuple = std::make_tuple(jobpriceID, productName, value, currencyName, volume, measureName, positionName, productID, 
							currencyID, measureID, positionID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for jobprice, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get jobsheet
	std::vector<jobsheetViewCollection> OrmasDal::GetJobsheet(std::string& errorMessage, std::string filter)
	{
		jobsheetViewCollection rowTuple;
		std::vector<jobsheetViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".jobsheet_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY jobsheet_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int jobsheetID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string date = PQgetvalue(result, i, 1);
						std::string employeeName = PQgetvalue(result, i, 2);
						std::string employeeSurname = PQgetvalue(result, i, 3);
						std::string employeePhone = PQgetvalue(result, i, 4);
						std::string productName = PQgetvalue(result, i, 5);
						double count = std::stod(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						std::string measureName = PQgetvalue(result, i, 7);
						int productID = std::stoi(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						rowTuple = std::make_tuple(jobsheetID, date, employeeName, employeeSurname, employeePhone, productName, count, measureName, 
							productID, employeeID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for jobsheet, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get Locations
	std::vector<locationsCollection> OrmasDal::GetLocations(std::string& errorMessage, std::string filter)
	{
		locationsCollection rowTuple;
		std::vector<locationsCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".locations_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY location_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int locationID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string countryName = PQgetvalue(result, i, 1);
						std::string countryCode = PQgetvalue(result, i, 2);
						std::string regionName = PQgetvalue(result, i, 3);
						std::string cityName = PQgetvalue(result, i, 4);
						rowTuple = std::make_tuple(locationID, countryName, countryCode, regionName, cityName);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for location, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get measures
	std::vector<measuresCollection> OrmasDal::GetMeasures(std::string& errorMessage, std::string filter)
	{
		measuresCollection rowTuple;
		std::vector<measuresCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".measures_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY measure_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int measureID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string measureName = PQgetvalue(result, i, 1);
						std::string measureShortName = PQgetvalue(result, i, 2);
						std::string mUnit = std::string(PQgetvalue(result, i, 3));
						int measureUnit = std::stoi(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						rowTuple = std::make_tuple(measureID, measureName, measureShortName, measureUnit);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for measure, please contact with appliction provider!";
			}
		}
		return resultVector;
	}
	
	// Get net cost
	std::vector<netCostViewCollection> OrmasDal::GetNetCost(std::string& errorMessage, std::string filter)
	{
		netCostViewCollection rowTuple;
		std::vector<netCostViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".net_cost_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY net_cost_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int netCostID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string netCostDate = PQgetvalue(result, i, 1);
						std::string productName = PQgetvalue(result, i, 2);
						double productVolume = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string measureName = PQgetvalue(result, i, 4);
						double netCostValue = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string currencyName = PQgetvalue(result, i, 6);
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string isOutdatedStr = (PQgetvalue(result, i, 9));
						bool isOutdated = (isOutdatedStr.compare("t") == 0 ? true : false);
						rowTuple = std::make_tuple(netCostID, netCostDate, productName, productVolume, measureName, netCostValue, 
							currencyName, currencyID, productID, isOutdated);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for net cost, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get order list
	std::vector<orderListViewCollection> OrmasDal::GetOrderList(std::string& errorMessage, std::string filter)
	{
		orderListViewCollection rowTuple;
		std::vector<orderListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".order_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY order_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int olID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int orderID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(olID, orderID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for order list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get orders
	std::vector<ordersViewCollection> OrmasDal::GetOrders(std::string& errorMessage, std::string filter)
	{
		ordersViewCollection rowTuple;
		std::vector<ordersViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".orders_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY order_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int orderID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string orderDate = PQgetvalue(result, i, 1);
						std::string orderExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string clientName = PQgetvalue(result, i, 5);
						std::string clientSurname = PQgetvalue(result, i, 6);
						std::string clientPhone = PQgetvalue(result, i, 7);
						std::string clientAddress = PQgetvalue(result, i, 8);
						std::string firm = PQgetvalue(result, i, 9);
						std::string employeeName = PQgetvalue(result, i, 10);
						std::string employeeSurname = PQgetvalue(result, i, 11);
						std::string employeePhone = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(orderID, orderDate, orderExecutionDate, statusCode, statusName, clientName, clientSurname, clientPhone,
							clientAddress, firm, employeeName, employeeSurname, employeePhone, count, sum, currencyName, employeeID, 
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for order, please contact with appliction provider!";
			}
		}
		return resultVector;
	}
	
	//Get order raw list
	std::vector<orderRawListViewCollection> OrmasDal::GetOrderRawList(std::string& errorMessage, std::string filter)
	{
		orderRawListViewCollection rowTuple;
		std::vector<orderRawListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".order_raw_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY order_raw_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int olID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int orderRawID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(olID, orderRawID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for order raw list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get order raws
	std::vector<orderRawsViewCollection> OrmasDal::GetOrderRaws(std::string& errorMessage, std::string filter)
	{
		orderRawsViewCollection rowTuple;
		std::vector<orderRawsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".order_raws_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY order_raw_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int orderRawID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string orderRawDate = PQgetvalue(result, i, 1);
						std::string orderRawExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string purveyorName = PQgetvalue(result, i, 5);
						std::string purveyorSurname = PQgetvalue(result, i, 6);
						std::string purveyorPhone = PQgetvalue(result, i, 7);
						std::string purveyorCompanyName = PQgetvalue(result, i, 8);
						std::string employeeName = PQgetvalue(result, i, 9);
						std::string employeeSurname = PQgetvalue(result, i, 10);
						std::string employeePhone = PQgetvalue(result, i, 11);
						std::string employeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(orderRawID, orderRawDate, orderRawExecutionDate, statusCode,
							statusName, purveyorName, purveyorSurname, purveyorPhone, purveyorCompanyName, employeeName,
							employeeSurname, employeePhone, employeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for order raw, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get payments
	std::vector<paymentsViewCollection> OrmasDal::GetPayments(std::string& errorMessage, std::string filter)
	{
		paymentsViewCollection rowTuple;
		std::vector<paymentsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".payments_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY payment_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int paymentID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string paymentDate = PQgetvalue(result, i, 1);
						std::string userName = PQgetvalue(result, i, 2);
						std::string userSurname = PQgetvalue(result, i, 3);
						std::string userPhone = PQgetvalue(result, i, 4);
						double paymentValue = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string currencyName = PQgetvalue(result, i, 6);
						std::string statusName = PQgetvalue(result, i, 7);
						int userID = std::stoi(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						rowTuple = std::make_tuple(paymentID, paymentDate, userName, userSurname, userPhone, paymentValue, 
							currencyName, statusName, userID, currencyID, statusID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for payment, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get payment-employee
	std::vector<paymentEmployeeCollection> OrmasDal::GetPaymentEmployee(std::string& errorMessage, std::string filter)
	{
		paymentEmployeeCollection rowTuple;
		std::vector<paymentEmployeeCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".payment_employee_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY payment_employee_id;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int paymentEmployeeID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int paymentID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						rowTuple = std::make_tuple(paymentEmployeeID, paymentID, employeeID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for payment-employee, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get payslip
	std::vector<payslipsViewCollection> OrmasDal::GetPayslips(std::string& errorMessage, std::string filter)
	{
		payslipsViewCollection rowTuple;
		std::vector<payslipsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".payslips_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY payslip_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int payslipID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string payslipDate = PQgetvalue(result, i, 1);
						double payslipValue = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string currencyName = PQgetvalue(result, i, 3);
						int salaryID = std::stoi(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						rowTuple = std::make_tuple(payslipID, payslipDate, payslipValue, currencyName, salaryID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for payslip, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Payslip-Order relation
	std::vector<payslipOrderCollection> OrmasDal::GetPayslipOrder(std::string& errorMessage, std::string filter)
	{
		payslipOrderCollection rowTuple;
		std::vector<payslipOrderCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".payslip_order_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY payslip_order_id;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int payslipOrederID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int payslipID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int orderID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						rowTuple = std::make_tuple(payslipOrederID, payslipID, orderID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for payslip-order relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get percent rate
	std::vector<percentRateCollection> OrmasDal::GetPercentRate(std::string& errorMessage, std::string filter)
	{
		percentRateCollection rowTuple;
		std::vector<percentRateCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".percent_rate_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY percent_rate_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int percentRateID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						double value = std::stod(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string condition = PQgetvalue(result, i, 2);
						int positionID = std::stoi(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						rowTuple = std::make_tuple(percentRateID, value, condition, positionID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for photo, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get photos
	std::vector<photosCollection> OrmasDal::GetPhotos(std::string& errorMessage, std::string filter)
	{
		photosCollection rowTuple;
		std::vector<photosCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".photos_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY photo_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int photoID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string source = PQgetvalue(result, i, 3);
						rowTuple = std::make_tuple(photoID, userID, productID, source);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for photo, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get positions
	std::vector<positionsCollection> OrmasDal::GetPositions(std::string& errorMessage, std::string filter)
	{
		positionsCollection rowTuple;
		std::vector<positionsCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".positions_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY position_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int positionID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string positionName = PQgetvalue(result, i, 1);
						rowTuple = std::make_tuple(positionID, positionName);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for position, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get prices
	std::vector<pricesViewCollection> OrmasDal::GetPrices(std::string& errorMessage, std::string filter)
	{
		pricesViewCollection rowTuple;
		std::vector<pricesViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".prices_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY price_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int priceID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string priceDate = PQgetvalue(result, i, 1);
						std::string productName = PQgetvalue(result, i, 2);
						double productVolume = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string measureName = PQgetvalue(result, i, 4);
						double priceValue = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string currencyName = PQgetvalue(result, i, 6);
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string isOutdatedStr = (PQgetvalue(result, i, 9));
						bool isOutdated = (isOutdatedStr.compare("t") == 0 ? true : false);
						rowTuple = std::make_tuple(priceID, priceDate, productName, productVolume, measureName, priceValue, 
							currencyName, currencyID, productID, isOutdated);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for price, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get product type
	std::vector<productTypeCollection> OrmasDal::GetProductTypes(std::string& errorMessage, std::string filter)
	{
		productTypeCollection rowTuple;
		std::vector<productTypeCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".product_types_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY product_type_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int productTypeID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string productTypeName = PQgetvalue(result, i, 1);
						std::string productTypeShortName = PQgetvalue(result, i, 2);
						std::string productCode = PQgetvalue(result, i, 3);
						rowTuple = std::make_tuple(productTypeID, productTypeName, productTypeShortName, productCode);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for product type, please contact with appliction provider!";
			}
		}
		return resultVector;
	}
	
	// Get production
	std::vector<productionCollection> OrmasDal::GetProduction(std::string& errorMessage, std::string filter)
	{
		productionCollection rowTuple;
		std::vector<productionCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".production_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY production_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int productionID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string productionDate = PQgetvalue(result, i, 1);
						std::string expiryDate = PQgetvalue(result, i, 2);
						std::string sessionStart = PQgetvalue(result, i, 3);
						std::string sessionEnd = PQgetvalue(result, i, 4);
						rowTuple = std::make_tuple(productionID, productionDate, expiryDate, sessionStart, sessionEnd);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production, please contact with appliction provider!";
			}
		}
		return resultVector;
	}
	
	//Get production list
	std::vector<productionListViewCollection> OrmasDal::GetProductionList(std::string& errorMessage, std::string filter)
	{
		productionListViewCollection rowTuple;
		std::vector<productionListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".production_list_view";
			sqlCommand += filter;
			sqlCommand += "  ORDER BY production_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int plID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int productionID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(plID, productionID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get production consume raw list
	std::vector<productionConsumeRawListViewCollection> OrmasDal::GetProductionConsumeRawList(std::string& errorMessage, std::string filter)
	{
		productionConsumeRawListViewCollection rowTuple;
		std::vector<productionConsumeRawListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".production_consume_raw_list_view";
			sqlCommand += filter;
			sqlCommand += "  ORDER BY consume_raw_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int clID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int consumeRawID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(clID, consumeRawID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production consume raw list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get production consume raws
	std::vector<productionConsumeRawsViewCollection> OrmasDal::GetProductionConsumeRaws(std::string& errorMessage, std::string filter)
	{
		productionConsumeRawsViewCollection rowTuple;
		std::vector<productionConsumeRawsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".production_consume_raws_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY consume_raw_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int consumeRawID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string consumeRawDate = PQgetvalue(result, i, 1);
						std::string consumeRawExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePosition = PQgetvalue(result, i, 8);
						std::string stockEmployeeName = PQgetvalue(result, i, 9);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 10);
						std::string stockEmployeePhone = PQgetvalue(result, i, 11);
						std::string stockEmployeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(consumeRawID, consumeRawDate, consumeRawExecutionDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production consume raw, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get production plan list
	std::vector<productionPlanListViewCollection> OrmasDal::GetProductionPlanList(std::string& errorMessage, std::string filter)
	{
		productionPlanListViewCollection rowTuple;
		std::vector<productionPlanListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".production_plan_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY production_plan_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int pplID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int productionPlanID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(pplID, productionPlanID, productName, price, currencyName, volume, measureName, 
							count, sum, sumCurrencyName, statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production plan list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get production plan
	std::vector<productionPlanViewCollection> OrmasDal::GetProductionPlan(std::string& errorMessage, std::string filter)
	{
		productionPlanViewCollection rowTuple;
		std::vector<productionPlanViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".production_plan_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY production_plan_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int productionPlanID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string productionPlanDate = PQgetvalue(result, i, 1);
						double count = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = PQgetvalue(result, i, 4);
						std::string statusName = PQgetvalue(result, i, 5);
						std::string employeeName = PQgetvalue(result, i, 6);
						std::string employeeSurname = PQgetvalue(result, i, 7);
						std::string employeePhone = PQgetvalue(result, i, 8);
						std::string employeePosition = PQgetvalue(result, i, 9);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));

						rowTuple = std::make_tuple(productionPlanID, productionPlanDate, count, sum, currencyName, statusName,
							employeeName, employeeSurname, employeePhone, employeePosition, employeeID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production plan, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get production stock
	std::vector<productionStockViewCollection> OrmasDal::GetProductionStock(std::string& errorMessage, std::string filter)
	{
		productionStockViewCollection rowTuple;
		std::vector<productionStockViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".production_stock_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY production_stock_id ASC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int sID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string productName = PQgetvalue(result, i, 1);
						double price = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string currencyName = std::string(PQgetvalue(result, i, 3));
						double volume = std::stod(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						std::string measureName = std::string(PQgetvalue(result, i, 5));
						double count = std::stod(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 8));
						std::string statusName = std::string(PQgetvalue(result, i, 9));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						rowTuple = std::make_tuple(sID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production stock, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get products
	std::vector<productsViewCollection> OrmasDal::GetProducts(std::string& errorMessage, std::string filter)
	{
		productsViewCollection rowTuple;
		std::vector<productsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".products_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY product_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());
			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int productID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string productName = PQgetvalue(result, i, 1);
						double price = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string currencyName = PQgetvalue(result, i, 3);
						double volume = std::stod(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						std::string measureName = PQgetvalue(result, i, 5);
						std::string productTypeName = PQgetvalue(result, i, 6);
						int shelfLife = std::stoi(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						std::string companyName = PQgetvalue(result, i, 8);
						int companyID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						int measureID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						int productTypeID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));

						rowTuple = std::make_tuple(productID, productName, price, currencyName, volume, measureName, productTypeName, 
							shelfLife, companyName, companyID, measureID, productTypeID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for product, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all purveyors from DB
	std::vector<purveyorsViewCollection> OrmasDal::GetPurveyors(std::string& errorMessage, std::string filter)
	{
		purveyorsViewCollection rowTuple;
		std::vector<purveyorsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".purveyors_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY user_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{

						int userID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string name = PQgetvalue(result, i, 1);
						std::string surname = PQgetvalue(result, i, 2);
						std::string phone = PQgetvalue(result, i, 3);
						std::string country = PQgetvalue(result, i, 4);
						std::string region = PQgetvalue(result, i, 5);
						std::string city = PQgetvalue(result, i, 6);
						std::string address = PQgetvalue(result, i, 7);
						std::string company_name = PQgetvalue(result, i, 8);
						std::string roleName = PQgetvalue(result, i, 9);
						std::string password = PQgetvalue(result, i, 10);
						std::string email = PQgetvalue(result, i, 11);
						std::string actStr = (PQgetvalue(result, i, 12));
						bool activated = (actStr.compare("t") == 0 ? true : false);
						int roleID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						int locationID = std::stoi(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						rowTuple = std::make_tuple(userID, name, surname, phone, country, region, city, address, company_name, 
							roleName, password, email, activated, roleID, locationID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for purveyors, please contact with appliction provider!";
			}
		}
		return resultVector;
	}
	
	//Get receipt product list
	std::vector<receiptProductListViewCollection> OrmasDal::GetReceiptProductList(std::string& errorMessage, std::string filter)
	{
		receiptProductListViewCollection rowTuple;
		std::vector<receiptProductListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".receipt_product_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY receipt_product_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int rlID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int receiptProductID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(rlID, receiptProductID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for receipt product list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get receipt products
	std::vector<receiptProductsViewCollection> OrmasDal::GetReceiptProducts(std::string& errorMessage, std::string filter)
	{
		receiptProductsViewCollection rowTuple;
		std::vector<receiptProductsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".receipt_products_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY receipt_product_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int receiptProductID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string receiptProductDate = PQgetvalue(result, i, 1);
						std::string receiptProductExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePosition = PQgetvalue(result, i, 8);
						std::string stockEmployeeName = PQgetvalue(result, i, 9);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 10);
						std::string stockEmployeePhone = PQgetvalue(result, i, 11);
						std::string stockEmployeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(receiptProductID, receiptProductDate, receiptProductExecutionDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for receipt product, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get receipt raw list
	std::vector<receiptRawListViewCollection> OrmasDal::GetReceiptRawList(std::string& errorMessage, std::string filter)
	{
		receiptRawListViewCollection rowTuple;
		std::vector<receiptRawListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".receipt_raw_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY receipt_raw_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int rlID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int receiptRawID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(rlID, receiptRawID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for receipt raw list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get receipt raws
	std::vector<receiptRawsViewCollection> OrmasDal::GetReceiptRaws(std::string& errorMessage, std::string filter)
	{
		receiptRawsViewCollection rowTuple;
		std::vector<receiptRawsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".receipt_raws_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY receipt_raw_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int receiptRawID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string receiptRawDate = PQgetvalue(result, i, 1);
						std::string receiptRawExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePosition = PQgetvalue(result, i, 8);
						std::string stockEmployeeName = PQgetvalue(result, i, 9);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 10);
						std::string stockEmployeePhone = PQgetvalue(result, i, 11);
						std::string stockEmployeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(receiptRawID, receiptRawDate, receiptRawExecutionDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for receipt raw, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get refunds
	std::vector<refundsViewCollection> OrmasDal::GetRefunds(std::string& errorMessage, std::string filter)
	{
		refundsViewCollection rowTuple;
		std::vector<refundsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".refunds_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY refund_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int refundID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string refundDate = PQgetvalue(result, i, 1);
						double refundValue = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string currencyName = PQgetvalue(result, i, 3);
						int userID = std::stoi(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						rowTuple = std::make_tuple(refundID, refundDate, refundValue, currencyName, userID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for refunds, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get relation type
	std::vector<relationTypeCollection> OrmasDal::GetRelationType(std::string& errorMessage, std::string filter)
	{
		relationTypeCollection rowTuple;
		std::vector<relationTypeCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".relation_type_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY relation_type_id DESC  LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());
			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int relationTypeID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string relationTypeName = PQgetvalue(result, i, 1);
						std::string comment = PQgetvalue(result, i, 2);
						rowTuple = std::make_tuple(relationTypeID, relationTypeName, comment);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for relation type, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get relations
	std::vector<relationsViewCollection> OrmasDal::GetRelations(std::string& errorMessage, std::string filter)
	{
		relationsViewCollection rowTuple;
		std::vector<relationsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".relations_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY user_id_1 ASC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());
			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int relationID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string employeeName = std::string(PQgetvalue(result, i, 1));
						std::string employeeSurname = std::string(PQgetvalue(result, i, 2));
						std::string employeePhone = std::string(PQgetvalue(result, i, 3));
						std::string relationName = std::string(PQgetvalue(result, i, 4));
						std::string clientName = std::string(PQgetvalue(result, i, 5));
						std::string clientSurname = std::string(PQgetvalue(result, i, 6));
						std::string clientPhone = std::string(PQgetvalue(result, i, 7));
						int userID1 = std::stoi(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						int userID2 = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						int relationTypeID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						
						rowTuple = std::make_tuple(relationID, employeeName, employeeSurname, employeePhone, relationName, clientName,
							clientSurname, clientPhone, userID1, userID2, relationTypeID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	
	//Get return list
	std::vector<returnListViewCollection> OrmasDal::GetReturnList(std::string& errorMessage, std::string filter)
	{
		returnListViewCollection rowTuple;
		std::vector<returnListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".return_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY return_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int rlID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int returnID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(rlID, returnID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for return list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get returns 
	std::vector<returnsViewCollection> OrmasDal::GetReturns(std::string& errorMessage, std::string filter)
	{
		returnsViewCollection rowTuple;
		std::vector<returnsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".returns_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY return_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int returnID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string returnDate = PQgetvalue(result, i, 1);
						std::string returnExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string clientName = PQgetvalue(result, i, 5);
						std::string clientSurname = PQgetvalue(result, i, 6);
						std::string clientPhone = PQgetvalue(result, i, 7);
						std::string clientAddress = PQgetvalue(result, i, 8);
						std::string firm = PQgetvalue(result, i, 9);
						std::string employeeName = PQgetvalue(result, i, 10);
						std::string employeeSurname = PQgetvalue(result, i, 11);
						std::string employeePhone = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(returnID, returnDate, returnExecutionDate, statusCode, statusName, clientName, clientSurname,
							clientPhone, clientAddress, firm, employeeName, employeeSurname, employeePhone, count, sum, currencyName,
							employeeID, userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for return, please contact with appliction provider!";
			}
		}
		return resultVector;
	}
	
	// Get roles
	std::vector<rolesCollection> OrmasDal::GetRoles(std::string& errorMessage, std::string filter)
	{
		rolesCollection rowTuple;
		std::vector<rolesCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".roles_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY role_id ASC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int roleID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string roleCode = PQgetvalue(result, i, 1);
						std::string roleName = PQgetvalue(result, i, 2);
						std::string comment = PQgetvalue(result, i, 3);
						rowTuple = std::make_tuple(roleID, roleCode, roleName, comment);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for role, please contact with appliction provider!";
			}
		}
		return resultVector;
	}
	
	// Get all salaries from DB
	std::vector<salariesViewCollection> OrmasDal::GetSalaries(std::string& errorMessage, std::string filter)
	{
		salariesViewCollection rowTuple;
		std::vector<salariesViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".salaries_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY salary_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int salaryID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string userName = PQgetvalue(result, i, 2);
						std::string userSurname = PQgetvalue(result, i, 3);
						std::string userPhone = PQgetvalue(result, i, 4);
						std::string salaryDate = PQgetvalue(result, i, 5);
						double salaryValue = std::stod(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						std::string currencyName = PQgetvalue(result, i, 7);
						std::string salaryTypeName = PQgetvalue(result, i, 8);
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						int salaryTypeID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						std::string bonusStr = (PQgetvalue(result, i, 11));
						bool salaryBonus = (bonusStr.compare("t") == 0 ? true : false);
						rowTuple = std::make_tuple(salaryID, userID, userName, userSurname, userPhone, salaryDate, salaryValue, 
							currencyName, salaryTypeName, currencyID, salaryTypeID, salaryBonus);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for salary, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all salaries from DB
	std::vector<salaryTypeCollection> OrmasDal::GetSalaryType(std::string& errorMessage, std::string filter)
	{
		salaryTypeCollection rowTuple;
		std::vector<salaryTypeCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".salary_type_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY salary_type_id DESC  LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{

						int salaryTypeID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string salaryTypeCode = PQgetvalue(result, i, 1);
						std::string salaryTypeName = PQgetvalue(result, i, 2);
						rowTuple = std::make_tuple(salaryTypeID, salaryTypeCode, salaryTypeName);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for salary type, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get specification list
	std::vector<specificationListViewCollection> OrmasDal::GetSpecificationList(std::string& errorMessage, std::string filter)
	{
		specificationListViewCollection rowTuple;
		std::vector<specificationListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".specification_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY specification_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int slID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int specificationID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double count = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string measureName = std::string(PQgetvalue(result, i, 4));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						rowTuple = std::make_tuple(slID, specificationID, productName, count, measureName, productID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for specification list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get specifications 
	std::vector<specificationsViewCollection> OrmasDal::GetSpecifications(std::string& errorMessage, std::string filter)
	{
		specificationsViewCollection rowTuple;
		std::vector<specificationsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".specifications_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY specification_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int specificationID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string specificationDate = PQgetvalue(result, i, 1);
						std::string productName = PQgetvalue(result, i, 2);
						double sum = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePositionName = PQgetvalue(result, i, 8);
						int productID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));

						rowTuple = std::make_tuple(specificationID, specificationDate, productName, sum, currencyName, employeeName,
							employeeSurname, employeePhone, employeePositionName, productID, currencyID, employeeID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for return, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get spoilage list
	std::vector<spoilageListViewCollection> OrmasDal::GetSpoilageList(std::string& errorMessage, std::string filter)
	{
		spoilageListViewCollection rowTuple;
		std::vector<spoilageListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".spoilage_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY spoilage_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int slID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int spoilageID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(slID, spoilageID, productName, price, currencyName, volume, measureName,
							count, sum, sumCurrencyName, statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production plan list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get spoilage
	std::vector<spoilageViewCollection> OrmasDal::GetSpoilage(std::string& errorMessage, std::string filter)
	{
		spoilageViewCollection rowTuple;
		std::vector<spoilageViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".spoilage_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY spoilage_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int spoilageID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string spoilageDate = PQgetvalue(result, i, 1);
						double count = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = PQgetvalue(result, i, 4);
						std::string statusName = PQgetvalue(result, i, 5);
						std::string employeeName = PQgetvalue(result, i, 6);
						std::string employeeSurname = PQgetvalue(result, i, 7);
						std::string employeePhone = PQgetvalue(result, i, 8);
						std::string employeePosition = PQgetvalue(result, i, 9);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));

						rowTuple = std::make_tuple(spoilageID, spoilageDate, count, sum, currencyName, statusName,
							employeeName, employeeSurname, employeePhone, employeePosition, employeeID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production plan, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get state
	std::vector<stateCollection> OrmasDal::GetState(std::string& errorMessage, std::string filter)
	{
		stateCollection rowTuple;
		std::vector<stateCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".state_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY state_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int stateID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int universalID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string lastChange = PQgetvalue(result, i, 3);
						rowTuple = std::make_tuple(stateID, universalID, statusID, lastChange);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for status, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get status
	std::vector<statusCollection> OrmasDal::GetStatus(std::string& errorMessage, std::string filter)
	{
		statusCollection rowTuple;
		std::vector<statusCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".status_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY status_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string statusCode = PQgetvalue(result, i, 1);
						std::string statusName = PQgetvalue(result, i, 2);
						std::string comment = PQgetvalue(result, i, 3);
						rowTuple = std::make_tuple(statusID, statusCode, statusName, comment);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for status, please contact with appliction provider!";
			}
		}
		return resultVector;
	}
	
	// Get status rule
	std::vector<statusRuleViewCollection> OrmasDal::GetStatusRule(std::string& errorMessage, std::string filter)
	{
		statusRuleViewCollection rowTuple;
		std::vector<statusRuleViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".status_rule_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY status_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int stateRuleID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string operation = PQgetvalue(result, i, 1);
						std::string statusName = PQgetvalue(result, i, 2);
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						rowTuple = std::make_tuple(stateRuleID, operation, statusName, statusID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for status, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get stock
	std::vector<stockViewCollection> OrmasDal::GetStock(std::string& errorMessage, std::string filter)
	{
		stockViewCollection rowTuple;
		std::vector<stockViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".stock_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY stock_id ASC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int sID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string productName = PQgetvalue(result, i, 1);
						double price = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string currencyName = std::string(PQgetvalue(result, i, 3));
						double volume = std::stod(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						std::string measureName = std::string(PQgetvalue(result, i, 5));
						double count = std::stod(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 8));
						std::string statusName = std::string(PQgetvalue(result, i, 9));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						rowTuple = std::make_tuple(sID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for stock, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all subaccounts from DB
	std::vector<subaccountsViewCollection> OrmasDal::GetSubaccounts(std::string& errorMessage, std::string filter)
	{
		subaccountsViewCollection rowTuple;
		std::vector<subaccountsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".subaccounts_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY subaccount_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int subaccountID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int accountID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string accountNumber = PQgetvalue(result, i, 2);
						std::string subaccountNumber = PQgetvalue(result, i, 3);
						double startBalance = std::stod(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						double currentBalance = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string currencyName = PQgetvalue(result, i, 6);
						std::string statusName = PQgetvalue(result, i, 7);
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						std::string dateOpened = PQgetvalue(result, i, 10);
						std::string dateClosed = PQgetvalue(result, i, 11);
						std::string details = PQgetvalue(result, i, 11);
						rowTuple = std::make_tuple(subaccountID, accountID, accountNumber, subaccountNumber, startBalance, 
							currentBalance, currencyName, statusName, currencyID, statusID, dateOpened, dateClosed, details);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for subaccount, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all subaccounts history from DB
	std::vector<subaccountHistoryCollection> OrmasDal::GetSubaccountHistory(std::string& errorMessage, std::string filter)
	{
		subaccountHistoryCollection rowTuple;
		std::vector<subaccountHistoryCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".subaccount_history_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY subaccount_history_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int shID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int subaccountID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						double startBalance = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						double currentBalance = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string fromDate = PQgetvalue(result, i, 4);
						std::string tillDate = PQgetvalue(result, i, 5);
						rowTuple = std::make_tuple(shID, subaccountID, startBalance, currentBalance, fromDate, tillDate);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for subaccount, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all taxes from DB
	std::vector<taxesCollection> OrmasDal::GetTaxes(std::string& errorMessage, std::string filter)
	{
		taxesCollection rowTuple;
		std::vector<taxesCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".taxes_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY taxes_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int texID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string taxName = PQgetvalue(result, i, 1);
						std::string taxCode = PQgetvalue(result, i, 2);
						double fixedValue = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						int percentValue = std::stoi(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						std::string formulaValue = PQgetvalue(result, i, 5);
						rowTuple = std::make_tuple(texID, taxName, taxCode, fixedValue, percentValue, formulaValue);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for subaccount, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get timesheet
	std::vector<timesheetViewCollection> OrmasDal::GetTimesheet(std::string& errorMessage, std::string filter)
	{
		timesheetViewCollection rowTuple;
		std::vector<timesheetViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".timesheet_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY timesheet_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int tID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string userName = PQgetvalue(result, i, 1);
						std::string userSurname = PQgetvalue(result, i, 2);
						std::string userPhone = PQgetvalue(result, i, 3);
						std::string date = PQgetvalue(result, i, 4);
						double workedTime = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						int salaryID = std::stoi(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						rowTuple = std::make_tuple(tID, userName, userSurname, userPhone, date, workedTime, salaryID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for stock, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get transport list
	std::vector<transportListViewCollection> OrmasDal::GetTransportList(std::string& errorMessage, std::string filter)
	{
		transportListViewCollection rowTuple;
		std::vector<transportListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".transport_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY transport_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int tlID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int transportID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(tlID, transportID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for transport list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get transports
	std::vector<transportsViewCollection> OrmasDal::GetTransports(std::string& errorMessage, std::string filter)
	{
		transportsViewCollection rowTuple;
		std::vector<transportsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".transports_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY transport_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int transportID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string transportDate = PQgetvalue(result, i, 1);
						std::string transportExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePosition = PQgetvalue(result, i, 8);
						std::string stockEmployeeName = PQgetvalue(result, i, 9);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 10);
						std::string stockEmployeePhone = PQgetvalue(result, i, 11);
						std::string stockEmployeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(transportID, transportDate, transportExecutionDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for transports, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get user
	std::vector<usersViewCollection> OrmasDal::GetUsers(std::string& errorMessage, std::string filter)
	{
		usersViewCollection rowTuple;
		std::vector<usersViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".users_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY user_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int userID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string userEmail = PQgetvalue(result, i, 1);
						std::string userName = PQgetvalue(result, i, 2);
						std::string userSurname = PQgetvalue(result, i, 3);
						std::string userPhone = PQgetvalue(result, i, 4);
						std::string userAddress = PQgetvalue(result, i, 5);
						std::string roleName = PQgetvalue(result, i, 6);
						std::string password = PQgetvalue(result, i, 7);
						std::string actStr = (PQgetvalue(result, i, 8));
						bool activated = (actStr.compare("t") == 0 ? true : false);
						int roleID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : PQgetvalue(result, i, 9));
						rowTuple = std::make_tuple(userID, userEmail, userName, userSurname, userPhone, userAddress,
							roleName, password, activated, roleID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					PQclear(result);
					// if result of query does not contain information and have 0 row, then function return an empty vector;
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for user, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get withdrawals
	std::vector<withdrawalsViewCollection> OrmasDal::GetWithdrawals(std::string& errorMessage, std::string filter)
	{
		withdrawalsViewCollection rowTuple;
		std::vector<withdrawalsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".withdrawals_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY withdrawal_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int withdrawalID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string withdrawalDate = PQgetvalue(result, i, 1);
						double withdrawalValue = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string currencyName = PQgetvalue(result, i, 3);
						int userID = std::stoi(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						rowTuple = std::make_tuple(withdrawalID, withdrawalDate, withdrawalValue, currencyName, userID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for withdrawal, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get write off list
	std::vector<writeOffListViewCollection> OrmasDal::GetWriteOffList(std::string& errorMessage, std::string filter)
	{
		writeOffListViewCollection rowTuple;
		std::vector<writeOffListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".write_off_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY write_off_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int olID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int orderID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(olID, orderID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for order list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get write-offs
	std::vector<writeOffsViewCollection> OrmasDal::GetWriteOffs(std::string& errorMessage, std::string filter)
	{
		writeOffsViewCollection rowTuple;
		std::vector<writeOffsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".write_offs_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY write_off_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int orderID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string orderDate = PQgetvalue(result, i, 1);
						std::string statusCode = PQgetvalue(result, i, 2);
						std::string statusName = PQgetvalue(result, i, 3);
						std::string clientName = PQgetvalue(result, i, 4);
						std::string clientSurname = PQgetvalue(result, i, 5);
						std::string clientPhone = PQgetvalue(result, i, 6);
						std::string clientAddress = PQgetvalue(result, i, 7);
						std::string firm = PQgetvalue(result, i, 8);
						std::string employeeName = PQgetvalue(result, i, 9);
						std::string employeeSurname = PQgetvalue(result, i, 10);
						std::string employeePhone = PQgetvalue(result, i, 11);
						double count = std::stod(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						std::string currencyName = PQgetvalue(result, i, 14);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 15)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 15)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));

						rowTuple = std::make_tuple(orderID, orderDate, statusCode, statusName, clientName, clientSurname, clientPhone,
							clientAddress, firm, employeeName, employeeSurname, employeePhone, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for order, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get write-off raw list
	std::vector<writeOffRawListViewCollection> OrmasDal::GetWriteOffRawList(std::string& errorMessage, std::string filter)
	{
		writeOffRawListViewCollection rowTuple;
		std::vector<writeOffRawListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".write_off_raw_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY write_off_raw_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int wlID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int writeOffRawID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(wlID, writeOffRawID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for write-off raw list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get write-off raw
	std::vector<writeOffRawsViewCollection> OrmasDal::GetWriteOffRaws(std::string& errorMessage, std::string filter)
	{
		writeOffRawsViewCollection rowTuple;
		std::vector<writeOffRawsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".write_off_raws_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY write_off_raw_id DESC LIMIT 200;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int writeOffRawID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string writeOffRawDate = PQgetvalue(result, i, 1);
						std::string statusCode = PQgetvalue(result, i, 2);
						std::string statusName = PQgetvalue(result, i, 3);
						std::string employeeName = PQgetvalue(result, i, 4);
						std::string employeeSurname = PQgetvalue(result, i, 5);
						std::string employeePhone = PQgetvalue(result, i, 6);
						std::string employeePosition = PQgetvalue(result, i, 7);
						std::string stockEmployeeName = PQgetvalue(result, i, 8);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 9);
						std::string stockEmployeePhone = PQgetvalue(result, i, 10);
						std::string stockEmployeePosition = PQgetvalue(result, i, 11);
						double count = std::stod(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						std::string currencyName = PQgetvalue(result, i, 14);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 15)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 15)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));

						rowTuple = std::make_tuple(writeOffRawID, writeOffRawDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for write-off raws, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// View section (overload functions)--------------------------------------------------------------------------------------
	//Get access items from DB
	std::vector<accessItemsCollection> OrmasDal::GetAccessItems(int offset, std::string& errorMessage, std::string filter)
	{
		accessItemsCollection rowTuple;
		std::vector<accessItemsCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".access_items_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY access_item_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int accessItemID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string itemEng = PQgetvalue(result, i, 1);
						std::string itemRu = PQgetvalue(result, i, 2);
						std::string division = PQgetvalue(result, i, 3);
						rowTuple = std::make_tuple(accessItemID, itemEng, itemRu, division);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for access items, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all access rights from DB
	std::vector<accessesViewCollection> OrmasDal::GetAccesses(int offset, std::string& errorMessage, std::string filter)
	{
		accessesViewCollection rowTuple;
		std::vector<accessesViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".accesses_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY access_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{

						int accessID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string roleName = PQgetvalue(result, i, 1);
						std::string itemEng = PQgetvalue(result, i, 2);
						std::string itemRu = PQgetvalue(result, i, 3);
						std::string division = PQgetvalue(result, i, 4);
						int roleID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						int accessItemID = std::stoi(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						rowTuple = std::make_tuple(accessID, roleName, itemEng, itemRu, division, roleID, accessItemID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for access, please contact with appliction provider!";
			}
		}
		return resultVector;
	}


	// Get all account types from DB
	std::vector<accountTypeCollection> OrmasDal::GetAccountType(int offset, std::string& errorMessage, std::string filter)
	{
		accountTypeCollection rowTuple;
		std::vector<accountTypeCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".account_type_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY account_type_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{

						int accountTypeID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string accountTypeName = PQgetvalue(result, i, 1);
						int accountTypeNumber = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string comment = PQgetvalue(result, i, 3);
						rowTuple = std::make_tuple(accountTypeID, accountTypeName, accountTypeNumber, comment);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for account type, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all accounts from DB
	std::vector<accountsCollection> OrmasDal::GetAccounts(int offset, std::string& errorMessage, std::string filter)
	{
		accountsCollection rowTuple;
		std::vector<accountsCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".accounts_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY account_number ASC  LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int accountID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string accountNumber = PQgetvalue(result, i, 1);
						double startBalance = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						double currentBalance = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						rowTuple = std::make_tuple(accountID, accountNumber, startBalance, currentBalance);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for accounts, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all account history from DB
	std::vector<accountHistoryCollection> OrmasDal::GetAccountHistory(int offset, std::string& errorMessage, std::string filter)
	{
		accountHistoryCollection rowTuple;
		std::vector<accountHistoryCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".account_history_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY account_history_id ASC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int hAID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int accountID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string accountNumber = PQgetvalue(result, i, 2);
						double startBalance = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						double currentBalance = std::stod(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						std::string fromDate = PQgetvalue(result, i, 5);
						std::string tillDate = PQgetvalue(result, i, 6);
						rowTuple = std::make_tuple(hAID, accountID, accountNumber, startBalance, currentBalance, fromDate, tillDate);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for account history, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Balance-Payment relation
	std::vector<balancePaymentCollection> OrmasDal::GetBalancePayment(int offset, std::string& errorMessage, std::string filter)
	{
		balancePaymentCollection rowTuple;
		std::vector<balancePaymentCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".balance_payment_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY balance_payment_id ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int balancePaymentID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int balanceID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int paymentID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						rowTuple = std::make_tuple(balancePaymentID, balanceID, paymentID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for balance-payment relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}


	// Balance-Payslip relation
	std::vector<balancePayslipCollection> OrmasDal::GetBalancePayslip(int offset, std::string& errorMessage, std::string filter)
	{
		balancePayslipCollection rowTuple;
		std::vector<balancePayslipCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".balance_payslip_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY balance_payslip_id ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int balancePaysliptID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int balanceID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int payslipID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						rowTuple = std::make_tuple(balancePaysliptID, balanceID, payslipID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for balance-payslip relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Balance-Refund relation
	std::vector<balanceRefundCollection> OrmasDal::GetBalanceRefund(int offset, std::string& errorMessage, std::string filter)
	{
		balanceRefundCollection rowTuple;
		std::vector<balanceRefundCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".balance_refund_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY balance_refund_id ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int balanceRefundID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int balanceID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int refundID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						rowTuple = std::make_tuple(balanceRefundID, balanceID, refundID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for balance-refund relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Balance-Withdrawal relation
	std::vector<balanceWithdrawalCollection> OrmasDal::GetBalanceWithdrawal(int offset, std::string& errorMessage, std::string filter)
	{
		balanceWithdrawalCollection rowTuple;
		std::vector<balanceWithdrawalCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".balance_withdrawal_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY balance_withdrawal_id ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int balanceWithdrawalID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int balanceID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int withdrawalID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						rowTuple = std::make_tuple(balanceWithdrawalID, balanceID, withdrawalID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for balance-withdrawal relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all balances from DB
	std::vector<balancesViewCollection> OrmasDal::GetBalances(int offset, std::string& errorMessage, std::string filter)
	{
		balancesViewCollection rowTuple;
		std::vector<balancesViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".balances_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY balance_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{

						int balanceID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string clientName = PQgetvalue(result, i, 1);
						std::string clientSurname = PQgetvalue(result, i, 2);
						double balanceValue = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = PQgetvalue(result, i, 4);
						int userID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						int accountID = std::stoi(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						rowTuple = std::make_tuple(balanceID, clientName, clientSurname, balanceValue, currencyName, userID, accountID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for balance, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get chart of account from DB
	std::vector<chartOfAccountsViewCollection> OrmasDal::GetChartOfAccounts(int offset, std::string& errorMessage, std::string filter)
	{
		chartOfAccountsViewCollection rowTuple;
		std::vector<chartOfAccountsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".chart_of_accounts_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY number_of_account ASC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int chartOfAccountID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string numberOfAccount = PQgetvalue(result, i, 1);
						std::string accountName = PQgetvalue(result, i, 2);
						std::string accountTypeName = PQgetvalue(result, i, 3);
						int accountTypeID = std::stoi(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						rowTuple = std::make_tuple(chartOfAccountID, numberOfAccount, accountName, accountTypeName, accountTypeID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for chart of accounts, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all clients from DB
	std::vector<clientsViewCollection> OrmasDal::GetClients(int offset, std::string& errorMessage, std::string filter)
	{
		clientsViewCollection rowTuple;
		std::vector<clientsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".clients_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY user_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{

						int userID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string name = PQgetvalue(result, i, 1);
						std::string surname = PQgetvalue(result, i, 2);
						std::string phone = PQgetvalue(result, i, 3);
						std::string country = PQgetvalue(result, i, 4);
						std::string region = PQgetvalue(result, i, 5);
						std::string city = PQgetvalue(result, i, 6);
						std::string address = PQgetvalue(result, i, 7);
						std::string firm = PQgetvalue(result, i, 8);
						std::string firmNumber = PQgetvalue(result, i, 9);
						std::string roleName = PQgetvalue(result, i, 10);
						std::string password = PQgetvalue(result, i, 11);
						std::string email = PQgetvalue(result, i, 12);
						std::string actStr = (PQgetvalue(result, i, 13));
						bool activated = (actStr.compare("t") == 0 ? true : false);
						int roleID = std::stoi(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						int locationID = std::stoi(std::string(PQgetvalue(result, i, 15)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 15)));
						rowTuple = std::make_tuple(userID, name, surname, phone, country, region, city, address, firm, firmNumber,
							roleName, password, email, activated, roleID, locationID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for clients, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all companies from DB
	std::vector<companiesCollection> OrmasDal::GetCompanies(int offset, std::string& errorMessage, std::string filter)
	{
		companiesCollection rowTuple;
		std::vector<companiesCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".companies_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY company_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{

						int companyID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string companyName = PQgetvalue(result, i, 1);
						std::string companyAddress = PQgetvalue(result, i, 2);
						std::string companyPhone = PQgetvalue(result, i, 3);
						std::string comment = PQgetvalue(result, i, 4);
						rowTuple = std::make_tuple(companyID, companyName, companyAddress, companyPhone, comment);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for company, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all company-account link from DB
	std::vector<companyAccountViewCollection> OrmasDal::GetCompanyAccount(int offset, std::string& errorMessage, std::string filter)
	{
		companyAccountViewCollection rowTuple;
		std::vector<companyAccountViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".company_account_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY company_account_id ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int id = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string companyName = PQgetvalue(result, i, 1);
						std::string accountNumber = PQgetvalue(result, i, 2);
						int companyID = std::stoi(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						int accountID = std::stoi(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						rowTuple = std::make_tuple(id, companyName, accountNumber, companyID, accountID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for company-account link, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all company employee
	std::vector<companyEmployeeViewCollection> OrmasDal::GetCompanyEmployee(int offset, std::string& errorMessage, std::string filter)
	{
		companyEmployeeViewCollection rowTuple;
		std::vector<companyEmployeeViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".company_employee_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY company_employee_id ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int companyEmployeeID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string companyName = PQgetvalue(result, i, 1);
						std::string employeeName = PQgetvalue(result, i, 2);
						std::string employeeSurname = PQgetvalue(result, i, 3);
						std::string employeePhone = PQgetvalue(result, i, 4);
						int companyID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						rowTuple = std::make_tuple(companyEmployeeID, companyName, employeeName, employeeSurname, employeePhone,
							companyID, employeeID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for company-employee relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get consume product list
	std::vector<consumeProductListViewCollection> OrmasDal::GetConsumeProductList(int offset, std::string& errorMessage, std::string filter)
	{
		consumeProductListViewCollection rowTuple;
		std::vector<consumeProductListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".consume_product_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY consume_product_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int clID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int consumeProductID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(clID, consumeProductID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for consume product list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get consume products
	std::vector<consumeProductsViewCollection> OrmasDal::GetConsumeProducts(int offset, std::string& errorMessage, std::string filter)
	{
		consumeProductsViewCollection rowTuple;
		std::vector<consumeProductsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".consume_products_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY consume_product_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int consumeProductID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string consumeProductDate = PQgetvalue(result, i, 1);
						std::string consumeProductExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePosition = PQgetvalue(result, i, 8);
						std::string stockEmployeeName = PQgetvalue(result, i, 9);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 10);
						std::string stockEmployeePhone = PQgetvalue(result, i, 11);
						std::string stockEmployeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(consumeProductID, consumeProductDate, consumeProductExecutionDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for consume product, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get consume raw list
	std::vector<consumeRawListViewCollection> OrmasDal::GetConsumeRawList(int offset, std::string& errorMessage, std::string filter)
	{
		consumeRawListViewCollection rowTuple;
		std::vector<consumeRawListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".consume_raw_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY consume_raw_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int clID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int consumeRawID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(clID, consumeRawID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for consume raw list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get consume raws
	std::vector<consumeRawsViewCollection> OrmasDal::GetConsumeRaws(int offset, std::string& errorMessage, std::string filter)
	{
		consumeRawsViewCollection rowTuple;
		std::vector<consumeRawsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".consume_raws_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY consume_raw_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int consumeRawID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string consumeRawDate = PQgetvalue(result, i, 1);
						std::string consumeRawExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePosition = PQgetvalue(result, i, 8);
						std::string stockEmployeeName = PQgetvalue(result, i, 9);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 10);
						std::string stockEmployeePhone = PQgetvalue(result, i, 11);
						std::string stockEmployeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(consumeRawID, consumeRawDate, consumeRawExecutionDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for consume raw, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get all currencies
	std::vector<currenciesCollection> OrmasDal::GetCurrencies(int offset, std::string& errorMessage, std::string filter)
	{
		currenciesCollection rowTuple;
		std::vector<currenciesCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".currencies_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY currency_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int currencyCode = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string currencyShortName = PQgetvalue(result, i, 2);
						std::string currencyName = PQgetvalue(result, i, 3);
						int currencyUnit = std::stoi(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						std::string cMT = (PQgetvalue(result, i, 5));
						bool currencyMainTrade = (cMT.compare("t") == 0 ? true : false);
						rowTuple = std::make_tuple(currencyID, currencyCode, currencyShortName, currencyName, currencyUnit, currencyMainTrade);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for currency, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get all division-account relation
	std::vector<divisionAccountViewCollection> OrmasDal::GetDivisionAccount(int offset, std::string& errorMessage, std::string filter)
	{
		divisionAccountViewCollection rowTuple;
		std::vector<divisionAccountViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".division_account_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY division_account_id ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int divisionAccountID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string divisionName = PQgetvalue(result, i, 1);
						std::string accountNumber = PQgetvalue(result, i, 2);
						std::string accountName = PQgetvalue(result, i, 3);
						std::string accountCode = PQgetvalue(result, i, 4);
						int divisionID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						int accountID = std::stoi(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						rowTuple = std::make_tuple(divisionAccountID, divisionName, accountNumber, accountName, accountCode, divisionID, accountID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for division-account relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get all division-employee relation
	std::vector<divisionEmployeeCollection> OrmasDal::GetDivisionEmployee(int offset, std::string& errorMessage, std::string filter)
	{
		divisionEmployeeCollection rowTuple;
		std::vector<divisionEmployeeCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".division_employee_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY division_employee_id ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int divisionEmployeeID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int divisionID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string contractStr = PQgetvalue(result, i, 3);
						bool isContract = (contractStr.compare("t") == 0 ? true : false);
						rowTuple = std::make_tuple(divisionEmployeeID, divisionID, employeeID, isContract);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for division-employee relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get all divisions
	std::vector<divisionsCollection> OrmasDal::GetDivisions(int offset, std::string& errorMessage, std::string filter)
	{
		divisionsCollection rowTuple;
		std::vector<divisionsCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".divisions_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY division_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int divisionID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string divisionName = PQgetvalue(result, i, 1);
						std::string divisionCode = PQgetvalue(result, i, 2);
						rowTuple = std::make_tuple(divisionID, divisionName, divisionCode);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for division, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all employees from DB
	std::vector<employeesViewCollection> OrmasDal::GetEmployees(int offset, std::string& errorMessage, std::string filter)
	{
		employeesViewCollection rowTuple;
		std::vector<employeesViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".employees_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY user_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int userID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string name = PQgetvalue(result, i, 1);
						std::string surname = PQgetvalue(result, i, 2);
						std::string positionName = PQgetvalue(result, i, 3);
						std::string phone = PQgetvalue(result, i, 4);
						std::string address = PQgetvalue(result, i, 5);
						std::string birthDate = PQgetvalue(result, i, 6);
						std::string roleName = PQgetvalue(result, i, 7);
						std::string hireDate = PQgetvalue(result, i, 8);
						std::string password = PQgetvalue(result, i, 9);
						std::string email = PQgetvalue(result, i, 10);
						std::string actStr = (PQgetvalue(result, i, 11));
						bool activated = (actStr.compare("t") == 0 ? true : false);
						int roleID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int positionID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						int divisionEmployeeID = std::stoi(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						int divisionID = std::stoi(std::string(PQgetvalue(result, i, 15)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 15)));
						std::string contractStr = (PQgetvalue(result, i, 16));
						bool isContract = (contractStr.compare("t") == 0 ? true : false);
						rowTuple = std::make_tuple(userID, name, surname, positionName, phone, address, birthDate, roleName, hireDate,
							password, email, activated, roleID, positionID, divisionEmployeeID, divisionID, isContract);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for employee, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all entries from DB
	std::vector<entriesViewCollection> OrmasDal::GetEntries(int offset, std::string& errorMessage, std::string filter)
	{
		entriesViewCollection rowTuple;
		std::vector<entriesViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".entries_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY entry_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int entryID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string entryDate = PQgetvalue(result, i, 1);
						std::string debitingAccountNumber = PQgetvalue(result, i, 2);
						double value = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string creditingAccountNumber = PQgetvalue(result, i, 4);
						int debitingAccountID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						int creditingAccountID = std::stoi(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						std::string description = PQgetvalue(result, i, 7);
						rowTuple = std::make_tuple(entryID, entryDate, debitingAccountNumber, value, creditingAccountNumber,
							debitingAccountID, creditingAccountID, description);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for entries, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get entry routing
	std::vector<entryRoutingCollection> OrmasDal::GetEntryRouting(int offset, std::string& errorMessage, std::string filter)
	{
		entryRoutingCollection rowTuple;
		std::vector<entryRoutingCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".entry_routing_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY entry_routing_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int entryRoutingID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string operation = PQgetvalue(result, i, 1);
						int debit = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						int credit = std::stoi(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						rowTuple = std::make_tuple(entryRoutingID, operation, debit, credit);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for entry routing, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all entry-subaccount link from DB
	std::vector<entrySubaccountCollection> OrmasDal::GetEntrySubaccount(int offset, std::string& errorMessage, std::string filter)
	{
		entrySubaccountCollection rowTuple;
		std::vector<entrySubaccountCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".entry_subaccount_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY entry_subaccount_id ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int entrySubaccountID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int entryID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int subaccountID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						rowTuple = std::make_tuple(entrySubaccountID, entryID, subaccountID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for entry-subaccount link, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all financial report
	std::vector<financialReportCollection> OrmasDal::GetFinancialReport(int offset, std::string& errorMessage, std::string filter)
	{
		financialReportCollection rowTuple;
		std::vector<financialReportCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".financial_report_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY financial_report_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());
			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int financialReportID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						double acc_44010 = std::stod(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						double acc_55010 = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						double acc_552 = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						double acc_55270 = std::stod(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						double acc_553 = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						double acc_55321 = std::stod(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						double acc_44020_90 = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double acc_66010_66110 = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						double acc_66020_66120 = std::stod(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						double acc_66040_66140 = std::stod(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						double acc_66050_66150 = std::stod(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						double acc_66060_66160 = std::stod(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						double acc_66130 = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double acc_66070_66170 = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						double tax = std::stod(std::string(PQgetvalue(result, i, 15)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 15)));
						std::string fromDate = PQgetvalue(result, i, 16);
						std::string tillDate = PQgetvalue(result, i, 17);
						rowTuple = std::make_tuple(financialReportID, acc_44010, acc_55010, acc_552, acc_55270, acc_553, acc_55321,
							acc_44020_90, acc_66010_66110, acc_66020_66120, acc_66040_66140, acc_66050_66150, acc_66060_66160,
							acc_66130, acc_66070_66170, tax, fromDate, tillDate);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for entry-subaccount link, please contact with appliction provider!";
			}
		}
		return resultVector;
	}


	//Get inventorization list
	std::vector<inventorizationListViewCollection> OrmasDal::GetInventorizationList(int offset, std::string& errorMessage, std::string filter)
	{
		inventorizationListViewCollection rowTuple;
		std::vector<inventorizationListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".inventorization_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY inventorization_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int ilID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int inventorizationID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(ilID, inventorizationID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for inventorization list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get inventorizations
	std::vector<inventorizationsViewCollection> OrmasDal::GetInventorizations(int offset, std::string& errorMessage, std::string filter)
	{
		inventorizationsViewCollection rowTuple;
		std::vector<inventorizationsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".inventorizations_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY inventorization_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int inventorizationID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string inventorizationDate = PQgetvalue(result, i, 1);
						std::string inventorizationExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePosition = PQgetvalue(result, i, 8);
						std::string stockEmployeeName = PQgetvalue(result, i, 9);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 10);
						std::string stockEmployeePhone = PQgetvalue(result, i, 11);
						std::string stockEmployeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(inventorizationID, inventorizationDate, inventorizationExecutionDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for inventorization, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get jobprice
	std::vector<jobpriceViewCollection> OrmasDal::GetJobprice(int offset, std::string& errorMessage, std::string filter)
	{
		jobpriceViewCollection rowTuple;
		std::vector<jobpriceViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".jobprice_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY jobprice_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int jobpriceID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string productName = PQgetvalue(result, i, 1);
						double value = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string currencyName = PQgetvalue(result, i, 3);
						double volume = std::stod(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						std::string measureName = PQgetvalue(result, i, 5);
						std::string positionName = PQgetvalue(result, i, 6);
						int productID = std::stoi(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						int measureID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						int positionID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						rowTuple = std::make_tuple(jobpriceID, productName, value, currencyName, volume, measureName, positionName, productID,
							currencyID, measureID, positionID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for jobprice, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get jobsheet
	std::vector<jobsheetViewCollection> OrmasDal::GetJobsheet(int offset, std::string& errorMessage, std::string filter)
	{
		jobsheetViewCollection rowTuple;
		std::vector<jobsheetViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".jobsheet_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY jobsheet_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int jobsheetID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string date = PQgetvalue(result, i, 1);
						std::string employeeName = PQgetvalue(result, i, 2);
						std::string employeeSurname = PQgetvalue(result, i, 3);
						std::string employeePhone = PQgetvalue(result, i, 4);
						std::string productName = PQgetvalue(result, i, 5);
						double count = std::stod(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						std::string measureName = PQgetvalue(result, i, 7);
						int productID = std::stoi(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						rowTuple = std::make_tuple(jobsheetID, date, employeeName, employeeSurname, employeePhone, productName, count, measureName,
							productID, employeeID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for jobsheet, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get Locations
	std::vector<locationsCollection> OrmasDal::GetLocations(int offset, std::string& errorMessage, std::string filter)
	{
		locationsCollection rowTuple;
		std::vector<locationsCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".locations_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY location_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int locationID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string countryName = PQgetvalue(result, i, 1);
						std::string countryCode = PQgetvalue(result, i, 2);
						std::string regionName = PQgetvalue(result, i, 3);
						std::string cityName = PQgetvalue(result, i, 4);
						rowTuple = std::make_tuple(locationID, countryName, countryCode, regionName, cityName);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for location, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get measures
	std::vector<measuresCollection> OrmasDal::GetMeasures(int offset, std::string& errorMessage, std::string filter)
	{
		measuresCollection rowTuple;
		std::vector<measuresCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".measures_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY measure_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int measureID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string measureName = PQgetvalue(result, i, 1);
						std::string measureShortName = PQgetvalue(result, i, 2);
						std::string mUnit = std::string(PQgetvalue(result, i, 3));
						int measureUnit = std::stoi(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						rowTuple = std::make_tuple(measureID, measureName, measureShortName, measureUnit);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for measure, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get net cost
	std::vector<netCostViewCollection> OrmasDal::GetNetCost(int offset, std::string& errorMessage, std::string filter)
	{
		netCostViewCollection rowTuple;
		std::vector<netCostViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".net_cost_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY net_cost_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int netCostID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string netCostDate = PQgetvalue(result, i, 1);
						std::string productName = PQgetvalue(result, i, 2);
						double productVolume = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string measureName = PQgetvalue(result, i, 4);
						double netCostValue = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string currencyName = PQgetvalue(result, i, 6);
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string isOutdatedStr = (PQgetvalue(result, i, 9));
						bool isOutdated = (isOutdatedStr.compare("t") == 0 ? true : false);
						rowTuple = std::make_tuple(netCostID, netCostDate, productName, productVolume, measureName, netCostValue,
							currencyName, currencyID, productID, isOutdated);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for net cost, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get order list
	std::vector<orderListViewCollection> OrmasDal::GetOrderList(int offset, std::string& errorMessage, std::string filter)
	{
		orderListViewCollection rowTuple;
		std::vector<orderListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".order_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY order_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int olID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int orderID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(olID, orderID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for order list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get orders
	std::vector<ordersViewCollection> OrmasDal::GetOrders(int offset, std::string& errorMessage, std::string filter)
	{
		ordersViewCollection rowTuple;
		std::vector<ordersViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".orders_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY order_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int orderID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string orderDate = PQgetvalue(result, i, 1);
						std::string orderExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string clientName = PQgetvalue(result, i, 5);
						std::string clientSurname = PQgetvalue(result, i, 6);
						std::string clientPhone = PQgetvalue(result, i, 7);
						std::string clientAddress = PQgetvalue(result, i, 8);
						std::string firm = PQgetvalue(result, i, 9);
						std::string employeeName = PQgetvalue(result, i, 10);
						std::string employeeSurname = PQgetvalue(result, i, 11);
						std::string employeePhone = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(orderID, orderDate, orderExecutionDate, statusCode, statusName, clientName, clientSurname, clientPhone,
							clientAddress, firm, employeeName, employeeSurname, employeePhone, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for order, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get order raw list
	std::vector<orderRawListViewCollection> OrmasDal::GetOrderRawList(int offset, std::string& errorMessage, std::string filter)
	{
		orderRawListViewCollection rowTuple;
		std::vector<orderRawListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".order_raw_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY order_raw_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int olID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int orderRawID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(olID, orderRawID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for order raw list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get order raws
	std::vector<orderRawsViewCollection> OrmasDal::GetOrderRaws(int offset, std::string& errorMessage, std::string filter)
	{
		orderRawsViewCollection rowTuple;
		std::vector<orderRawsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".order_raws_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY order_raw_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int orderRawID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string orderRawDate = PQgetvalue(result, i, 1);
						std::string orderRawExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string purveyorName = PQgetvalue(result, i, 5);
						std::string purveyorSurname = PQgetvalue(result, i, 6);
						std::string purveyorPhone = PQgetvalue(result, i, 7);
						std::string purveyorCompanyName = PQgetvalue(result, i, 8);
						std::string employeeName = PQgetvalue(result, i, 9);
						std::string employeeSurname = PQgetvalue(result, i, 10);
						std::string employeePhone = PQgetvalue(result, i, 11);
						std::string employeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(orderRawID, orderRawDate, orderRawExecutionDate, statusCode,
							statusName, purveyorName, purveyorSurname, purveyorPhone, purveyorCompanyName, employeeName,
							employeeSurname, employeePhone, employeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for order raw, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get payments
	std::vector<paymentsViewCollection> OrmasDal::GetPayments(int offset, std::string& errorMessage, std::string filter)
	{
		paymentsViewCollection rowTuple;
		std::vector<paymentsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".payments_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY payment_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int paymentID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string paymentDate = PQgetvalue(result, i, 1);
						std::string userName = PQgetvalue(result, i, 2);
						std::string userSurname = PQgetvalue(result, i, 3);
						std::string userPhone = PQgetvalue(result, i, 4);
						double paymentValue = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string currencyName = PQgetvalue(result, i, 6);
						std::string statusName = PQgetvalue(result, i, 7);
						int userID = std::stoi(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						rowTuple = std::make_tuple(paymentID, paymentDate, userName, userSurname, userPhone, paymentValue,
							currencyName, statusName, userID, currencyID, statusID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for payment, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get payment-employee
	std::vector<paymentEmployeeCollection> OrmasDal::GetPaymentEmployee(int offset, std::string& errorMessage, std::string filter)
	{
		paymentEmployeeCollection rowTuple;
		std::vector<paymentEmployeeCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".payment_employee_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY payment_employee_id ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int paymentEmployeeID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int paymentID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						rowTuple = std::make_tuple(paymentEmployeeID, paymentID, employeeID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for payment-employee, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get payslip
	std::vector<payslipsViewCollection> OrmasDal::GetPayslips(int offset, std::string& errorMessage, std::string filter)
	{
		payslipsViewCollection rowTuple;
		std::vector<payslipsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".payslips_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY payslip_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int payslipID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string payslipDate = PQgetvalue(result, i, 1);
						double payslipValue = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string currencyName = PQgetvalue(result, i, 3);
						int salaryID = std::stoi(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						rowTuple = std::make_tuple(payslipID, payslipDate, payslipValue, currencyName, salaryID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for payslip, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Payslip-Order relation
	std::vector<payslipOrderCollection> OrmasDal::GetPayslipOrder(int offset, std::string& errorMessage, std::string filter)
	{
		payslipOrderCollection rowTuple;
		std::vector<payslipOrderCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".payslip_order_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY payslip_order_id ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int payslipOrederID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int payslipID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int orderID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						rowTuple = std::make_tuple(payslipOrederID, payslipID, orderID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for payslip-order relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get percent rate
	std::vector<percentRateCollection> OrmasDal::GetPercentRate(int offset, std::string& errorMessage, std::string filter)
	{
		percentRateCollection rowTuple;
		std::vector<percentRateCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".percent_rate_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY percent_rate_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int percentRateID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						double value = std::stod(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string condition = PQgetvalue(result, i, 2);
						int positionID = std::stoi(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						rowTuple = std::make_tuple(percentRateID, value, condition, positionID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for photo, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get photos
	std::vector<photosCollection> OrmasDal::GetPhotos(int offset, std::string& errorMessage, std::string filter)
	{
		photosCollection rowTuple;
		std::vector<photosCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".photos_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY photo_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int photoID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string source = PQgetvalue(result, i, 3);
						rowTuple = std::make_tuple(photoID, userID, productID, source);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for photo, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get positions
	std::vector<positionsCollection> OrmasDal::GetPositions(int offset, std::string& errorMessage, std::string filter)
	{
		positionsCollection rowTuple;
		std::vector<positionsCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".positions_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY position_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int positionID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string positionName = PQgetvalue(result, i, 1);
						rowTuple = std::make_tuple(positionID, positionName);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for position, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get prices
	std::vector<pricesViewCollection> OrmasDal::GetPrices(int offset, std::string& errorMessage, std::string filter)
	{
		pricesViewCollection rowTuple;
		std::vector<pricesViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".prices_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY price_id DESC LIMIT 200 ";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int priceID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string priceDate = PQgetvalue(result, i, 1);
						std::string productName = PQgetvalue(result, i, 2);
						double productVolume = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string measureName = PQgetvalue(result, i, 4);
						double priceValue = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string currencyName = PQgetvalue(result, i, 6);
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string isOutdatedStr = (PQgetvalue(result, i, 9));
						bool isOutdated = (isOutdatedStr.compare("t") == 0 ? true : false);
						rowTuple = std::make_tuple(priceID, priceDate, productName, productVolume, measureName, priceValue,
							currencyName, currencyID, productID, isOutdated);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for price, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get product type
	std::vector<productTypeCollection> OrmasDal::GetProductTypes(int offset, std::string& errorMessage, std::string filter)
	{
		productTypeCollection rowTuple;
		std::vector<productTypeCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".product_types_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY product_type_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int productTypeID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string productTypeName = PQgetvalue(result, i, 1);
						std::string productTypeShortName = PQgetvalue(result, i, 2);
						std::string productCode = PQgetvalue(result, i, 3);
						rowTuple = std::make_tuple(productTypeID, productTypeName, productTypeShortName, productCode);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for product type, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get production
	std::vector<productionCollection> OrmasDal::GetProduction(int offset, std::string& errorMessage, std::string filter)
	{
		productionCollection rowTuple;
		std::vector<productionCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".production_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY production_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int productionID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string productionDate = PQgetvalue(result, i, 1);
						std::string expiryDate = PQgetvalue(result, i, 2);
						std::string sessionStart = PQgetvalue(result, i, 3);
						std::string sessionEnd = PQgetvalue(result, i, 4);
						rowTuple = std::make_tuple(productionID, productionDate, expiryDate, sessionStart, sessionEnd);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get production list
	std::vector<productionListViewCollection> OrmasDal::GetProductionList(int offset, std::string& errorMessage, std::string filter)
	{
		productionListViewCollection rowTuple;
		std::vector<productionListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".production_list_view";
			sqlCommand += filter;
			sqlCommand += "  ORDER BY production_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int plID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int productionID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(plID, productionID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get production consume raw list
	std::vector<productionConsumeRawListViewCollection> OrmasDal::GetProductionConsumeRawList(int offset, std::string& errorMessage, std::string filter)
	{
		productionConsumeRawListViewCollection rowTuple;
		std::vector<productionConsumeRawListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".production_consume_raw_list_view";
			sqlCommand += filter;
			sqlCommand += "  ORDER BY consume_raw_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int clID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int consumeRawID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(clID, consumeRawID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production consume raw list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get production consume raws
	std::vector<productionConsumeRawsViewCollection> OrmasDal::GetProductionConsumeRaws(int offset, std::string& errorMessage, std::string filter)
	{
		productionConsumeRawsViewCollection rowTuple;
		std::vector<productionConsumeRawsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".production_consume_raws_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY consume_raw_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int consumeRawID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string consumeRawDate = PQgetvalue(result, i, 1);
						std::string consumeRawExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePosition = PQgetvalue(result, i, 8);
						std::string stockEmployeeName = PQgetvalue(result, i, 9);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 10);
						std::string stockEmployeePhone = PQgetvalue(result, i, 11);
						std::string stockEmployeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(consumeRawID, consumeRawDate, consumeRawExecutionDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production consume raw, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get production plan list
	std::vector<productionPlanListViewCollection> OrmasDal::GetProductionPlanList(int offset, std::string& errorMessage, std::string filter)
	{
		productionPlanListViewCollection rowTuple;
		std::vector<productionPlanListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".production_plan_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY production_plan_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int pplID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int productionPlanID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(pplID, productionPlanID, productName, price, currencyName, volume, measureName,
							count, sum, sumCurrencyName, statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production plan list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get production plan
	std::vector<productionPlanViewCollection> OrmasDal::GetProductionPlan(int offset, std::string& errorMessage, std::string filter)
	{
		productionPlanViewCollection rowTuple;
		std::vector<productionPlanViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".production_plan_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY production_plan_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int productionPlanID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string productionPlanDate = PQgetvalue(result, i, 1);
						double count = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = PQgetvalue(result, i, 4);
						std::string statusName = PQgetvalue(result, i, 5);
						std::string employeeName = PQgetvalue(result, i, 6);
						std::string employeeSurname = PQgetvalue(result, i, 7);
						std::string employeePhone = PQgetvalue(result, i, 8);
						std::string employeePosition = PQgetvalue(result, i, 9);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));

						rowTuple = std::make_tuple(productionPlanID, productionPlanDate, count, sum, currencyName, statusName,
							employeeName, employeeSurname, employeePhone, employeePosition, employeeID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production plan, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get production stock
	std::vector<productionStockViewCollection> OrmasDal::GetProductionStock(int offset, std::string& errorMessage, std::string filter)
	{
		productionStockViewCollection rowTuple;
		std::vector<productionStockViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".production_stock_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY production_stock_id ASC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int sID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string productName = PQgetvalue(result, i, 1);
						double price = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string currencyName = std::string(PQgetvalue(result, i, 3));
						double volume = std::stod(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						std::string measureName = std::string(PQgetvalue(result, i, 5));
						double count = std::stod(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 8));
						std::string statusName = std::string(PQgetvalue(result, i, 9));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						rowTuple = std::make_tuple(sID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production stock, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get products
	std::vector<productsViewCollection> OrmasDal::GetProducts(int offset, std::string& errorMessage, std::string filter)
	{
		productsViewCollection rowTuple;
		std::vector<productsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".products_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY product_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());
			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int productID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string productName = PQgetvalue(result, i, 1);
						double price = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string currencyName = PQgetvalue(result, i, 3);
						double volume = std::stod(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						std::string measureName = PQgetvalue(result, i, 5);
						std::string productTypeName = PQgetvalue(result, i, 6);
						int shelfLife = std::stoi(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						std::string companyName = PQgetvalue(result, i, 8);
						int companyID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						int measureID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						int productTypeID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));

						rowTuple = std::make_tuple(productID, productName, price, currencyName, volume, measureName, productTypeName,
							shelfLife, companyName, companyID, measureID, productTypeID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for product, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all purveyors from DB
	std::vector<purveyorsViewCollection> OrmasDal::GetPurveyors(int offset, std::string& errorMessage, std::string filter)
	{
		purveyorsViewCollection rowTuple;
		std::vector<purveyorsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".purveyors_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY user_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{

						int userID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string name = PQgetvalue(result, i, 1);
						std::string surname = PQgetvalue(result, i, 2);
						std::string phone = PQgetvalue(result, i, 3);
						std::string country = PQgetvalue(result, i, 4);
						std::string region = PQgetvalue(result, i, 5);
						std::string city = PQgetvalue(result, i, 6);
						std::string address = PQgetvalue(result, i, 7);
						std::string company_name = PQgetvalue(result, i, 8);
						std::string roleName = PQgetvalue(result, i, 9);
						std::string password = PQgetvalue(result, i, 10);
						std::string email = PQgetvalue(result, i, 11);
						std::string actStr = (PQgetvalue(result, i, 12));
						bool activated = (actStr.compare("t") == 0 ? true : false);
						int roleID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						int locationID = std::stoi(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						rowTuple = std::make_tuple(userID, name, surname, phone, country, region, city, address, company_name,
							roleName, password, email, activated, roleID, locationID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for purveyors, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get receipt product list
	std::vector<receiptProductListViewCollection> OrmasDal::GetReceiptProductList(int offset, std::string& errorMessage, std::string filter)
	{
		receiptProductListViewCollection rowTuple;
		std::vector<receiptProductListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".receipt_product_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY receipt_product_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int rlID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int receiptProductID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(rlID, receiptProductID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for receipt product list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get receipt products
	std::vector<receiptProductsViewCollection> OrmasDal::GetReceiptProducts(int offset, std::string& errorMessage, std::string filter)
	{
		receiptProductsViewCollection rowTuple;
		std::vector<receiptProductsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".receipt_products_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY receipt_product_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int receiptProductID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string receiptProductDate = PQgetvalue(result, i, 1);
						std::string receiptProductExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePosition = PQgetvalue(result, i, 8);
						std::string stockEmployeeName = PQgetvalue(result, i, 9);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 10);
						std::string stockEmployeePhone = PQgetvalue(result, i, 11);
						std::string stockEmployeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(receiptProductID, receiptProductDate, receiptProductExecutionDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for receipt product, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get receipt raw list
	std::vector<receiptRawListViewCollection> OrmasDal::GetReceiptRawList(int offset, std::string& errorMessage, std::string filter)
	{
		receiptRawListViewCollection rowTuple;
		std::vector<receiptRawListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".receipt_raw_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY receipt_raw_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int rlID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int receiptRawID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(rlID, receiptRawID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for receipt raw list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get receipt raws
	std::vector<receiptRawsViewCollection> OrmasDal::GetReceiptRaws(int offset, std::string& errorMessage, std::string filter)
	{
		receiptRawsViewCollection rowTuple;
		std::vector<receiptRawsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".receipt_raws_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY receipt_raw_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int receiptRawID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string receiptRawDate = PQgetvalue(result, i, 1);
						std::string receiptRawExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePosition = PQgetvalue(result, i, 8);
						std::string stockEmployeeName = PQgetvalue(result, i, 9);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 10);
						std::string stockEmployeePhone = PQgetvalue(result, i, 11);
						std::string stockEmployeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(receiptRawID, receiptRawDate, receiptRawExecutionDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for receipt raw, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get refunds
	std::vector<refundsViewCollection> OrmasDal::GetRefunds(int offset, std::string& errorMessage, std::string filter)
	{
		refundsViewCollection rowTuple;
		std::vector<refundsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".refunds_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY refund_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int refundID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string refundDate = PQgetvalue(result, i, 1);
						double refundValue = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string currencyName = PQgetvalue(result, i, 3);
						int userID = std::stoi(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						rowTuple = std::make_tuple(refundID, refundDate, refundValue, currencyName, userID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for refunds, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get relation type
	std::vector<relationTypeCollection> OrmasDal::GetRelationType(int offset, std::string& errorMessage, std::string filter)
	{
		relationTypeCollection rowTuple;
		std::vector<relationTypeCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".relation_type_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY relation_type_id DESC  LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());
			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int relationTypeID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string relationTypeName = PQgetvalue(result, i, 1);
						std::string comment = PQgetvalue(result, i, 2);
						rowTuple = std::make_tuple(relationTypeID, relationTypeName, comment);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for relation type, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get relations
	std::vector<relationsViewCollection> OrmasDal::GetRelations(int offset, std::string& errorMessage, std::string filter)
	{
		relationsViewCollection rowTuple;
		std::vector<relationsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".relations_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY user_id_1 ASC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());
			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int relationID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string employeeName = std::string(PQgetvalue(result, i, 1));
						std::string employeeSurname = std::string(PQgetvalue(result, i, 2));
						std::string employeePhone = std::string(PQgetvalue(result, i, 3));
						std::string relationName = std::string(PQgetvalue(result, i, 4));
						std::string clientName = std::string(PQgetvalue(result, i, 5));
						std::string clientSurname = std::string(PQgetvalue(result, i, 6));
						std::string clientPhone = std::string(PQgetvalue(result, i, 7));
						int userID1 = std::stoi(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						int userID2 = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						int relationTypeID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));

						rowTuple = std::make_tuple(relationID, employeeName, employeeSurname, employeePhone, relationName, clientName,
							clientSurname, clientPhone, userID1, userID2, relationTypeID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for relation, please contact with appliction provider!";
			}
		}
		return resultVector;
	}


	//Get return list
	std::vector<returnListViewCollection> OrmasDal::GetReturnList(int offset, std::string& errorMessage, std::string filter)
	{
		returnListViewCollection rowTuple;
		std::vector<returnListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".return_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY return_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int rlID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int returnID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(rlID, returnID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for return list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get returns 
	std::vector<returnsViewCollection> OrmasDal::GetReturns(int offset, std::string& errorMessage, std::string filter)
	{
		returnsViewCollection rowTuple;
		std::vector<returnsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".returns_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY return_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int returnID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string returnDate = PQgetvalue(result, i, 1);
						std::string returnExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string clientName = PQgetvalue(result, i, 5);
						std::string clientSurname = PQgetvalue(result, i, 6);
						std::string clientPhone = PQgetvalue(result, i, 7);
						std::string clientAddress = PQgetvalue(result, i, 8);
						std::string firm = PQgetvalue(result, i, 9);
						std::string employeeName = PQgetvalue(result, i, 10);
						std::string employeeSurname = PQgetvalue(result, i, 11);
						std::string employeePhone = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(returnID, returnDate, returnExecutionDate, statusCode, statusName, clientName, clientSurname,
							clientPhone, clientAddress, firm, employeeName, employeeSurname, employeePhone, count, sum, currencyName,
							employeeID, userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for return, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get roles
	std::vector<rolesCollection> OrmasDal::GetRoles(int offset, std::string& errorMessage, std::string filter)
	{
		rolesCollection rowTuple;
		std::vector<rolesCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".roles_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY role_id ASC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int roleID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string roleCode = PQgetvalue(result, i, 1);
						std::string roleName = PQgetvalue(result, i, 2);
						std::string comment = PQgetvalue(result, i, 3);
						rowTuple = std::make_tuple(roleID, roleCode, roleName, comment);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for role, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all salaries from DB
	std::vector<salariesViewCollection> OrmasDal::GetSalaries(int offset, std::string& errorMessage, std::string filter)
	{
		salariesViewCollection rowTuple;
		std::vector<salariesViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".salaries_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY salary_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int salaryID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string userName = PQgetvalue(result, i, 2);
						std::string userSurname = PQgetvalue(result, i, 3);
						std::string userPhone = PQgetvalue(result, i, 4);
						std::string salaryDate = PQgetvalue(result, i, 5);
						double salaryValue = std::stod(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						std::string currencyName = PQgetvalue(result, i, 7);
						std::string salaryTypeName = PQgetvalue(result, i, 8);
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						int salaryTypeID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						std::string bonusStr = (PQgetvalue(result, i, 11));
						bool salaryBonus = (bonusStr.compare("t") == 0 ? true : false);
						rowTuple = std::make_tuple(salaryID, userID, userName, userSurname, userPhone, salaryDate, salaryValue,
							currencyName, salaryTypeName, currencyID, salaryTypeID, salaryBonus);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for salary, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all salaries from DB
	std::vector<salaryTypeCollection> OrmasDal::GetSalaryType(int offset, std::string& errorMessage, std::string filter)
	{
		salaryTypeCollection rowTuple;
		std::vector<salaryTypeCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".salary_type_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY salary_type_id DESC  LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{

						int salaryTypeID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string salaryTypeCode = PQgetvalue(result, i, 1);
						std::string salaryTypeName = PQgetvalue(result, i, 2);
						rowTuple = std::make_tuple(salaryTypeID, salaryTypeCode, salaryTypeName);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for salary type, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get specification list
	std::vector<specificationListViewCollection> OrmasDal::GetSpecificationList(int offset, std::string& errorMessage, std::string filter)
	{
		specificationListViewCollection rowTuple;
		std::vector<specificationListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".specification_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY specification_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int slID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int specificationID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double count = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string measureName = std::string(PQgetvalue(result, i, 4));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						rowTuple = std::make_tuple(slID, specificationID, productName, count, measureName, productID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for specification list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get specifications 
	std::vector<specificationsViewCollection> OrmasDal::GetSpecifications(int offset, std::string& errorMessage, std::string filter)
	{
		specificationsViewCollection rowTuple;
		std::vector<specificationsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".specifications_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY specification_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int specificationID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string specificationDate = PQgetvalue(result, i, 1);
						std::string productName = PQgetvalue(result, i, 2);
						double sum = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePositionName = PQgetvalue(result, i, 8);
						int productID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));

						rowTuple = std::make_tuple(specificationID, specificationDate, productName, sum, currencyName, employeeName,
							employeeSurname, employeePhone, employeePositionName, productID, currencyID, employeeID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for return, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get spoilage list
	std::vector<spoilageListViewCollection> OrmasDal::GetSpoilageList(int offset, std::string& errorMessage, std::string filter)
	{
		spoilageListViewCollection rowTuple;
		std::vector<spoilageListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".spoilage_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY spoilage_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int slID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int spoilageID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(slID, spoilageID, productName, price, currencyName, volume, measureName,
							count, sum, sumCurrencyName, statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production plan list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get spoilage
	std::vector<spoilageViewCollection> OrmasDal::GetSpoilage(int offset, std::string& errorMessage, std::string filter)
	{
		spoilageViewCollection rowTuple;
		std::vector<spoilageViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".spoilage_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY spoilage_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int spoilageID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string spoilageDate = PQgetvalue(result, i, 1);
						double count = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = PQgetvalue(result, i, 4);
						std::string statusName = PQgetvalue(result, i, 5);
						std::string employeeName = PQgetvalue(result, i, 6);
						std::string employeeSurname = PQgetvalue(result, i, 7);
						std::string employeePhone = PQgetvalue(result, i, 8);
						std::string employeePosition = PQgetvalue(result, i, 9);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));

						rowTuple = std::make_tuple(spoilageID, spoilageDate, count, sum, currencyName, statusName,
							employeeName, employeeSurname, employeePhone, employeePosition, employeeID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for production plan, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get state
	std::vector<stateCollection> OrmasDal::GetState(int offset, std::string& errorMessage, std::string filter)
	{
		stateCollection rowTuple;
		std::vector<stateCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".state_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY state_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int stateID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int universalID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string lastChange = PQgetvalue(result, i, 3);
						rowTuple = std::make_tuple(stateID, universalID, statusID, lastChange);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for status, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get status
	std::vector<statusCollection> OrmasDal::GetStatus(int offset, std::string& errorMessage, std::string filter)
	{
		statusCollection rowTuple;
		std::vector<statusCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".status_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY status_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string statusCode = PQgetvalue(result, i, 1);
						std::string statusName = PQgetvalue(result, i, 2);
						std::string comment = PQgetvalue(result, i, 3);
						rowTuple = std::make_tuple(statusID, statusCode, statusName, comment);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for status, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get status rule
	std::vector<statusRuleViewCollection> OrmasDal::GetStatusRule(int offset, std::string& errorMessage, std::string filter)
	{
		statusRuleViewCollection rowTuple;
		std::vector<statusRuleViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".status_rule_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY status_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int stateRuleID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string operation = PQgetvalue(result, i, 1);
						std::string statusName = PQgetvalue(result, i, 2);
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						rowTuple = std::make_tuple(stateRuleID, operation, statusName, statusID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for status, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get stock
	std::vector<stockViewCollection> OrmasDal::GetStock(int offset, std::string& errorMessage, std::string filter)
	{
		stockViewCollection rowTuple;
		std::vector<stockViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".stock_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY stock_id ASC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int sID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string productName = PQgetvalue(result, i, 1);
						double price = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string currencyName = std::string(PQgetvalue(result, i, 3));
						double volume = std::stod(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						std::string measureName = std::string(PQgetvalue(result, i, 5));
						double count = std::stod(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 8));
						std::string statusName = std::string(PQgetvalue(result, i, 9));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 10)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 10)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						rowTuple = std::make_tuple(sID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for stock, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all subaccounts from DB
	std::vector<subaccountsViewCollection> OrmasDal::GetSubaccounts(int offset, std::string& errorMessage, std::string filter)
	{
		subaccountsViewCollection rowTuple;
		std::vector<subaccountsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".subaccounts_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY subaccount_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int subaccountID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int accountID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string accountNumber = PQgetvalue(result, i, 2);
						std::string subaccountNumber = PQgetvalue(result, i, 3);
						double startBalance = std::stod(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						double currentBalance = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string currencyName = PQgetvalue(result, i, 6);
						std::string statusName = PQgetvalue(result, i, 7);
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 9)));
						std::string dateOpened = PQgetvalue(result, i, 10);
						std::string dateClosed = PQgetvalue(result, i, 11);
						std::string details = PQgetvalue(result, i, 11);
						rowTuple = std::make_tuple(subaccountID, accountID, accountNumber, subaccountNumber, startBalance,
							currentBalance, currencyName, statusName, currencyID, statusID, dateOpened, dateClosed, details);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for subaccount, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all subaccounts history from DB
	std::vector<subaccountHistoryCollection> OrmasDal::GetSubaccountHistory(int offset, std::string& errorMessage, std::string filter)
	{
		subaccountHistoryCollection rowTuple;
		std::vector<subaccountHistoryCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".subaccount_history_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY subaccount_history_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int shID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int subaccountID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						double startBalance = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						double currentBalance = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string fromDate = PQgetvalue(result, i, 4);
						std::string tillDate = PQgetvalue(result, i, 5);
						rowTuple = std::make_tuple(shID, subaccountID, startBalance, currentBalance, fromDate, tillDate);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for subaccount, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get all taxes from DB
	std::vector<taxesCollection> OrmasDal::GetTaxes(int offset, std::string& errorMessage, std::string filter)
	{
		taxesCollection rowTuple;
		std::vector<taxesCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult* result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".taxes_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY taxes_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int texID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string taxName = PQgetvalue(result, i, 1);
						std::string taxCode = PQgetvalue(result, i, 2);
						double fixedValue = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						int percentValue = std::stoi(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						std::string formulaValue = PQgetvalue(result, i, 5);
						rowTuple = std::make_tuple(texID, taxName, taxCode, fixedValue, percentValue, formulaValue);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for subaccount, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get timesheet
	std::vector<timesheetViewCollection> OrmasDal::GetTimesheet(int offset, std::string& errorMessage, std::string filter)
	{
		timesheetViewCollection rowTuple;
		std::vector<timesheetViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".timesheet_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY timesheet_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int tID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string userName = PQgetvalue(result, i, 1);
						std::string userSurname = PQgetvalue(result, i, 2);
						std::string userPhone = PQgetvalue(result, i, 3);
						std::string date = PQgetvalue(result, i, 4);
						double workedTime = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						int salaryID = std::stoi(std::string(PQgetvalue(result, i, 6)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 6)));
						rowTuple = std::make_tuple(tID, userName, userSurname, userPhone, date, workedTime, salaryID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for stock, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get transport list
	std::vector<transportListViewCollection> OrmasDal::GetTransportList(int offset, std::string& errorMessage, std::string filter)
	{
		transportListViewCollection rowTuple;
		std::vector<transportListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".transport_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY transport_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int tlID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int transportID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(tlID, transportID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for transport list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get transports
	std::vector<transportsViewCollection> OrmasDal::GetTransports(int offset, std::string& errorMessage, std::string filter)
	{
		transportsViewCollection rowTuple;
		std::vector<transportsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".transports_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY transport_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int transportID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string transportDate = PQgetvalue(result, i, 1);
						std::string transportExecutionDate = PQgetvalue(result, i, 2);
						std::string statusCode = PQgetvalue(result, i, 3);
						std::string statusName = PQgetvalue(result, i, 4);
						std::string employeeName = PQgetvalue(result, i, 5);
						std::string employeeSurname = PQgetvalue(result, i, 6);
						std::string employeePhone = PQgetvalue(result, i, 7);
						std::string employeePosition = PQgetvalue(result, i, 8);
						std::string stockEmployeeName = PQgetvalue(result, i, 9);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 10);
						std::string stockEmployeePhone = PQgetvalue(result, i, 11);
						std::string stockEmployeePosition = PQgetvalue(result, i, 12);
						double count = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 14)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 14)));
						std::string currencyName = PQgetvalue(result, i, 15);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 19)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 19)));

						rowTuple = std::make_tuple(transportID, transportDate, transportExecutionDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for transports, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get user
	std::vector<usersViewCollection> OrmasDal::GetUsers(int offset, std::string& errorMessage, std::string filter)
	{
		usersViewCollection rowTuple;
		std::vector<usersViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".users_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY user_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int userID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string userEmail = PQgetvalue(result, i, 1);
						std::string userName = PQgetvalue(result, i, 2);
						std::string userSurname = PQgetvalue(result, i, 3);
						std::string userPhone = PQgetvalue(result, i, 4);
						std::string userAddress = PQgetvalue(result, i, 5);
						std::string roleName = PQgetvalue(result, i, 6);
						std::string password = PQgetvalue(result, i, 7);
						std::string actStr = (PQgetvalue(result, i, 8));
						bool activated = (actStr.compare("t") == 0 ? true : false);
						int roleID = std::stoi(std::string(PQgetvalue(result, i, 9)).length() == 0 ? "0" : PQgetvalue(result, i, 9));
						rowTuple = std::make_tuple(userID, userEmail, userName, userSurname, userPhone, userAddress,
							roleName, password, activated, roleID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					PQclear(result);
					// if result of query does not contain information and have 0 row, then function return an empty vector;
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for user, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Get withdrawals
	std::vector<withdrawalsViewCollection> OrmasDal::GetWithdrawals(int offset, std::string& errorMessage, std::string filter)
	{
		withdrawalsViewCollection rowTuple;
		std::vector<withdrawalsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".withdrawals_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY withdrawal_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int withdrawalID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string withdrawalDate = PQgetvalue(result, i, 1);
						double withdrawalValue = std::stod(std::string(PQgetvalue(result, i, 2)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 2)));
						std::string currencyName = PQgetvalue(result, i, 3);
						int userID = std::stoi(std::string(PQgetvalue(result, i, 4)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 4)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						rowTuple = std::make_tuple(withdrawalID, withdrawalDate, withdrawalValue, currencyName, userID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for withdrawal, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get write off list
	std::vector<writeOffListViewCollection> OrmasDal::GetWriteOffList(int offset, std::string& errorMessage, std::string filter)
	{
		writeOffListViewCollection rowTuple;
		std::vector<writeOffListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".write_off_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY write_off_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int olID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int orderID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(olID, orderID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for order list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get write-offs
	std::vector<writeOffsViewCollection> OrmasDal::GetWriteOffs(int offset, std::string& errorMessage, std::string filter)
	{
		writeOffsViewCollection rowTuple;
		std::vector<writeOffsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".write_offs_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY write_off_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int orderID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string orderDate = PQgetvalue(result, i, 1);
						std::string statusCode = PQgetvalue(result, i, 2);
						std::string statusName = PQgetvalue(result, i, 3);
						std::string clientName = PQgetvalue(result, i, 4);
						std::string clientSurname = PQgetvalue(result, i, 5);
						std::string clientPhone = PQgetvalue(result, i, 6);
						std::string clientAddress = PQgetvalue(result, i, 7);
						std::string firm = PQgetvalue(result, i, 8);
						std::string employeeName = PQgetvalue(result, i, 9);
						std::string employeeSurname = PQgetvalue(result, i, 10);
						std::string employeePhone = PQgetvalue(result, i, 11);
						double count = std::stod(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						std::string currencyName = PQgetvalue(result, i, 14);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 15)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 15)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));

						rowTuple = std::make_tuple(orderID, orderDate, statusCode, statusName, clientName, clientSurname, clientPhone,
							clientAddress, firm, employeeName, employeeSurname, employeePhone, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for order, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get write-off raw list
	std::vector<writeOffRawListViewCollection> OrmasDal::GetWriteOffRawList(int offset, std::string& errorMessage, std::string filter)
	{
		writeOffRawListViewCollection rowTuple;
		std::vector<writeOffRawListViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".write_off_raw_list_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY write_off_raw_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int wlID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						int writeOffRawID = std::stoi(std::string(PQgetvalue(result, i, 1)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 1)));
						std::string productName = PQgetvalue(result, i, 2);
						double price = std::stod(std::string(PQgetvalue(result, i, 3)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 3)));
						std::string currencyName = std::string(PQgetvalue(result, i, 4));
						double volume = std::stod(std::string(PQgetvalue(result, i, 5)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 5)));
						std::string measureName = std::string(PQgetvalue(result, i, 6));
						double count = std::stod(std::string(PQgetvalue(result, i, 7)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 7)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 8)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 8)));
						std::string sumCurrencyName = std::string(PQgetvalue(result, i, 9));
						std::string statusName = std::string(PQgetvalue(result, i, 10));
						int productID = std::stoi(std::string(PQgetvalue(result, i, 11)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 11)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						rowTuple = std::make_tuple(wlID, writeOffRawID, productName, price, currencyName, volume, measureName, count, sum, sumCurrencyName,
							statusName, productID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for write-off raw list, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	//Get write-off raw
	std::vector<writeOffRawsViewCollection> OrmasDal::GetWriteOffRaws(int offset, std::string& errorMessage, std::string filter)
	{
		writeOffRawsViewCollection rowTuple;
		std::vector<writeOffRawsViewCollection> resultVector;
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
		}
		else
		{
			PGresult * result;
			std::string sqlCommand = "SELECT * FROM \"OrmasSchema\".write_off_raws_view";
			sqlCommand += filter;
			sqlCommand += " ORDER BY write_off_raw_id DESC LIMIT 200 ";
			if (offset > 0)
			{
				sqlCommand += " OFFSET ";
				sqlCommand += boost::lexical_cast<std::string>(offset);
			}
			sqlCommand += " ;";
			result = PQexec(dbConnection, sqlCommand.c_str());

			if (PQresultStatus(result) == PGRES_TUPLES_OK)
			{
				if (PQntuples(result) > 0)
				{
					for (int i = 0; i < PQntuples(result); i++)
					{
						int writeOffRawID = std::stoi(std::string(PQgetvalue(result, i, 0)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 0)));
						std::string writeOffRawDate = PQgetvalue(result, i, 1);
						std::string statusCode = PQgetvalue(result, i, 2);
						std::string statusName = PQgetvalue(result, i, 3);
						std::string employeeName = PQgetvalue(result, i, 4);
						std::string employeeSurname = PQgetvalue(result, i, 5);
						std::string employeePhone = PQgetvalue(result, i, 6);
						std::string employeePosition = PQgetvalue(result, i, 7);
						std::string stockEmployeeName = PQgetvalue(result, i, 8);
						std::string stockEmployeeSurname = PQgetvalue(result, i, 9);
						std::string stockEmployeePhone = PQgetvalue(result, i, 10);
						std::string stockEmployeePosition = PQgetvalue(result, i, 11);
						double count = std::stod(std::string(PQgetvalue(result, i, 12)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 12)));
						double sum = std::stod(std::string(PQgetvalue(result, i, 13)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 13)));
						std::string currencyName = PQgetvalue(result, i, 14);
						int employeeID = std::stoi(std::string(PQgetvalue(result, i, 15)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 15)));
						int userID = std::stoi(std::string(PQgetvalue(result, i, 16)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 16)));
						int statusID = std::stoi(std::string(PQgetvalue(result, i, 17)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 17)));
						int currencyID = std::stoi(std::string(PQgetvalue(result, i, 18)).length() == 0 ? "0" : std::string(PQgetvalue(result, i, 18)));

						rowTuple = std::make_tuple(writeOffRawID, writeOffRawDate, statusCode,
							statusName, employeeName, employeeSurname, employeePhone, employeePosition, stockEmployeeName,
							stockEmployeeSurname, stockEmployeePhone, stockEmployeePosition, count, sum, currencyName, employeeID,
							userID, statusID, currencyID);
						resultVector.push_back(rowTuple);
					}
					PQclear(result);
					return resultVector;
				}
				else
				{
					// if result of query does not contain information and have 0 row, then function return an empty vector;
					PQclear(result);
				}
			}
			else
			{
				std::string logStr = PQresultErrorMessage(result);
				//WriteLog(logStr);
				PQclear(result);
				errorMessage = "Cannot get information from DB for write-off raws, please contact with appliction provider!";
			}
		}
		return resultVector;
	}

	// Create section----------------------------------------------------------------
	// Create access item
	bool OrmasDal::CreateAccessItem(int accessItemID, std::string accessItemEng, std::string accessItemRu, std::string accessItemDivision, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".access_items(access_item_id, access_item_eng, access_item_ru, access_division) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(accessItemID);
		sqlCommand += ", '";
		sqlCommand += accessItemEng;
		sqlCommand += "', '";
		sqlCommand += accessItemRu;
		sqlCommand += "', '";
		sqlCommand += accessItemDivision;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in order list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create access
	bool OrmasDal::CreateAccess(int accessID, int roleID, int accessItemID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".accesses(access_id, role_id, access_item_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(accessID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(roleID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(accessItemID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the access creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create account type
	bool OrmasDal::CreateAccountType(int accountTypeID, std::string accountTypeName, int accountTypeNumber, std::string accountTypeComment, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".account_type(account_type_id, account_type_name, account_type_number, \
								 comment) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(accountTypeID);
		sqlCommand += ", '";
		sqlCommand += accountTypeName;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(accountTypeNumber);
		sqlCommand += ", '";
		sqlCommand += accountTypeComment;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the account type creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create account
	bool OrmasDal::CreateAccount(int accountID, std::string accountNumber, double accountStartBalance, double accountCurrentBalance, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".accounts(account_id, account_number, start_balance, current_balance) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(accountID);
		sqlCommand += ", '";
		sqlCommand += accountNumber;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(accountStartBalance);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(accountCurrentBalance);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the account creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create account history
	bool OrmasDal::CreateAccountHistory(int accountHistoryID, int accountID, std::string accountNumber, double accountStartBalance,
		double aCurrentBalance, std::string fromDate, std::string tillDate, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".account_history(account_history_id, account_id, account_number, start_balance, current_balance, from_date, till_date) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(accountHistoryID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(accountID);
		sqlCommand += ", '";
		sqlCommand += accountNumber;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(accountStartBalance);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(aCurrentBalance);
		sqlCommand += ", '";
		sqlCommand += fromDate;
		sqlCommand += "', '";
		sqlCommand += tillDate;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the account creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	// Create Balance-Payment relation
	bool OrmasDal::CreateBalancePayment(int balancePaymentID, int balanceID, int paymentID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".balance_payment(balance_payment_id, balance_id, payment_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(balancePaymentID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(balanceID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(paymentID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the balance-payment relation creating is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create Balance-Payslip relation
	bool OrmasDal::CreateBalancePayslip(int balancePayslipID, int bananceID, int payslipID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".balance_payslip(balance_payslip_id, balance_id, payslip_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(balancePayslipID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(bananceID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(payslipID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the balance-payslip relation creating is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create Balance-Refund relation
	bool OrmasDal::CreateBalanceRefund(int balanceRefundID, int balanceID, int refundID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".balance_refund(balance_refund_id, balance_id, refund_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(balanceRefundID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(balanceID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(refundID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the balance-refund relation creating is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create Balance-Withdrawal relation
	bool OrmasDal::CreateBalanceWithdrawal(int balanceWithdrawalID, int balanceID, int withdrawalID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".balance_withdrawal(balance_withdrawal_id, balance_id, withdrawal_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(balanceWithdrawalID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(balanceID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(withdrawalID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the balance-withdrawal relation creating is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create balance
	bool OrmasDal::CreateBalance(int balanceID, int userID, int subaccountID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".balances(balance_id, user_id, subaccount_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(balanceID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(subaccountID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the balance creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create char of account
	bool OrmasDal::CreateChartOfAccount(int chartOfAccountID, std::string chartOfAccountNumber, std::string chartOfAccountName, int accountTypeID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".chart_of_accounts(chart_of_account_id, number_of_account, \
								 name_of_account, account_type_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(chartOfAccountID);
		sqlCommand += ", '";
		sqlCommand += chartOfAccountNumber;
		sqlCommand += "', '";
		sqlCommand += chartOfAccountName;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(accountTypeID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the chart of account creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create client
	bool OrmasDal::CreateClient(int userID, std::string firm, std::string firmNumber, int locationID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".clients(user_id, firm, firm_number, location_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", '";
		sqlCommand += firm;
		sqlCommand += "', '";
		sqlCommand += firmNumber;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(locationID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the client creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create company
	bool OrmasDal::CreateCompany(int companyID, std::string companyName, std::string companyAddress, std::string companyPhone, std::string comment,
		std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".companies(company_id, company_name, company_address, company_phone,\
			comment) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(companyID);
		sqlCommand += ", '";
		sqlCommand += companyName;
		sqlCommand += "', '";
		sqlCommand += companyAddress;
		sqlCommand += "', '";
		sqlCommand += companyPhone;
		sqlCommand += "', '";
		sqlCommand += comment;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the company creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	// Create company-account
	bool OrmasDal::CreateCompanyAccount(int companyAccountID, int companyID, int accountID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".company_account(company_account_id, company_id, account_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(companyAccountID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(companyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(accountID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the company-account creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create company-employee
	bool OrmasDal::CreateCompanyEmployee(int companyEmployeeID, int companyID, int employeeID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".company_employee(company_employee_id, company_id, employee_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(companyEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(companyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the company-employee creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create an item in consume product list
	bool OrmasDal::CreateConsumeProductList(int consumeProductListID, int consumeProductID, int productID, double consumeListCount,
		double consumeListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".consume_product_list(consume_product_list_id, consume_product_id, \
										product_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(consumeProductListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeProductID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in consume product list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateConsumeProduct(int consumeProductID, int employeeID, std::string consumeProductDate, std::string consumeProductExecutionDate,
		int stockEmployeeID, double consumeProductCount, double consumeProductSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".consume_products(consume_product_id, user_id, consume_product_date, \
										execution_date, employee_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(consumeProductID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", '";
		sqlCommand += consumeProductDate;
		if (consumeProductExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += consumeProductExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeProductCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeProductSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the consume product creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create an item in consume raw list
	bool OrmasDal::CreateConsumeRawList(int consuleRawListID, int consumeRawID, int productID, double consumeRawListCount, double consumeRawListSum,
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".consume_raw_list(consume_raw_list_id, consume_raw_id, \
								 										product_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(consuleRawListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeRawListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeRawListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in consume raw list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateConsumeRaw(int consumeRawID, int userID, std::string consumeRawDate, std::string consumeRawExecutionDate,
		int stockEmployeeID, double consumeRawaCount, double conusmeRawSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".consume_raws(consume_raw_id, user_id, consume_raw_date, \
								 										execution_date, employee_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(consumeRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", '";
		sqlCommand += consumeRawDate;
		if (consumeRawExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += consumeRawExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeRawaCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(conusmeRawSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the consume raw creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateCurrency(int currenycID, int currencyCode, std::string currencyShortName, std::string currencyName,
		int currencyUnit, bool cMainTrade, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".currencies(currency_id, currency_code, currency_short_name, \
								  currency_name, currency_unit, currency_main_trade) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(currenycID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyCode);
		sqlCommand += ", '";
		sqlCommand += currencyShortName;
		sqlCommand += "', '";
		sqlCommand += currencyName;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(currencyUnit);
		sqlCommand += ", ";
		sqlCommand += cMainTrade ? "TRUE" : "FALSE";
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the curency creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	//Create division account
	bool OrmasDal::CreateDivisionAccount(int divisionAccountID, int disvisionID, int accountID, std::string accountCode, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".division_account(division_account_id, division_id, account_id, \
								 								  account_code) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(divisionAccountID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(disvisionID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(accountID);
		sqlCommand += ", '";
		sqlCommand += accountCode;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the division-account creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	//Create division employee
	bool OrmasDal::CreateDivisionEmployee(int divisionEmployeeID, int disvisionID, int employeeID, bool isContract, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".division_employee(division_employee_id, division_id, employee_id, \
								 								 								  is_contract) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(divisionEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(disvisionID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", ";
		sqlCommand += isContract ? "TRUE" : "FALSE";
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the division-employee creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	//Create division
	bool OrmasDal::CreateDivision(int divisionID, std::string divisionName, std::string divisionCode, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".divisions(division_id, division_name, division_code) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(divisionID);
		sqlCommand += ", '";
		sqlCommand += divisionName;
		sqlCommand += "', '";
		sqlCommand += divisionCode;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the employee creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	// Create employee
	bool OrmasDal::CreateEmployee(int userID, int positionID, std::string birthDate, std::string hireDate, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".employees(user_id, position_id, birth_date, hire_date) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(positionID);
		sqlCommand += ", '";
		sqlCommand += birthDate;
		sqlCommand += "', '";
		sqlCommand += hireDate;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the employee creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create entry
	bool OrmasDal::CreateEntry(int entryID, std::string entryDate, int debbitingAccountID, double entryValue, int creditingAccountID, 
		std::string description, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".entries(entry_id, entry_date, debiting_account_id, value, \
								 crediting_account_id, description) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(entryID);
		sqlCommand += ", '";
		sqlCommand += entryDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(debbitingAccountID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(entryValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(creditingAccountID);
		sqlCommand += ", '";
		sqlCommand += description;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the entry creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create entry routing
	bool OrmasDal::CreateEntryRouting(int entryRoutingID, std::string entryRoutingOperation, int debbitingAccountID, int creditingAccountID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".entry_routing(entry_routing_id, operation_name, debit, credit) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(entryRoutingID);
		sqlCommand += ", '";
		sqlCommand += entryRoutingOperation;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(debbitingAccountID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(creditingAccountID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the entry routing creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create entry-subaccount
	bool OrmasDal::CreateEntrySubaccount(int entrySubaccountID, int entryID, int subaccountID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".entry_subaccount(entry_subaccount_id, entry_id, subaccount_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(entrySubaccountID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(entryID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(subaccountID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the entry-subaccount link creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create financial report 
	bool OrmasDal::CreateFinancialReport(int financialReportID, double account_44010, double account_55010, double account_552,
		double account_55270, double account_553, double account_55321, double account_44020_90, double account_66010_66110,
		double account_66020_66120, double account_66040_66140, double account_66050_66150, double account_66060_66160,
		double account_66130, double account_66070_66170, double tax, std::string fromDate, std::string tillDate, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".financial_report(financial_report_id, acc_44010, acc_55010, acc_552, \
								 acc_55270, acc_553, acc_55321, acc_44020_90, acc_66010_66110, acc_66020_66120, acc_66040_66140,\
								 acc_66050_66150, acc_66060_66160, acc_66130, acc_66070_66170, tax, from_date, till_date) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(financialReportID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_44010);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_55010);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_552);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_55270);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_553);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_55321);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_44020_90);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_66010_66110);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_66020_66120);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_66040_66140);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_66050_66150);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_66060_66160);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_66130);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_66070_66170);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(tax);
		sqlCommand += ", '";
		sqlCommand += fromDate;
		sqlCommand += "', '";
		sqlCommand += tillDate;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the entry-subaccount link creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create an item in inventorization list
	bool OrmasDal::CreateInventorizationList(int inventorizationListID, int inventorizationID, int productID, double inventorizationListCount,
		double inventorizationListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".inventorization_list(inventorization_list_id, inventorization_id, \
								 								 										product_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(inventorizationListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(inventorizationID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(inventorizationListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(inventorizationListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in inventorization list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateInventorization(int inventorizationID, int userID, std::string inventorizationDate, std::string inventorizationExecutionDate,
		int stockEmployeeID, double inventorizationCount, double inventorizationSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".inventorizations(inventorization_id, user_id, inventorization_date, \
								 						execution_date, employee_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(inventorizationID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", '";
		sqlCommand += inventorizationDate;
		if (inventorizationExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += inventorizationExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(inventorizationCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(inventorizationSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the inventorization creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateJobprice(int jobpriceID, int producrtID, double jobpriceValue, int currencyID, double jobpriceVolume,
		int measureID, int positionID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".jobprice(jobprice_id, product_id, value, currency_id, volume,\
								 								 								  measure_id, position_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(jobpriceID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(producrtID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(jobpriceValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(jobpriceVolume);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(measureID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(positionID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the jobprice creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateJobsheet(int jobsheetID, std::string jobsheetDate, double jobsheetCount, int productID, int employeeID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".jobsheet(jobsheet_id, jobsheet_date, count, product_id, employee_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(jobsheetID);
		sqlCommand += ", '";
		sqlCommand += jobsheetDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(jobsheetCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the jobsheet creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateLocation(int locationID, std::string countryName, std::string countryCode, std::string regionName
		, std::string cityName, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".locations(location_id, country_name, country_code,\
								 								  region_name, city_name) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(locationID);
		sqlCommand += ", '";
		sqlCommand += countryName;
		sqlCommand += "', '";
		sqlCommand += countryCode;
		sqlCommand += "', '";
		sqlCommand += regionName;
		sqlCommand += "', '";
		sqlCommand += cityName;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the Location creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateMeasure(int measureID, std::string measureName, std::string measureShortName, int measureUnit, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".measures(measure_id, measure_name, measure_short_name, measure_unit) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(measureID);
		sqlCommand += ", '";
		sqlCommand += measureName;
		sqlCommand += "', '";
		sqlCommand += measureShortName;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(measureUnit);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the measure creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create net cost
	bool OrmasDal::CreateNetCost(int netCostID, std::string netCostDate, double netCostValue, int currencyID, int productID,
		bool netCostIsOutdated, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".net_cost(net_cost_id, net_cost_date, net_cost_value, currency_id,\
								  product_id, is_outdated) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(netCostID);
		sqlCommand += ", '";
		sqlCommand += netCostDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(netCostValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += netCostIsOutdated ? "TRUE" : "FALSE";
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the net cost creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create an item in order list
	bool OrmasDal::CreateOrderList(int orderListID, int orderID, int productID, double orderListCount, double orderListSum, 
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".order_list(order_list_id, order_id, product_id, count, sum, status_id,\
								 								  currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(orderListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in order list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateOrder(int orderID, int clientID, std::string orderDate, std::string orderExecutionDate, int employeeID,
		double orderCount, double orderSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".orders(order_id, user_id, order_date, execution_date, \
									employee_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(orderID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(clientID);
		sqlCommand += ", '";
		sqlCommand += orderDate;
		if (orderExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += orderExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the order creation is failed, please contact with application provider!"; 
			return false;
		}
		PQclear(result);
		return true;
	}
	
	// Create an item in order raw list
	bool OrmasDal::CreateOrderRawList(int orderRawListID, int orderRawID, int productID, double orderRawListCount, double orderRawSum,
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".order_raw_list(order_raw_list_id, order_raw_id, \
								 								 	product_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(orderRawListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderRawListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderRawSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in order raw list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateOrderRaw(int orderRawID, int productID, std::string orderRawDate, std::string orderRawExecutionDate,
		int employeeID, double orderRawCount, double orderRawSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".order_raws(order_raw_id, purveyor_id, order_raw_date, \
								 						execution_date, employee_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(orderRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", '";
		sqlCommand += orderRawDate;
		if (orderRawExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += orderRawExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderRawCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderRawSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the order raw creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create payment
	bool OrmasDal::CreatePayment(int paymentID, std::string paymatDate, double paymentValue, int userID, int currencyID, int statusID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".payments(payment_id, payment_date, payment_value, user_id, currency_id, status_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(paymentID);
		sqlCommand += ", '";
		sqlCommand += paymatDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(paymentValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the payment creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create Payment-Employee relation
	bool OrmasDal::CreatePaymentEmployee(int paymentEmployeeID, int paymentID, int employeeID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".payment_employee(payment_employee_id, payment_id, employee_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(paymentEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(paymentID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the payment-employee relation creating is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create payslip
	bool OrmasDal::CreatePayslip(int payslipID, std::string payslipDate, double payslipValue, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".payslips(payslip_id, payslip_date, payslip_value, salary_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(payslipID);
		sqlCommand += ", '";
		sqlCommand += payslipDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(payslipValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the payslip creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create Payslip-Order relation
	bool OrmasDal::CreatePayslipOrder(int payslipOrderID, int payslipID, int orderID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".payslip_order(payslip_order_id, payslip_id, order_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(payslipOrderID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(payslipID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the payslip-order relation creating is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create percent rate
	bool OrmasDal::CreatePercentRate(int percentRateID, double percentRateValue, std::string percentRateCondition, int positionID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".percent_rate(percent_rate_id, value, condition, position_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(percentRateID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(percentRateValue);
		sqlCommand += ", '";
		sqlCommand += percentRateCondition;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(positionID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the percent rate creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create photo
	bool OrmasDal::CreatePhoto(int photoID, int userID, int productionID, std::string photoSource, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".photos(photo_id, user_id, product_id, photo_source) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(photoID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionID);
		sqlCommand += ", '";
		sqlCommand += photoSource;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the photo creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create postion
	bool OrmasDal::CreatePosition(int positionID, std::string positionName, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".positions(position_id, position_name) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(positionID);
		sqlCommand += ", '";
		sqlCommand += positionName;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the position creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create prices
	bool OrmasDal::CreatePrice(int priceID, std::string priceDate, double priceValue, int currencyID, int productID, bool priceIsOutdated, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".prices(price_id, price_date, price_value, currency_id, product_id, is_outdated) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(priceID);
		sqlCommand += ", '";
		sqlCommand += priceDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(priceValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += priceIsOutdated ? "TRUE" : "FALSE";
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the price creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateProductType(int productTypeID, std::string productTypeName, std::string productTypeShortName, 
		std::string productTypeCode, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".product_type(product_type_id, product_type_name,\
								  product_type_short_name, product_type_code) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(productTypeID);
		sqlCommand += ", '";
		sqlCommand += productTypeName;
		sqlCommand += "', '";
		sqlCommand += productTypeShortName;
		sqlCommand += "', '";
		sqlCommand += productTypeCode;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the ptoduct type creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	bool OrmasDal::CreateProduction(int productionID, std::string productionDate, std::string productionExpiryDate, std::string productionSessionStart,
		std::string productionSessionEnd, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".production(production_id, production_date,\
								 								  expiry_date, session_start, session_end) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(productionID);
		sqlCommand += ", '";
		sqlCommand += productionDate;
		sqlCommand += "', '";
		sqlCommand += productionExpiryDate;
		sqlCommand += "', '";
		sqlCommand += productionSessionStart;
		sqlCommand += "', '";
		sqlCommand += productionSessionEnd;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the ptoduct type creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	// Create item in production list
	bool OrmasDal::CreateProductionList(int productionListID, int productionID, int productID, double productionListCount, double productionListSum, 
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".production_list(production_list_id, production_id, product_id, count,\
								  sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(productionListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in production list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create an item in production consume raw list
	bool OrmasDal::CreateProductionConsumeRawList(int productionConsRawListID, int productionConsumeRawID, int productID, 
		double productionConsRawListCount, double productionConsRawListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".production_consume_raw_list(consume_raw_list_id, consume_raw_id, \
								 								 										product_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(productionConsRawListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionConsumeRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionConsRawListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionConsRawListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in consume raw list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateProductionConsumeRaw(int productionConsumeRawID, int userID, std::string productionConsumeRawDate,
		std::string productionConsumeRawExecutionDate, int stockEmployeeID, double productionConsumeRawCount,
		double productionConsumeRawSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".production_consume_raws(consume_raw_id, user_id, consume_raw_date, \
								 								 										execution_date, employee_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(productionConsumeRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", '";
		sqlCommand += productionConsumeRawDate;
		if (productionConsumeRawExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += productionConsumeRawExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionConsumeRawCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionConsumeRawSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the consume raw creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	// Create an item in production plan list
	bool OrmasDal::CreateProductionPlanList(int productionPlanListID, int productionPlanID, int productID, 
		double productionPlanListCount, double productionPlanListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".production_plan_list(production_plan_list_id, production_plan_id, \
								 product_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(productionPlanListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionPlanID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionPlanListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionPlanListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in production plan list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateProductionPlan(int productionPlanID, std::string productionPlanDate, int employeeID, 
		double productionPlanListCount, double productionPlanListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".production_plan(production_plan_id, production_plan_date, \
								 									employee_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(productionPlanID);
		sqlCommand += ", '";
		sqlCommand += productionPlanDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionPlanListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionPlanListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the production plan creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create production stock
	bool OrmasDal::CreateProductionStock(int productionStockID, int productID, double productionStockCount, double productionStockSum,
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".production_stock(production_stock_id, product_id, \
								 								 			count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(productionStockID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionStockCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionStockSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in production stock is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateProduct(int productID, int companyID, std::string productName, double productVolume, int measureID, double productPrice, 
		int productTypeID, int productShelfLife, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".products(product_id, company_id, product_name, volume, measure_id,\
								 price, product_type_id, shelf_life, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(companyID);
		sqlCommand += ", '";
		sqlCommand += productName;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(productVolume);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(measureID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productPrice);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productTypeID);
		sqlCommand += ",";
		sqlCommand += boost::lexical_cast<std::string>(productShelfLife);
		sqlCommand += ",";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the product creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create purveyor
	bool OrmasDal::CreatePurveyor(int userID, std::string purveyorCompanyName, int locationID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".purveyors(user_id, company_name, location_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", '";
		sqlCommand += purveyorCompanyName;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(locationID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the purveyor creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create an item in consume product list
	bool OrmasDal::CreateReceiptProductList(int receiptProductListID, int receiptProductID, int productID, double receiptProductCount,
		double receiptProductSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".receipt_product_list(receipt_product_list_id, receipt_product_id, \
								 										product_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(receiptProductListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptProductID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptProductCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptProductSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in receipt product list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateReceiptProduct(int receiptProductID, int employeeID, std::string receiptProductDate,
		std::string receiptProductExecutionDate, int stockEmployeeID, double receiptProductCount, double receiptProductSum,
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".receipt_products(receipt_product_id, user_id, receipt_product_date, \
								 										execution_date, employee_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(receiptProductID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", '";
		sqlCommand += receiptProductDate;
		if (receiptProductExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += receiptProductExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptProductCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptProductSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the receipt product creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create an item in consume raw list
	bool OrmasDal::CreateReceiptRawList(int receiptRawListID, int receiptRawID, int productID, double receiptRawListCount, 
		double receiptRawListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".receipt_raw_list(receipt_raw_list_id, receipt_raw_id, \
								 								 										product_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(receiptRawListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptRawListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptRawListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in receipt raw list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateReceiptRaw(int receiptRawID, int employeeID, std::string receiptRawDate, std::string receiptRawExecutionDate,
		int stockEmployeeID, double receiptRawCount, double receiptRawSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".receipt_raws(receipt_raw_id, user_id, receipt_raw_date, \
								 								 										execution_date, employee_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(receiptRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", '";
		sqlCommand += receiptRawDate;
		if (receiptRawExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += receiptRawExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptRawCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptRawSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the receiprt raw creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create refund
	bool OrmasDal::CreateRefund(int refundID, std::string refuntDate, double refundValue, int userID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".refunds(refund_id, refund_date, refund_value, user_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(refundID);
		sqlCommand += ", '";
		sqlCommand += refuntDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(refundValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the refund creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create relation type
	bool OrmasDal::CreateRelationType(int relationTypeID, std::string relationTypeName, std::string comment, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".relation_type(relation_type_id, relation_name, comment) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(relationTypeID);
		sqlCommand += ", '";
		sqlCommand += relationTypeName;
		sqlCommand += "', '";
		sqlCommand += comment;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the relation type creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create relation
	bool OrmasDal::CreateRelation(int relationID, int user1ID, int user2ID, int relationTypeID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".relations(relation_id, user_id_1, user_id_2, relation_type_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(relationID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(user1ID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(user2ID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(relationTypeID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the relation creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateReturnList(int returnListID, int returnID, int productID, double returnListCount, double returnListSum,
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".return_list(return_list_id, return_id, product_id, count,\
								 								  sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(returnListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(returnID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(returnListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(returnListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in return list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateReturn(int returnID, int clientID, std::string returnDate, std::string returnExecutionDate, int employeeID, 
		double returnCount, double returnSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			errorMessage = "SQL command for creation company is failed, please contact with application provider!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".returns(return_id, user_id, return_date, execution_date,  employee_id,\
								  count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(returnID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(clientID);
		sqlCommand += ", '";
		sqlCommand += returnDate;
		if (returnExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += returnExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(returnCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(returnSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the return creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	bool OrmasDal::CreateRole(int roleID, std::string roleCode, std::string roleName, std::string comment, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".roles(role_id, role_code, role_name, comment) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(roleID);
		sqlCommand += ", '";
		sqlCommand += roleCode;
		sqlCommand += "', '";
		sqlCommand += roleName;
		sqlCommand += "', '";
		sqlCommand += comment;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the role creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateSpecificationList(int specificationListID, int specificationID, int productID, double specificationListCount,
		std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".specification_list(specification_list_id, specification_id, \
			product_id, count) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(specificationListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(specificationID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(specificationListCount);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the specification list creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateSpecification(int specificationID, int productID, double specificationSum, int currencyID, int employeeID, 
		std::string sDate, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".specifications(specification_id, product_id, sum, currency_id, \
								 			employee_id, specification_date) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(specificationID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(specificationSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", '";
		sqlCommand += sDate;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the specification creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	bool OrmasDal::CreateSalary(int salaryID, int userID, double salaryValue, int currencyID, int salaryTypeID, 
		std::string salaryDate, bool salaryBonus, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".salaries(salary_id, user_id, salary_value, currency_id, \
			salary_type_id, salary_date, salary_bonus) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(salaryID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(salaryValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(salaryTypeID);
		sqlCommand += ", '";
		sqlCommand += salaryDate;
		sqlCommand += "', ";
		sqlCommand += salaryBonus ? "TRUE" : "FALSE";
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the salary creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateSalaryType(int salaryTypeID, std::string salaryTypeCode, std::string salaryTypeName, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".salary_type(salary_type_id, salary_type_code, salary_type_name) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(salaryTypeID);
		sqlCommand += ", '";
		sqlCommand += salaryTypeCode;
		sqlCommand += "', '";
		sqlCommand += salaryTypeName;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the salary type creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create an item in spoilage list
	bool OrmasDal::CreateSpoilageList(int spoilageListID, int spoilageID, int productID, double spoilageListCount, 
		double spoilageListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".spoilage_list(spoilage_list_id, spoilage_id, \
								 								 product_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(spoilageListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(spoilageID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(spoilageListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(spoilageListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in spoilage list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateSpoilage(int spoilageID, std::string spoilageDate, int employeeID, double spoilageCount, double spoilageSum, 
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".spoilage(spoilage_id, spoilage_date, \
								 							employee_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(spoilageID);
		sqlCommand += ", '";
		sqlCommand += spoilageDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(spoilageCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(spoilageSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the production plan creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	//Create state
	bool OrmasDal::CreateState(int stateID, int universalID, int statusID, std::string statusLastChange, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".state(state_id, universal_id, status_id, last_change) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(stateID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(universalID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", '";
		sqlCommand += statusLastChange;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the state creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateStatus(int statusID, std::string statusCode, std::string statusName, std::string comment, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".status(status_id, status_code, status_name, comment) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", '";
		sqlCommand += statusCode;
		sqlCommand += "', '";
		sqlCommand += statusName;
		sqlCommand += "', '";
		sqlCommand += comment;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the status creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	//Create status rule
	bool OrmasDal::CreateStatusRule(int statusRuleID, std::string statusRuleOperation, int statusID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".status_rule(status_rule_id, operation, status_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(statusRuleID);
		sqlCommand += ", '";
		sqlCommand += statusRuleOperation;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the status rule creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	// Create stock
	bool OrmasDal::CreateStock(int stockID, int productID, double stockCount, double stockSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".stock(stock_id, product_id, \
								 								 count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(stockID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(stockCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(stockSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in stock is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create subaccount
	bool OrmasDal::CreateSubaccount(int subaccountID, int accountID, std::string subaccountNumber, double subaccountStartBalance, 
		double subaccountCurrentBalance, int currencyID, int statusID, std::string subaccountOpenDate, 
		std::string subaccountCloseDate, std::string subaccountDetails, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".subaccounts(subaccount_id, account_id, subaccount_number, start_balance, \
											current_balance, currency_id, status_id, date_opened, date_closed, details) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(subaccountID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(accountID);
		sqlCommand += ", '";
		sqlCommand += subaccountNumber;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(subaccountStartBalance);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(subaccountCurrentBalance);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", '";
		sqlCommand += subaccountOpenDate;
		if (subaccountCloseDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", '";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += subaccountCloseDate;
			sqlCommand += "', '";
		}
		sqlCommand += subaccountDetails;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the account creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create subaccount history
	bool OrmasDal::CreateSubaccountHistory(int subaccountHistoryID, int subaccountID, double subaccountStartBalance, double subaccountCurrentBalance,
		std::string fromDate, std::string tillDate, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".subaccount_history(subaccount_history_id, subaccount_id, start_balance, \
								 											current_balance, from_date, till_date) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(subaccountHistoryID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(subaccountID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(subaccountStartBalance);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(subaccountCurrentBalance);
		sqlCommand += ", '";
		sqlCommand += fromDate;
		sqlCommand += "', '";
		sqlCommand += tillDate;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the subaccount history creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create tax 
	bool OrmasDal::CreateTax(int taxID, std::string taxName, std::string taxCode, double fixedValue, int percentValue,
		std::string formulaValue, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".taxes(tax_id, tax_name, tax_code, \
								 							fixed_value, percent_value, formula_value) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(taxID);
		sqlCommand += ", '";
		sqlCommand += taxName;
		sqlCommand += "', '";
		sqlCommand += taxCode;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(fixedValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(percentValue);
		sqlCommand += ", '";
		sqlCommand += formulaValue;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the tax creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create timesheet
	bool OrmasDal::CreateTimesheet(int timesheetID, int salaryID, double workedTime, std::string timesheetDate, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".timesheet(timesheet_id, salary_id, worked_time, timesheet_date) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(timesheetID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(salaryID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(workedTime);
		sqlCommand += ", '";
		sqlCommand += timesheetDate;
		sqlCommand += "');";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the timesheet creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create an item in transport list
	bool OrmasDal::CreateTransportList(int transportListID, int transportID, int productID, double transportListCount,
		double transportListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".transport_list(transport_list_id, transport_id, \
								 								 				product_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(transportListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(transportID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(transportListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(transportListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in transport list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateTransport(int transportID, int employeeID, std::string transportDate, std::string transportExecutionDate,
		int stockEmployeeID, double transportCount, double transportSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".transports(transport_id, user_id, transport_date, \
								 							execution_date, employee_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(transportID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", '";
		sqlCommand += transportDate;
		if (transportExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += transportExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(transportCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(transportSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the transport creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateUser(int userID, std::string userEmail, std::string userName, std::string userSurname, std::string userPhone,
		std::string userAddress, int roleID, std::string userPassword, bool userActivated, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".users(user_id, user_email, user_name, user_surname, user_phone, \
								 user_address, role_id, password, activated) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", '";
		sqlCommand += userEmail;
		sqlCommand += "', '";
		sqlCommand += userName;
		sqlCommand += "', '";
		sqlCommand += userSurname;
		sqlCommand += "', '";
		sqlCommand += userPhone;
		sqlCommand += "', '";
		sqlCommand += userAddress;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(roleID);
		sqlCommand += ", '";
		sqlCommand += userPassword;
		sqlCommand += "', ";
		sqlCommand += userActivated ? "TRUE" : "FALSE";
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the user creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	// Create withdrawal
	bool OrmasDal::CreateWithdrawal(int withdrawalID, std::string withdrawalDate, double withdrawalValue, int userID,
		int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".withdrawals(withdrawal_id, withdrawal_date, withdrawal_value, user_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(withdrawalID);
		sqlCommand += ", '";
		sqlCommand += withdrawalDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(withdrawalValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the withdrawal creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create an item in write-off list
	bool OrmasDal::CreateWriteOffList(int writeOffListID, int writeOffID, int productID, double writeOffListCount, double writeOffListSum,
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".write_off_list(write_off_list_id, write_off_id, product_id, count, \
																							sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(writeOffListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in order list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateWriteOff(int writeOffID, int userID, std::string writeOffDate, int employeeID, double writeOffCount, 
		double writeOffSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".write_offs(write_off_id, user_id, write_off_date, employee_id, count,\
			sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(writeOffID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", '";
		sqlCommand += writeOffDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the order creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Create an item in write-off raw list
	bool OrmasDal::CreateWriteOffRawList(int writeOffRawListID, int writeOffRawID, int productID, double writeOffRawListCount, 
		double writeOffRawListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".write_off_raw_list(write_off_raw_list_id, write_off_raw_id, \
								 								 								 				product_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(writeOffRawListID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffRawListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffRawListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the item creation in write-off raw list is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::CreateWriteOffRaw(int writeOffRawID, int employeeID, std::string writeOffRawListDate, int stockEmployeeID,
		double writeOffRawListCount, double writeOffRawListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "INSERT INTO \"OrmasSchema\".write_off_raws(write_off_raw_id, user_id, write_off_raw_date, \
								 								 							 employee_id, count, sum, status_id, currency_id) VALUES(";
		sqlCommand += boost::lexical_cast<std::string>(writeOffRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", '";
		sqlCommand += writeOffRawListDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffRawListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffRawListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ");";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command for the write-off raw creation is failed, please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}


	// Delete section----------------------------------------------------------------
	// Delete access item
	bool OrmasDal::DeleteAccessItem(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".access_items where access_item_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete access item! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete access 
	bool OrmasDal::DeleteAccess(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".accesses where access_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete access! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	
	
	// Delete account type
	bool OrmasDal::DeleteAccountType(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".account_type where account_type_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete account type! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete account
	bool OrmasDal::DeleteAccount(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".accounts where account_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete account! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete account history
	bool OrmasDal::DeleteAccountHistory(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".account_history where account_history_id = ";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete account history! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete balance-payment
	bool OrmasDal::DeleteBalancePayment(int bID, int pID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".balance_payment where ";
		if (0 == pID)
		{
			sqlCommand += "balance_id = ";
			sqlCommand += boost::lexical_cast<std::string>(bID);
			sqlCommand += ";";
		}
		if (0 == bID)
		{
			sqlCommand += "payment_id = ";
			sqlCommand += boost::lexical_cast<std::string>(pID);
			sqlCommand += ";";
		}
		if (0 != bID && 0 != pID)
		{
			sqlCommand += "balance_id = ";
			sqlCommand += boost::lexical_cast<std::string>(bID);
			sqlCommand += " AND ";
			sqlCommand += "payment_id = ";
			sqlCommand += boost::lexical_cast<std::string>(pID);
			sqlCommand += ";";
		}
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete balance-payment relation! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete balance-payslip
	bool OrmasDal::DeleteBalancePayslip(int bID, int pID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".balance_payslip where ";
		if (0 == pID)
		{
			sqlCommand += "balance_id = ";
			sqlCommand += boost::lexical_cast<std::string>(bID);
			sqlCommand += ";";
		}
		if (0 == bID)
		{
			sqlCommand += "payslip_id = ";
			sqlCommand += boost::lexical_cast<std::string>(pID);
			sqlCommand += ";";
		}
		if (0 != bID && 0 != pID)
		{
			sqlCommand += "balance_id = ";
			sqlCommand += boost::lexical_cast<std::string>(bID);
			sqlCommand += " AND ";
			sqlCommand += "payslip_id = ";
			sqlCommand += boost::lexical_cast<std::string>(pID);
			sqlCommand += ";";
		}
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete balance-payslip relation! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete balance-refund
	bool OrmasDal::DeleteBalanceRefund(int bID, int rID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".balance_refund where ";
		if (0 == rID)
		{
			sqlCommand += "balance_id = ";
			sqlCommand += boost::lexical_cast<std::string>(bID);
			sqlCommand += ";";
		}
		if (0 == bID)
		{
			sqlCommand += "refund_id = ";
			sqlCommand += boost::lexical_cast<std::string>(rID);
			sqlCommand += ";";
		}
		if (0 != bID && 0 != rID)
		{
			sqlCommand += "balance_id = ";
			sqlCommand += boost::lexical_cast<std::string>(bID);
			sqlCommand += " AND ";
			sqlCommand += "refund_id = ";
			sqlCommand += boost::lexical_cast<std::string>(rID);
			sqlCommand += ";";
		}
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete balance-refund relation! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete balance-withdrawal
	bool OrmasDal::DeleteBalanceWithdrawal(int bID, int wID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".balance_withdrawal where ";
		if (0 == wID)
		{
			sqlCommand += "balance_id = ";
			sqlCommand += boost::lexical_cast<std::string>(bID);
			sqlCommand += ";";
		}
		if (0 == bID)
		{
			sqlCommand += "withdrawal_id = ";
			sqlCommand += boost::lexical_cast<std::string>(wID);
			sqlCommand += ";";
		}
		if (0 != bID && 0 != wID)
		{
			sqlCommand += "balance_id = ";
			sqlCommand += boost::lexical_cast<std::string>(bID);
			sqlCommand += " AND ";
			sqlCommand += "withdrawal_id = ";
			sqlCommand += boost::lexical_cast<std::string>(wID);
			sqlCommand += ";";
		}
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete balance-withdrawal relation! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete balance
	bool OrmasDal::DeleteBalance(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".balances where balance_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete balance! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}
	
	// Delete chart of account
	bool OrmasDal::DeleteChartOfAccount(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".chart_of_accounts where chart_of_account_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete chart of account! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete clients
	bool OrmasDal::DeleteClient(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".clients where user_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete client! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}
	// Delete company
	bool OrmasDal::DeleteCompany(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".companies where company_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete company! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete company-account
	bool OrmasDal::DeleteCompanyAccount(int cID, int aID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".company_account where ";
		if (0 == cID)
		{
			sqlCommand += "company_id = ";
			sqlCommand += boost::lexical_cast<std::string>(cID);
			sqlCommand += ";";
		}
		if (0 == aID)
		{
			sqlCommand += "account_id = ";
			sqlCommand += boost::lexical_cast<std::string>(aID);
			sqlCommand += ";";
		}
		if (0 != cID && 0 != aID)
		{
			sqlCommand += "company_id = ";
			sqlCommand += boost::lexical_cast<std::string>(cID);
			sqlCommand += " AND ";
			sqlCommand += "account_id = ";
			sqlCommand += boost::lexical_cast<std::string>(aID);
			sqlCommand += ";";
		}
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete company-account relation! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete company-employee
	bool OrmasDal::DeleteCompanyEmployee(int cID, int eID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".company_employee where ";
		if (0 == cID)
		{
			sqlCommand += "company_id = ";
			sqlCommand += boost::lexical_cast<std::string>(cID);
			sqlCommand += ";";
		}
		if (0 == eID)
		{
			sqlCommand += "employee_id = ";
			sqlCommand += boost::lexical_cast<std::string>(eID);
			sqlCommand += ";";
		}
		if (0 != cID && 0 != eID)
		{
			sqlCommand += "company_id = ";
			sqlCommand += boost::lexical_cast<std::string>(cID);
			sqlCommand += " AND ";
			sqlCommand += "employee_id = ";
			sqlCommand += boost::lexical_cast<std::string>(eID);
			sqlCommand += ";";
		}
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete company-employee relation! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}
	
	// Delete item in consume product list
	bool OrmasDal::DeleteItemInConsumeProductList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".consume_product_list where consume_product_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from consume product list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete list in consume product list by consume product id
	bool OrmasDal::DeleteListByConsumeProductID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".consume_product_list where consume_product_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from consume product list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete consume product
	bool OrmasDal::DeleteConsumeProduct(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}

		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".consume_products where consume_product_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete consume product! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	
	// Delete item in consume raw list
	bool OrmasDal::DeleteItemInConsumeRawList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".consume_raw_list where consume_raw_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from consume raw list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete list in consume raw list by consume raw id
	bool OrmasDal::DeleteListByConsumeRawID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".consume_raw_list where consume_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from consume product list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete consume raw
	bool OrmasDal::DeleteConsumeRaw(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}

		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".consume_raws where consume_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete consume raw! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Delete currency
	bool OrmasDal::DeleteCurrency(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".currencies where currency_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete currency! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete division-account
	bool OrmasDal::DeleteDivisionAccount(int divisionID, int accountID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".division_account where ";
		if (0 == divisionID)
		{
			sqlCommand += "division_id = ";
			sqlCommand += boost::lexical_cast<std::string>(divisionID);
			sqlCommand += ";";
		}
		if (0 == accountID)
		{
			sqlCommand += "account_id = ";
			sqlCommand += boost::lexical_cast<std::string>(accountID);
			sqlCommand += ";";
		}
		if (0 != divisionID && 0 != accountID)
		{
			sqlCommand += "division_id = ";
			sqlCommand += boost::lexical_cast<std::string>(divisionID);
			sqlCommand += " AND ";
			sqlCommand += "account_id = ";
			sqlCommand += boost::lexical_cast<std::string>(accountID);
			sqlCommand += ";";
		}
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete division-account relation! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete division-employee
	bool OrmasDal::DeleteDivisionEmployee(int divisionID, int employeeID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".division_employee where ";
		if (0 == divisionID)
		{
			sqlCommand += "division_id = ";
			sqlCommand += boost::lexical_cast<std::string>(divisionID);
			sqlCommand += ";";
		}
		if (0 == employeeID)
		{
			sqlCommand += "employee_id = ";
			sqlCommand += boost::lexical_cast<std::string>(employeeID);
			sqlCommand += ";";
		}
		if (0 != divisionID && 0 != employeeID)
		{
			sqlCommand += "division_id = ";
			sqlCommand += boost::lexical_cast<std::string>(divisionID);
			sqlCommand += " AND ";
			sqlCommand += "employee_id = ";
			sqlCommand += boost::lexical_cast<std::string>(employeeID);
			sqlCommand += ";";
		}
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete division-employee relation! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}
	// Delete division
	bool OrmasDal::DeleteDivision(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".divisions where division_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete division! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete employee
	bool OrmasDal::DeleteEmployee(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".employees where user_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete employee! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete entries
	bool OrmasDal::DeleteEntry(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".entries where entry_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete entry! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete entry routing
	bool OrmasDal::DeleteEntryRouting(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".entry_routing where entry_routing_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete entry routing! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete entry-subaccount 
	bool OrmasDal::DeleteEntrySubaccount(int eID, int sID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".entry_subaccount where ";
		if (0 == eID)
		{
			sqlCommand += "entry_id = ";
			sqlCommand += boost::lexical_cast<std::string>(eID);
			sqlCommand += ";";
		}
		if (0 == sID)
		{
			sqlCommand += "subaccount_id = ";
			sqlCommand += boost::lexical_cast<std::string>(sID);
			sqlCommand += ";";
		}
		if (0 != sID && 0 != eID)
		{
			sqlCommand += "subaccount_id = ";
			sqlCommand += boost::lexical_cast<std::string>(sID);
			sqlCommand += " AND ";
			sqlCommand += "entry_id = ";
			sqlCommand += boost::lexical_cast<std::string>(eID);
			sqlCommand += ";";
		}
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete entry-subaccount relation! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete financial report
	bool OrmasDal::DeleteFinancialReport(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".financial_report where financial_report_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete financial report! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete item in inventorization list
	bool OrmasDal::DeleteItemInInventorizationList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".inventorization_list where inventorization_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from inventorization list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete list in inventorization list by inventorization id
	bool OrmasDal::DeleteListByInventorizationID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".inventorization_list where inventorization_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from inventorization list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete inventorization
	bool OrmasDal::DeleteInventorization(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}

		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".inventorizations where inventorization_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete inventorization! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Delete jobprice
	bool OrmasDal::DeleteJobprice(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".jobprice where jobprice_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete jobprice! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete jobsheet
	bool OrmasDal::DeleteJobsheet(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".jobsheet where jobsheet_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete jobsheet! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete Location
	bool OrmasDal::DeleteLocation(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".locations where location_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete location! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	

	// Delete measure
	bool OrmasDal::DeleteMeasure(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".measures where measure_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete measure! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete net cost
	bool OrmasDal::DeleteNetCost(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".net_cost where net_cost_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete net cost! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}
	
	// Delete item in order list
	bool OrmasDal::DeleteItemInOrderList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".order_list where order_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from order list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}
	
	// Delete list in order list by order id
	bool OrmasDal::DeleteListByOrderID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".order_list where order_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from order list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete order
	bool OrmasDal::DeleteOrder(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".orders where order_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete order! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Delete item in order raw list
	bool OrmasDal::DeleteItemInOrderRawList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".order_raw_list where order_raw_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from order raw list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete list in order raw list by order raw id
	bool OrmasDal::DeleteListByOrderRawID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".order_raw_list where order_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from order raw list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete order
	bool OrmasDal::DeleteOrderRaw(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}

		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".order_raws where order_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete order raw! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	// Delete payment
	bool OrmasDal::DeletePayment(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".payments where payment_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete payment! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete payment
	bool OrmasDal::DeletePayslip(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".payslips where payslip_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete payslip! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete payment-employee
	bool OrmasDal::DeletePaymentEmployee(int pID, int eID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".payment_employee where ";
		if (0 == pID)
		{
			sqlCommand += "payment_id = ";
			sqlCommand += boost::lexical_cast<std::string>(pID);
			sqlCommand += ";";
		}
		if (0 == eID)
		{
			sqlCommand += "employee_id = ";
			sqlCommand += boost::lexical_cast<std::string>(eID);
			sqlCommand += ";";
		}
		if (0 != pID && 0 != eID)
		{
			sqlCommand += "payment_id = ";
			sqlCommand += boost::lexical_cast<std::string>(pID);
			sqlCommand += " AND ";
			sqlCommand += "employee_id = ";
			sqlCommand += boost::lexical_cast<std::string>(eID);
			sqlCommand += ";";
		}
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete payment-employee relation! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete payslip-order
	bool OrmasDal::DeletePayslipOrder(int pID, int oID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".payslip_order where ";
		if (0 == pID)
		{
			sqlCommand += "payslip_id = ";
			sqlCommand += boost::lexical_cast<std::string>(pID);
			sqlCommand += ";";
		}
		if (0 == oID)
		{
			sqlCommand += "order_id = ";
			sqlCommand += boost::lexical_cast<std::string>(oID);
			sqlCommand += ";";
		}
		if (0 != pID && 0 != oID)
		{
			sqlCommand += "payslip_id = ";
			sqlCommand += boost::lexical_cast<std::string>(pID);
			sqlCommand += " AND ";
			sqlCommand += "order_id = ";
			sqlCommand += boost::lexical_cast<std::string>(oID);
			sqlCommand += ";";
		}
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete payslip-order relation! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete percent rate
	bool OrmasDal::DeletePercentRate(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".percent_rate where percent_rate_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete percent rate! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete photo
	bool OrmasDal::DeletePhoto(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".photos where photo_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete photo! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete position
	bool OrmasDal::DeletePosition(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".positions where position_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete position! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete price
	bool OrmasDal::DeletePrice(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".prices where price_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete price! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}


	// Delete product type
	bool OrmasDal::DeleteProductType(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".product_type where product_type_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		
		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete product type! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete production
	bool OrmasDal::DeleteProduction(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".production where production_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete product type! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}
	
	// Delete item in production list
	bool OrmasDal::DeleteItemInProductionList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".production_list where production_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item in production list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete item in production consume raw list
	bool OrmasDal::DeleteItemInProductionConsumeRawList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".production_consume_raw_list where consume_raw_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from production consume raw list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete list in production consume raw list by consume raw id
	bool OrmasDal::DeleteListByProductionConsumeRawID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".production_consume_raw_list where consume_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from production consume raw list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete production consume raw
	bool OrmasDal::DeleteProductionConsumeRaw(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}

		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".production_consume_raws where consume_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete production consume raw! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Delete list in production list by production id
	bool OrmasDal::DeleteListByProductionID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".production_list where production_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from production list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete item in production plan list
	bool OrmasDal::DeleteItemInProductionPlanList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".production_plan_list where production_plan_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from production plan list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete list in production plan list by order id
	bool OrmasDal::DeleteListByProductionPlanID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".production_plan_list where production_plan_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from production plan list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete order
	bool OrmasDal::DeleteProductionPlan(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}

		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".production_plan where production_plan_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete production plan! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Delete item in production stock
	bool OrmasDal::DeleteProductionStock(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".production_stock where production_stock_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from production stock! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete product
	bool OrmasDal::DeleteProduct(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".products where product_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		
		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete product! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete product
	bool OrmasDal::DeletePurveyor(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".purveyors where user_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete purveyors! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}
	
	// Delete item in receipt product list
	bool OrmasDal::DeleteItemInReceiptProductList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".receipt_product_list where receipt_product_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from receipt product list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete list in receipt product list by receipt product id
	bool OrmasDal::DeleteListByReceiptProductID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".receipt_product_list where receipt_product_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from receipt product list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete consume product
	bool OrmasDal::DeleteReceiptProduct(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}

		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".receipt_products where receipt_product_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete receipt product! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}


	// Delete list in receipt raw list by receipt raw id
	bool OrmasDal::DeleteListByReceiptRawID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".receipt_raw_list where receipt_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from receipt raw list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete item in receipt raw list
	bool OrmasDal::DeleteItemInReceiptRawList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".receipt_raw_list where receipt_raw_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from receipt raw list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete receipt raw
	bool OrmasDal::DeleteReceiptRaw(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}

		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".receipt_raws where receipt_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete receipt raw! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Delete refund
	bool OrmasDal::DeleteRefund(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".refunds where payslip_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete refund! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete relation type
	bool OrmasDal::DeleteRelationType(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".relation_type where relation_type_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete relation type! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete relation
	bool OrmasDal::DeleteRelation(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".relations where relation_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete location! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete item in return list
	bool OrmasDal::DeleteItemInReturnList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".return_list where return_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item in return list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}
	// Delete list in return list by return id
	bool OrmasDal::DeleteListByReturnID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".return_list where return_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from return list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete return
	bool OrmasDal::DeleteReturn(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		
		PGresult * result;
		//cascading detele
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".returns where return_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete return! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	// Delete role
	bool OrmasDal::DeleteRole(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".roles where role_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		
		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete role! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}
	
	// Delete salary
	bool OrmasDal::DeleteSalary(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".salaries where salary_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete salary! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete salary type
	bool OrmasDal::DeleteSalaryType(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".salary_type where salary_type_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete salary type! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete item in specification list
	bool OrmasDal::DeleteItemInSpecificationList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".specification_list where specification_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item in specification list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}
	// Delete list in specification list by specification id
	bool OrmasDal::DeleteListBySpecificationID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".specification_list where specification_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from specification list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete return
	bool OrmasDal::DeleteSpecification(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}

		PGresult * result;
		//cascading detele
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".specifications where specification_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete specification! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}


	// Delete item in spoilage list
	bool OrmasDal::DeleteItemInSpoilageList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".spoilage_list where spoilage_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from spoilage list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete list in spoilage list by order id
	bool OrmasDal::DeleteListBySpoilageID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".spoilage_list where spoilage_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from spoilage list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete spoilage
	bool OrmasDal::DeleteSpoilage(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}

		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".spoilage where spoilage_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete spoilage! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Delete state
	bool OrmasDal::DeleteState(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".state where state_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete state! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete status
	bool OrmasDal::DeleteStatus(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".status where status_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		
		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete status! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	bool OrmasDal::DeleteStatusRule(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".status_rule where status_rule_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete status rule! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}
	
	// Delete item in stock
	bool OrmasDal::DeleteStock(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".stock where stock_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from stock! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete subaccount
	bool OrmasDal::DeleteSubaccount(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".subaccounts where subaccount_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete subaccount! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete subaccount history
	bool OrmasDal::DeleteSubaccountHistory(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".subaccount_history where subaccount_history_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete subaccount history! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete tax
	bool OrmasDal::DeleteTax(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".taxes where tax_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{

			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);PQclear(result);
			errorMessage = "Could not delete tax! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete timesheet
	bool OrmasDal::DeleteTimesheet(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".timesheet where timesheet_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete timesheet! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete item in transport list
	bool OrmasDal::DeleteItemInTransportList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".transport_list where transport_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from transport list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete list in transport list by transport id
	bool OrmasDal::DeleteListByTransportID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".transport_list where transport_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from transport list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete transport
	bool OrmasDal::DeleteTransport(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}

		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".transports where transport_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete transport! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Delete user
	bool OrmasDal::DeleteUser(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".users where user_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		
		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete user! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete payment
	bool OrmasDal::DeleteWithdrawal(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".withdrawals where withdrawal_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete withdrawal! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}
	
	// Delete item in write-off list
	bool OrmasDal::DeleteItemInWriteOffList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".write_off_list where write_off_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from write-off list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	bool OrmasDal::DeleteListByWriteOffID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".write_off_list where write_off_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from write-off list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete write-off
	bool OrmasDal::DeleteWriteOff(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".write_offs where write_off_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete write-off! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::DeleteListByWriteOffRawID(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".write_off_raw_list where write_off_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from write-off raw list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}


	// Delete item in write-off raw list
	bool OrmasDal::DeleteItemInWriteOffRawList(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".write_off_raw_list where write_off_raw_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) == PGRES_COMMAND_OK)
		{
			PQclear(result);
			return true;
		}
		else
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete item from write off raw list! SQL command is failed. Please contact with application provider!";
			return false;
		}
	}

	// Delete write-off raw
	bool OrmasDal::DeleteWriteOffRaw(int id, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}

		PGresult * result;
		std::string sqlCommand = "DELETE FROM \"OrmasSchema\".write_off_raws where write_off_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(id);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());
		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "Could not delete write off raw! SQL command is failed. Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Update section----------------------------------------------------------------
	
	bool OrmasDal::UpdateAccessItem(int accessItemID, std::string accessItemEng, std::string accessItemRu, std::string accessItemDivision, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".access_items SET(access_item_eng, access_item_ru, access_division) = ('";
		sqlCommand += accessItemEng;
		sqlCommand += "', '";
		sqlCommand += accessItemRu;
		sqlCommand += "', '";
		sqlCommand += accessItemDivision;
		sqlCommand += "') WHERE access_item_id=";
		sqlCommand += boost::lexical_cast<std::string>(accessItemID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the access item with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(accessItemID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	//Update access
	bool OrmasDal::UpdateAccess(int accessID, int roleID, int accessItemID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".accesses SET(role_id, access_item_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(roleID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(accessItemID);
		sqlCommand += ") WHERE access_id=";
		sqlCommand += boost::lexical_cast<std::string>(accessID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the access with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(accessID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	//Update account type
	bool OrmasDal::UpdateAccountType(int accountTypeID, std::string accountTypeName, int accountTypeNumber, std::string accountTypeComment, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".account_type SET(account_type_name, account_type_number,commnet) = ('";
		sqlCommand += accountTypeName;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(accountTypeNumber);
		sqlCommand += ", '";
		sqlCommand += accountTypeComment;
		sqlCommand += "') WHERE account_type_id=";
		sqlCommand += boost::lexical_cast<std::string>(accountTypeID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the accout type with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(accountTypeID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	//Update account
	bool OrmasDal::UpdateAccount(int accountID, std::string accountNumber, double accountStartBalance, double accountCurrentBalance, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".accounts SET(account_number, start_balance, \
								 								  current_balance) = ('";
		sqlCommand += accountNumber;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(accountStartBalance);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(accountCurrentBalance);
		sqlCommand += ") WHERE account_id=";
		sqlCommand += boost::lexical_cast<std::string>(accountID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the account with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(accountID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	//Update account history
	bool OrmasDal::UpdateAccountHistory(int accountHistoryID, int accountID, std::string accountNumber, double accountStartBalance,
		double accountCurrentBalance, std::string fromDate, std::string tillDate, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".account_history SET(account_id, account_number, start_balance, \
								 								  current_balance, from_date, till_date) = (";
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(accountID);
		sqlCommand += ", '";
		sqlCommand += accountNumber;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(accountStartBalance);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(accountCurrentBalance);
		sqlCommand += ", '";
		sqlCommand += fromDate;
		sqlCommand += "', '";
		sqlCommand += tillDate;
		sqlCommand += "') WHERE account_history_id=";
		sqlCommand += boost::lexical_cast<std::string>(accountHistoryID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the account history with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(accountHistoryID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	//Update balance
	bool OrmasDal::UpdateBalance(int balanceID, int userID, int subaccountID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".balances SET(user_id, subaccount_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(subaccountID);
		sqlCommand += ") WHERE balance_id=";
		sqlCommand += boost::lexical_cast<std::string>(balanceID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the balance with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(balanceID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	//Update chart of accounts
	bool OrmasDal::UpdateChartOfAccount(int chartOfAccountID, std::string chartOfAccountNumber, std::string chartOfAccountName, 
		int accountTypeID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".chart_of_accounts SET(number_of_account, name_of_account, account_type_id) = ('";
		sqlCommand += chartOfAccountNumber;
		sqlCommand += "', '";
		sqlCommand += chartOfAccountName;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(accountTypeID);
		sqlCommand += ") WHERE chart_of_account_id=";
		sqlCommand += boost::lexical_cast<std::string>(chartOfAccountID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the balance with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(chartOfAccountID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	bool OrmasDal::UpdateClient(int userID, std::string firm, std::string firmNumber, int locationID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".clients SET(firm, firm_number, location_id) = ('";
		sqlCommand += firm;
		sqlCommand += "', '";
		sqlCommand += firmNumber;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(locationID);
		sqlCommand += ") WHERE user_id=";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the client with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(userID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Update company
	bool OrmasDal::UpdateCompany(int companyID, std::string companyName, std::string companyAddress, std::string companyPhone, 
		std::string comment, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".companies SET(company_name, company_address, company_phone,\
			comment) = ('";
		sqlCommand += companyName;
		sqlCommand += "', '";
		sqlCommand += companyAddress;
		sqlCommand += "', '";
		sqlCommand += companyPhone;
		sqlCommand += "', '";
		sqlCommand += comment;
		sqlCommand += "') WHERE company_id=";
		sqlCommand += boost::lexical_cast<std::string>(companyID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the company with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(companyID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Update company-employee
	bool OrmasDal::UpdateCompanyAccount(int companyAccountID, int companyID, int accountID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".company_account SET(company_id, account_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(companyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(accountID);
		sqlCommand += ") WHERE company_account_id=";
		sqlCommand += boost::lexical_cast<std::string>(companyAccountID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the company-account relation with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(companyAccountID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Update company-employee
	bool OrmasDal::UpdateCompanyEmployee(int companyEmployeeID, int companyID, int employeeID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".company_employee SET(company_id, employee_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(companyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ") WHERE company_employee_id=";
		sqlCommand += boost::lexical_cast<std::string>(companyEmployeeID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the company-employee relation with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(companyEmployeeID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	bool OrmasDal::UpdateConsumeProductList(int consumeProductListID, int consumeProductID, int productID, double consumeListCount,
		double consumeListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".consume_product_list SET(consume_product_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(consumeProductID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE consume_product_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(consumeProductListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the item in consume product list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(consumeProductListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateConsumeProduct(int consumeProductID, int employeeID, std::string consumeProductDate, std::string consumeProductExecutionDate,
		int stockEmployeeID, double consumeProductCount, double consumeProductSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".consume_products SET(user_id, consume_product_date, execution_date, employee_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", '";
		sqlCommand += consumeProductDate;
		if (consumeProductExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += consumeProductExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeProductCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeProductSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE consume_product_id=";
		sqlCommand += boost::lexical_cast<std::string>(consumeProductID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the consume product with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(consumeProductID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateConsumeRawList(int consuleRawListID, int consumeRawID, int productID, double consumeRawListCount, double consumeRawListSum,
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".consume_raw_list SET(consume_raw_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(consumeRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeRawListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeRawListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE consume_raw_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(consuleRawListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the item in consume raw list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(consuleRawListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateConsumeRaw(int consumeRawID, int userID, std::string consumeRawDate, std::string consumeRawExecutionDate,
		int stockEmployeeID, double consumeRawaCount, double conusmeRawSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".consume_raws SET(user_id, consume_raw_date, execution_date, employee_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", '";
		sqlCommand += consumeRawDate;
		if (consumeRawExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += consumeRawExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(consumeRawaCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(conusmeRawSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE consume_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(consumeRawID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the consume raw with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(consumeRawID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateCurrency(int currenycID, int currencyCode, std::string currencyShortName, std::string currencyName, int currencyUnit,
		bool cMainTrade, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".currencies SET(currency_code, currency_short_name, \
								  currency_name, currency_unit, currency_main_trade) = (";
		sqlCommand += boost::lexical_cast<std::string>(currencyCode);
		sqlCommand += ", '";
		sqlCommand += currencyShortName;
		sqlCommand += "', '";
		sqlCommand += currencyName;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(currencyUnit);
		sqlCommand += ", ";
		sqlCommand += cMainTrade ? "TRUE" : "FALSE";
		sqlCommand += ") WHERE currency_id=";
		sqlCommand += boost::lexical_cast<std::string>(currenycID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the currency with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(currenycID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateDivisionAccount(int divisionAccountID, int divisionID, int accountID, std::string accountCode, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".division_account SET(division_id, account_id, account_code) = (";
		sqlCommand += boost::lexical_cast<std::string>(divisionID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(accountID);
		sqlCommand += ", '";
		sqlCommand += accountCode;
		sqlCommand += "') WHERE division_account_id=";
		sqlCommand += boost::lexical_cast<std::string>(divisionAccountID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the division-account with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(divisionAccountID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateDivisionEmployee(int divisionEmployeeID, int divisionID, int employeeID, bool isContract, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".division_account SET(division_id, account_id, account_code) = (";
		sqlCommand += boost::lexical_cast<std::string>(divisionID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", ";
		sqlCommand += isContract ? "TRUE" : "FALSE";
		sqlCommand += ") WHERE division_employee_id=";
		sqlCommand += boost::lexical_cast<std::string>(divisionEmployeeID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the division-account with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(divisionEmployeeID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	bool OrmasDal::UpdateDivision(int divisionID, std::string divisionName, std::string divisionCode, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".divisions SET(division_name, division_code) = ('";
		sqlCommand += divisionName;
		sqlCommand += "', '";
		sqlCommand += divisionCode;
		sqlCommand += "') WHERE division_id=";
		sqlCommand += boost::lexical_cast<std::string>(divisionID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the division with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(divisionID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}


	bool OrmasDal::UpdateEmployee(int userID, int positionID, std::string birthDate, std::string hireDate, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".employees SET(position_id, birth_date, hire_date) = (";
		sqlCommand += boost::lexical_cast<std::string>(positionID);
		sqlCommand += ", '";
		sqlCommand += birthDate;
		sqlCommand += "', '";
		sqlCommand += hireDate;
		sqlCommand += "') WHERE user_id=";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the employee with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(userID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateEntry(int entryID, std::string entryDate, int debbitingAccountID, double entryValue, int creditingAccountID, 
		std::string description, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".entries SET(entry_date, debiting_account_id, value, crediting_account_id, description) = ('";
		sqlCommand += entryDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(debbitingAccountID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(entryValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(creditingAccountID);
		sqlCommand += ", '";
		sqlCommand += description;
		sqlCommand += "') WHERE entry_id=";
		sqlCommand += boost::lexical_cast<std::string>(entryID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the entry with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(entryID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateEntryRouting(int entryRoutingID, std::string entryRoutingOperation, int debbitingAccountID, int creditingAccountID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".entry_routing SET(operation_name, debit, credit) = ('";
		sqlCommand += entryRoutingOperation;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(debbitingAccountID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(creditingAccountID);
		sqlCommand += ") WHERE entry_routing_id=";
		sqlCommand += boost::lexical_cast<std::string>(entryRoutingID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the entry routing with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(entryRoutingID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateFinancialReport(int financialReportID, double account_44010, double account_55010, double account_552,
		double account_55270, double account_553, double account_55321, double account_44020_90, double account_66010_66110,
		double account_66020_66120, double account_66040_66140, double account_66050_66150, double account_66060_66160,
		double account_66130, double account_66070_66170, double tax, std::string fromDate, std::string tillDate, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".financial_report SET(acc_44010, acc_55010, acc_552, acc_55270, acc_553, \
								 acc_55321, acc_44020_90, acc_66010_66110, acc_66020_66120, acc_66040_66140, acc_66050_66150,\
								 acc_66060_66160, acc_66130,acc_66070_66170, tax, from_date, till_date) = (";
		sqlCommand += boost::lexical_cast<std::string>(account_44010);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_55010);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_552);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_55270);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_553);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_55321);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_44020_90);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_66010_66110);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_66020_66120);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_66040_66140);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_66050_66150);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_66060_66160);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_66130);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(account_66070_66170);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(tax);
		sqlCommand += ", '";
		sqlCommand += fromDate;
		sqlCommand += "', '";
		sqlCommand += tillDate;
		sqlCommand += "') WHERE financial_report_id=";
		sqlCommand += boost::lexical_cast<std::string>(financialReportID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the account history with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(financialReportID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateInventorizationList(int inventorizationListID, int inventorizationID, int productID, double inventorizationListCount,
		double inventorizationListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".inventorization_list SET(inventorization_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(inventorizationID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(inventorizationListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(inventorizationListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE inventorization_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(inventorizationListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the item in inventorization list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(inventorizationListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateInventorization(int inventorizationID, int userID, std::string inventorizationDate, std::string inventorizationExecutionDate,
		int stockEmployeeID, double inventorizationCount, double inventorizationSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".inventorizations SET(user_id, inventorization_date, execution_date, employee_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", '";
		sqlCommand += inventorizationDate;
		if (inventorizationExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += inventorizationExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(inventorizationCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(inventorizationSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE inventorization_id=";
		sqlCommand += boost::lexical_cast<std::string>(inventorizationID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the inventorization with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(inventorizationID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateJobprice(int jobpriceID, int productID, double jobpriceValue, int currencyID, double jobpriceVolume, 
		int measureID, int positionID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".jobprice SET(product_id, value, currency_id, volume, measure_id,\
								 								 								 position_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(jobpriceValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(jobpriceVolume);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(measureID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(positionID);
		sqlCommand += ") WHERE jobprice_id=";
		sqlCommand += boost::lexical_cast<std::string>(jobpriceID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the jobprice with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(jobpriceID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateJobsheet(int jobsheetID, std::string jobsheetDate, double jobsheetCount,
		int productID, int employeeID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".jobsheet SET(jobsheet_date, count, product_id, employee_id) = ('";
		sqlCommand += jobsheetDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(jobsheetCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ") WHERE jobsheet_id=";
		sqlCommand += boost::lexical_cast<std::string>(jobsheetID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the jobsheet with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(jobsheetID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateLocation(int locationID, std::string countryName, std::string countryCode, std::string regionName
		, std::string cityName, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".locations SET(country_name, country_code, region_name, \
								 								 city_name) = ('";
		sqlCommand += countryName;
		sqlCommand += "', '";
		sqlCommand += countryCode;
		sqlCommand += "', '";
		sqlCommand += regionName;
		sqlCommand += "', '";
		sqlCommand += cityName;
		sqlCommand += "') WHERE location_id=";
		sqlCommand += boost::lexical_cast<std::string>(locationID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the location with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(locationID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}


	bool OrmasDal::UpdateMeasure(int measureID, std::string measureName, std::string measureShortName, int measureUnit, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".measures SET(measure_name, measure_short_name, measure_unit) = ('";
		sqlCommand += measureName;
		sqlCommand += "', '";
		sqlCommand += measureShortName;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(measureUnit);
		sqlCommand += ") WHERE measure_id=";
		sqlCommand += boost::lexical_cast<std::string>(measureID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the measure with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(measureID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateNetCost(int netCostID, std::string netCostDate, double netCostValue, int currencyID, int productID, 
		bool netCostIsOutdated, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".net_cost SET(net_cost_date, net_cost_value, currency_id, product_id, is_outdated) = ('";
		sqlCommand += netCostDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(netCostValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += netCostIsOutdated ? "TRUE" : "FALSE";
		sqlCommand += ") WHERE net_cost_id=";
		sqlCommand += boost::lexical_cast<std::string>(netCostID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the net cost with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(netCostID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateOrderList(int orderListID, int orderID, int productID, double orderListCount, double orderListSum, 
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".order_list SET(order_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(orderID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE order_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(orderListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the intem in order list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(orderListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateOrder(int orderID, int clientID, std::string orderDate, std::string orderExecutionDate, int employeeID,
		double orderCount, double orderSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".orders SET(user_id, order_date, execution_date, employee_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(clientID);
		sqlCommand += ", '";
		sqlCommand += orderDate;
		if (orderExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += orderExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE order_id=";
		sqlCommand += boost::lexical_cast<std::string>(orderID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the order with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(orderID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateOrderRawList(int orderRawListID, int orderRawID, int productID, double orderRawListCount, 
		double orderRawSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".order_raw_list SET(order_raw_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(orderRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderRawListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderRawSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE order_raw_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(orderRawListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the item in order raw list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(orderRawListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateOrderRaw(int orderRawID, int productID, std::string orderRawDate, std::string orderRawExecutionDate,
		int employeeID, double orderRawCount, double orderRawSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".order_raws SET(purveyor_id, order_raw_date, execution_date, employee_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", '";
		sqlCommand += orderRawDate;
		if (orderRawExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += orderRawExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderRawCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(orderRawSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE order_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(orderRawID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the order raw with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(orderRawID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	
	bool OrmasDal::UpdatePayment(int paymentID, std::string paymatDate, double paymentValue, int userID, int currencyID, int statusID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".payments SET(payment_date, payment_value, user_id, currency_id, status_id) = ('";
		sqlCommand += paymatDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(paymentValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ") WHERE payment_id=";
		sqlCommand += boost::lexical_cast<std::string>(paymentID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the payment with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(paymentID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdatePayslip(int payslipID, std::string payslipDate, double payslipValue, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".payslips SET(payslip_date, payslip_value, salary_id, currency_id) = ('";
		sqlCommand += payslipDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(payslipValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE payslip_id=";
		sqlCommand += boost::lexical_cast<std::string>(payslipID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the payslip with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(payslipID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdatePercentRate(int percentRateID, double percentRateValue, std::string percentRateCondition, int positionID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".percent_rate SET(value, condition, position_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(percentRateValue);
		sqlCommand += ", '";
		sqlCommand += percentRateCondition;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(positionID);
		sqlCommand += ") WHERE percent_rate_id=";
		sqlCommand += boost::lexical_cast<std::string>(percentRateID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the percent rate with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(percentRateID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdatePhoto(int photoID, int userID, int productionID, std::string photoSource, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".photos SET(user_id, product_id, photo_source) = (";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionID);
		sqlCommand += ", '";
		sqlCommand += photoSource;
		sqlCommand += "') WHERE photo_id=";
		sqlCommand += boost::lexical_cast<std::string>(photoID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the photo with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(photoID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}


	bool OrmasDal::UpdatePosition(int positionID, std::string positionName, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".positions SET(position_name) = ('";
		sqlCommand += positionName;
		sqlCommand += "') WHERE position_id=";
		sqlCommand += boost::lexical_cast<std::string>(positionID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the position with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(positionID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdatePrice(int priceID, std::string priceDate, double priceValue, int currencyID, int productID,
		bool priceIsOutdated, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".prices SET(price_date, price_value, currency_id, product_id, is_outdated) = ('";
		sqlCommand += priceDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(priceValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += priceIsOutdated ? "TRUE" : "FALSE";
		sqlCommand += ") WHERE price_id=";
		sqlCommand += boost::lexical_cast<std::string>(priceID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the price with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(priceID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateProductType(int productTypeID, std::string productTypeName, std::string productTypeShortName,
		std::string productTypeCode, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".product_type SET(product_type_name,\
								  product_type_short_name, product_type_code) = ('";
		sqlCommand += productTypeName;
		sqlCommand += "', '";
		sqlCommand += productTypeShortName;
		sqlCommand += "', '";
		sqlCommand += productTypeCode;
		sqlCommand += "') WHERE product_type_id=";
		sqlCommand += boost::lexical_cast<std::string>(productTypeID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the product type with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(productTypeID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateProduction(int productionID, std::string productionDate, std::string productionExpiryDate, std::string productionSessionStart,
		std::string productionSessionEnd, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".production SET(production_date, expiry_date,\
								 								  session_start, session_end) = ('";
		sqlCommand += productionDate;
		sqlCommand += "', '";
		sqlCommand += productionExpiryDate;
		sqlCommand += "', '";
		sqlCommand += productionSessionStart;
		sqlCommand += "', '";
		sqlCommand += productionSessionEnd;
		sqlCommand += "') WHERE production_id=";
		sqlCommand += boost::lexical_cast<std::string>(productionID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the production with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(productionID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	bool OrmasDal::UpdateProductionList(int productionListID, int productionID, int productID, double productionListCount, double productionListSum,
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".production_list SET(production_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(productionID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE production_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(productionListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the intem in production list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(productionListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateProductionConsumeRawList(int productionConsRawListID, int productionConsumeRawID, int productID, 
		double productionConsRawListCount, double productionConsRawListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".production_consume_raw_list SET(consume_raw_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(productionConsumeRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionConsRawListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionConsRawListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE consume_raw_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(productionConsRawListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the item in consume raw list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(productionConsRawListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateProductionConsumeRaw(int productionConsumeRawID, int userID, std::string productionConsumeRawDate,
		std::string productionConsumeRawExecutionDate, int stockEmployeeID, double productionConsumeRawCount,
		double productionConsumeRawSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".production_consume_raws SET(user_id, consume_raw_date, execution_date, employee_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", '";
		sqlCommand += productionConsumeRawDate;
		if (productionConsumeRawExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += productionConsumeRawExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionConsumeRawCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionConsumeRawSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE consume_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(productionConsumeRawID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the consume raw with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(productionConsumeRawID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateProductionPlanList(int productionPlanListID, int productionPlanID, int productID, 
		double productionPlanListCount, double productionPlanListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".production_plan_list SET(production_plan_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(productionPlanID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionPlanListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionPlanListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE production_plan_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(productionPlanListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the intem in production plan list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(productionPlanListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateProductionPlan(int productionPlanID, std::string productionPlanDate, int employeeID, 
		double productionPlanListCount, double productionPlanListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".production_plan SET(production_plan_date, employee_id, count, sum, status_id, currency_id) = ('";
		sqlCommand += productionPlanDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionPlanListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionPlanListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE production_plan_id=";
		sqlCommand += boost::lexical_cast<std::string>(productionPlanID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the production plan with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(productionPlanID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateProductionStock(int productionStockID, int productID, double productionStockCount, 
		double productionStockSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".production_stock SET(product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionStockCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productionStockSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE production_stock_id=";
		sqlCommand += boost::lexical_cast<std::string>(productionStockID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the item in production stock with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(productionStockID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateProduct(int productID, int companyID, std::string productName, double productVolume, int measureID, 
		double productPrice, int productTypeID, int productShelfLife, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".products SET(company_id, product_name, volume, measure_id,\
								 price, product_type_id, shelf_life, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(companyID);
		sqlCommand += ", '";
		sqlCommand += productName;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(productVolume);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(measureID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productPrice);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productTypeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productShelfLife);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE product_id=";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the product with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(productID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdatePurveyor(int userID, std::string purveyorCompanyName, int locationID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".purveyors SET(company_name, location_id) = ('";
		sqlCommand += purveyorCompanyName;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(locationID);
		sqlCommand += ") WHERE user_id=";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the purveyor with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(userID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	bool OrmasDal::UpdateReceiptProductList(int receiptProductListID, int receiptProductID, int productID, double receiptProductCount,
		double receiptProductSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".receipt_product_list SET(receipt_product_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(receiptProductID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptProductCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptProductSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE receipt_product_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(receiptProductListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the item in receipt product list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(receiptProductListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateReceiptProduct(int receiptProductID, int employeeID, std::string receiptProductDate,
		std::string receiptProductExecutionDate, int stockEmployeeID, double receiptProductCount, double receiptProductSum,
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".receipt_products SET(user_id, receipt_product_date, execution_date, employee_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", '";
		sqlCommand += receiptProductDate;
		if (receiptProductExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += receiptProductExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptProductCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptProductSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE receipt_product_id=";
		sqlCommand += boost::lexical_cast<std::string>(receiptProductID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the receipt product with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(receiptProductID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateReceiptRawList(int receiptRawListID, int receiptRawID, int productID, double receiptRawListCount,
		double receiptRawListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".receipt_raw_list SET(receipt_raw_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(receiptRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptRawListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptRawListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE receipt_raw_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(receiptRawListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the item in receipt raw list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(receiptRawListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateReceiptRaw(int receiptRawID, int employeeID, std::string receiptRawDate, std::string receiptRawExecutionDate,
		int stockEmployeeID, double receiptRawCount, double receiptRawSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".receipt_raws SET(user_id, receipt_raw_date, execution_date, employee_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", '";
		sqlCommand += receiptRawDate;
		if (receiptRawExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += receiptRawExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptRawCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(receiptRawSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE receipt_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(receiptRawID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the receipt raw with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(receiptRawID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateRefund(int refundID, std::string refuntDate, double refundValue, int userID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".refunds SET(refund_date, refund_value, user_id, currency_id) = ('";
		sqlCommand += refuntDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(refundValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE refund_id=";
		sqlCommand += boost::lexical_cast<std::string>(refundID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the refund with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(refundID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateRelationType(int relationTypeID, std::string relationTypeName, std::string comment, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".relation_type SET(relation_name, comment) = ('";
		sqlCommand += relationTypeName;
		sqlCommand += "', '";
		sqlCommand += comment;
		sqlCommand += "') WHERE relation_type_id=";
		sqlCommand += boost::lexical_cast<std::string>(relationTypeID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the relation type with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(relationTypeID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateRelation(int relationID, int user1ID, int user2ID, int relationTypeID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".relations SET(user_id_1, user_id_2, relation_type_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(user1ID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(user2ID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(relationTypeID);
		sqlCommand += ") WHERE relation_id=";
		sqlCommand += boost::lexical_cast<std::string>(relationID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the relation with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(relationID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateReturnList(int returnListID, int returnID, int productID, double returnListCount, double returnListSum, 
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".return_list SET(return_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(returnID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(returnListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(returnListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE return_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(returnListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the intem in return list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(returnListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateReturn(int returnID, int clientID, std::string returnDate, std::string returnExecutionDate, int employeeID,
		double returnCount, double returnSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".returns SET(user_id, return_date, execution_date, employee_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(clientID);
		sqlCommand += ", '";
		sqlCommand += returnDate;
		if (returnExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += returnExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(returnCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(returnSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE return_id=";
		sqlCommand += boost::lexical_cast<std::string>(returnID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the return with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(returnID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	bool OrmasDal::UpdateRole(int roleID, std::string roleCode, std::string roleName, std::string comment, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".roles SET(role_code, role_name, comment) = ('";
		sqlCommand += roleCode;
		sqlCommand += "', '";
		sqlCommand += roleName;
		sqlCommand += "', '";
		sqlCommand += comment;
		sqlCommand += "') WHERE role_id=";
		sqlCommand += boost::lexical_cast<std::string>(roleID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the role with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(roleID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateSalary(int salaryID, int userID, double salaryValue, int currencyID, int salaryTypeID, 
		std::string salaryDate, bool salaryBonus, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".salaries SET(user_id, salary_value, currency_id, salary_type_id, salary_date, salary_bonus) = (";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(salaryValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(salaryTypeID);
		sqlCommand += ", '";
		sqlCommand += salaryDate;
		sqlCommand += "',";
		sqlCommand += salaryBonus ? "TRUE" : "FALSE";
		sqlCommand += ") WHERE salary_id=";
		sqlCommand += boost::lexical_cast<std::string>(salaryID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the salary with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(salaryID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateSalaryType(int salaryTypeID, std::string salaryTypeCode, std::string salaryTypeName, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".salary_type SET(salary_type_code, salary_type_name) = ('";
		sqlCommand += salaryTypeCode;
		sqlCommand += "', '";
		sqlCommand += salaryTypeName;
		sqlCommand += "') WHERE salary_type_id=";
		sqlCommand += boost::lexical_cast<std::string>(salaryTypeID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the salary type with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(salaryTypeID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateSpecificationList(int specificationListID, int specificationID, int productID, 
		double specificationListCount, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".specification_list SET(specification_id, product_id, count) = (";
		sqlCommand += boost::lexical_cast<std::string>(specificationID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(specificationListCount);
		sqlCommand += ") WHERE specification_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(specificationListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the intem in specification list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(specificationListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateSpecification(int specificationID, int productID, double specificationSum, int currencyID,
		int employeeID, std::string sDate, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".specifications SET(product_id, sum, currency_id, employee_id, specification_date) = (";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(specificationSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", '";
		sqlCommand += sDate;
		sqlCommand += "') WHERE specification_id=";
		sqlCommand += boost::lexical_cast<std::string>(specificationID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the specification with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(specificationID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateSpoilageList(int spoilageListID, int spoilageID, int productID, double spoilageListCount, 
		double spoilageListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".spoilage_list SET(spoilage_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(spoilageID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(spoilageListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(spoilageListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE spoilage_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(spoilageListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the intem in spoilage list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(spoilageListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateSpoilage(int spoilageID, std::string spoilageDate, int employeeID, double spoilageCount, double spoilageSum, 
		int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".spoilage SET(spoilage_date, employee_id, count, sum, status_id, currency_id) = ('";
		sqlCommand += spoilageDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(spoilageCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(spoilageSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE spoilage_id=";
		sqlCommand += boost::lexical_cast<std::string>(spoilageID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the spoilage with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(spoilageID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateState(int stateID, int universalID, int statusID, std::string statusLastChange, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".state SET(universal_id, status_id, last_change) = (";
		sqlCommand += boost::lexical_cast<std::string>(universalID);;
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);;
		sqlCommand += ", '";
		sqlCommand += statusLastChange;
		sqlCommand += "') WHERE state_id=";
		sqlCommand += boost::lexical_cast<std::string>(stateID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the state with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(stateID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateStatus(int statusID, std::string statusCode, std::string statusName, std::string comment, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".status SET(status_code, status_name, comment) = ('";
		sqlCommand += statusCode;
		sqlCommand += "', '";
		sqlCommand += statusName;
		sqlCommand += "', '";
		sqlCommand += comment;
		sqlCommand += "') WHERE status_id=";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the status with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(statusID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateStatusRule(int statusRuleID, std::string statusRuleOperation, int statusID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".status_rule SET(operation, status_id) = ('";
		sqlCommand += statusRuleOperation;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);;
		sqlCommand += ") WHERE status_rule_id=";
		sqlCommand += boost::lexical_cast<std::string>(statusRuleID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the status rule with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(statusRuleID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}
	
	bool OrmasDal::UpdateStock(int stockID, int productID, double stockCount, double stockSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".stock SET(product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(stockCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(stockSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE stock_id=";
		sqlCommand += boost::lexical_cast<std::string>(stockID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the item in stock with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(stockID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	//Update subaccount
	bool OrmasDal::UpdateSubaccount(int subaccountID, int accountID, std::string subaccountNumber, double subaccountStartBalance,
		double subaccountCurrentBalance, int currencyID, int statusID,	std::string subaccountOpenDate, std::string subaccountCloseDate, 
		std::string subaccountDetails, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".subaccounts SET(account_id, subaccount_number, start_balance, current_balance, currency_id,\
								 								 status_id, date_opened, date_closed, details) = (";
		sqlCommand += boost::lexical_cast<std::string>(accountID);
		sqlCommand += ", '";
		sqlCommand += subaccountNumber;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(subaccountStartBalance);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(subaccountCurrentBalance);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", '";
		sqlCommand += subaccountOpenDate;
		if (subaccountCloseDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", '";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += subaccountCloseDate;
			sqlCommand += "', '";
		}
		sqlCommand += subaccountDetails;
		sqlCommand += "') WHERE subaccount_id=";
		sqlCommand += boost::lexical_cast<std::string>(subaccountID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the subaccount with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(subaccountID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	//Update subaccount history
	bool OrmasDal::UpdateSubaccountHistory(int subaccountHistoryID, int subaccountID, double subaccountStartBalance, double subaccountCurrentBalance,
		std::string fromDate, std::string tillDate, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".subaccount_history SET(subaccount_id, start_balance, current_balance, \
								 from_date, till_date,) = (";
		sqlCommand += boost::lexical_cast<std::string>(subaccountID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(subaccountStartBalance);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(subaccountCurrentBalance);
		sqlCommand += ", '";
		sqlCommand += fromDate;
		sqlCommand += "', '";
		sqlCommand += tillDate;
		sqlCommand += "') WHERE subaccount_history_id=";
		sqlCommand += boost::lexical_cast<std::string>(subaccountHistoryID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the subaccount history with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(subaccountHistoryID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	//Update subaccount history
	bool OrmasDal::UpdateTax(int taxID, std::string taxName, std::string taxCode, double fixedValue, int percentValue,
		std::string formulaValue, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".taxes SET(tax_name, tax_code, fixed_value, \
								 								 percent_value, formula_value,) = ('";
		sqlCommand += taxName;
		sqlCommand += "', '";
		sqlCommand += taxCode;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(fixedValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(percentValue);
		sqlCommand += ", '";
		sqlCommand += formulaValue;
		sqlCommand += "') WHERE tax_id=";
		sqlCommand += boost::lexical_cast<std::string>(taxID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the tax with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(taxID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateTimesheet(int timesheetID, int salaryID, double workedTime, std::string timesheetDate, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".timesheet SET(salary_id, worked_time, timesheet_date) = (";
		sqlCommand += boost::lexical_cast<std::string>(salaryID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(workedTime);
		sqlCommand += ", '";
		sqlCommand += boost::lexical_cast<std::string>(timesheetDate);
		sqlCommand += "') WHERE timesheet_id=";
		sqlCommand += boost::lexical_cast<std::string>(timesheetID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the timesheet with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(timesheetID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateTransportList(int transportListID, int transportID, int productID, double transportListCount,
		double transportListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".transport_list SET(transport_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(transportID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(transportListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(transportListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE transport_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(transportListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the item in transport list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(transportListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateTransport(int transportID, int employeeID, std::string transportDate, std::string transportExecutionDate,
		int stockEmployeeID, double transportCount, double transportSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".transports SET(user_id, transport_date, execution_date, employee_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", '";
		sqlCommand += transportDate;
		if (transportExecutionDate.empty())
		{
			sqlCommand += "', ";
			sqlCommand += "NULL";
			sqlCommand += ", ";
		}
		else
		{
			sqlCommand += "', '";
			sqlCommand += transportExecutionDate;
			sqlCommand += "', ";
		}
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(transportCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(transportSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE transport_id=";
		sqlCommand += boost::lexical_cast<std::string>(transportID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the transport with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(transportID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateUser(int userID, std::string userEmail, std::string userName, std::string userSurname, std::string userPhone,
		std::string userAddress, int roleID, std::string userPassword, bool userActivated, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".users SET(user_email, user_name, user_surname, user_phone, user_address, \
								 role_id, password, activated) = ('";
		sqlCommand += userEmail;
		sqlCommand += "', '";
		sqlCommand += userName;
		sqlCommand += "', '";
		sqlCommand += userSurname;
		sqlCommand += "', '";
		sqlCommand += userPhone;
		sqlCommand += "', '";
		sqlCommand += userAddress;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(roleID);
		sqlCommand += ", '";
		sqlCommand += userPassword;
		sqlCommand += "',";
		sqlCommand += userActivated ? "TRUE" : "FALSE";
		sqlCommand += ") WHERE user_id=";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the user with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(userID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateWithdrawal(int withdrawalID, std::string withdrawalDate, double withdrawalValue, int userID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".withdrawals SET(withdrawal_date, withdrawal_value, user_id, currency_id) = ('";
		sqlCommand += withdrawalDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(withdrawalValue);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE withdrawal_id=";
		sqlCommand += boost::lexical_cast<std::string>(withdrawalID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the withdrawal with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(withdrawalID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateWriteOffList(int writeOffListID, int writeOffID, int productID, double writeOffListCount, 
		double writeOffListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".write_off_list SET(write_off_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(writeOffID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE write_off_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(writeOffListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the item in write-off list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(writeOffListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateWriteOff(int writeOffID, int userID, std::string writeOffDate, int employeeID, double writeOffCount, 
		double writeOffSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".write-offs SET(user_id, write_off_date, employee_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(userID);
		sqlCommand += ", '";
		sqlCommand += writeOffDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE write_off_id=";
		sqlCommand += boost::lexical_cast<std::string>(writeOffID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the company with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(writeOffID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateWriteOffRawList(int writeOffRawListID, int writeOffRawID, int productID, double writeOffRawListCount, 
		double writeOffRawListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".write_off_raw_list SET(write_off_raw_id, product_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(writeOffRawID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(productID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffRawListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffRawListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE write_off_raw_list_id=";
		sqlCommand += boost::lexical_cast<std::string>(writeOffRawListID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the item in write_off_raw list with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(writeOffRawListID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	bool OrmasDal::UpdateWriteOffRaw(int writeOffRawID, int employeeID, std::string writeOffRawListDate, int stockEmployeeID, 
		double writeOffRawListCount, double writeOffRawListSum, int statusID, int currencyID, std::string& errorMessage)
	{
		if (PQstatus(dbConnection) == CONNECTION_BAD)
		{
			errorMessage = "DB connection was lost! Please restart application!";
			return false;
		}
		PGresult * result;
		std::string sqlCommand = "UPDATE \"OrmasSchema\".write_off_raws SET(user_id, write_off_raw_date, employee_id, count, sum, status_id, currency_id) = (";
		sqlCommand += boost::lexical_cast<std::string>(employeeID);
		sqlCommand += ", '";
		sqlCommand += writeOffRawListDate;
		sqlCommand += "', ";
		sqlCommand += boost::lexical_cast<std::string>(stockEmployeeID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffRawListCount);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(writeOffRawListSum);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(statusID);
		sqlCommand += ", ";
		sqlCommand += boost::lexical_cast<std::string>(currencyID);
		sqlCommand += ") WHERE write_off_raw_id=";
		sqlCommand += boost::lexical_cast<std::string>(writeOffRawID);
		sqlCommand += ";";
		result = PQexec(dbConnection, sqlCommand.c_str());

		if (PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			std::string logStr = PQresultErrorMessage(result);
			//WriteLog(logStr);
			PQclear(result);
			errorMessage = "SQL command is failing while updating the write off raw with this ID = ";
			errorMessage += boost::lexical_cast<std::string>(writeOffRawID);
			errorMessage += " .Please contact with application provider!";
			return false;
		}
		PQclear(result);
		return true;
	}

	// Fillter section ----------------------------------------------
	
	std::string OrmasDal::GetFilterForAccessItem(int accessItemID, std::string accessItemEng, std::string accessItemRu, 
		std::string accessItemDivision)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != accessItemID)
		{
			tempString = "";
			tempString += "access_item_id = ";
			tempString += boost::lexical_cast<std::string>(accessItemID);
			conditionVec.push_back(tempString);
		}
		if (!accessItemEng.empty())
		{
			tempString = "";
			tempString += " access_item_eng = '";
			tempString += accessItemEng;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!accessItemRu.empty())
		{
			tempString = "";
			tempString += " access_item_ru = '";
			tempString += accessItemRu;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!accessItemDivision.empty())
		{
			tempString = "";
			tempString += " access_division = '";
			tempString += accessItemDivision;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForAccess(int accessID, int roleID, int accessItemID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != accessID)
		{
			tempString = "";
			tempString += " access_id = ";
			tempString += boost::lexical_cast<std::string>(accessID);
			conditionVec.push_back(tempString);
		}
		if (0 != roleID)
		{
			tempString = "";
			tempString += " role_id = ";
			tempString += boost::lexical_cast<std::string>(roleID);
			conditionVec.push_back(tempString);
		}
		if (0 != accessItemID)
		{
			tempString = "";
			tempString += " access_item_id = ";
			tempString += boost::lexical_cast<std::string>(accessItemID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForAccountType(int accountTypeID, std::string accountTypeName, int accountTypeNumber, 
		std::string accountTypeComment)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != accountTypeID)
		{
			tempString = "";
			tempString += " account_type_id = ";
			tempString += boost::lexical_cast<std::string>(accountTypeID);
			conditionVec.push_back(tempString);
		}
		if (!accountTypeName.empty())
		{
			tempString = "";
			tempString += " account_type_name = '";
			tempString += accountTypeName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != accountTypeNumber)
		{
			tempString = "";
			tempString += " account_type_number = ";
			tempString += boost::lexical_cast<std::string>(accountTypeNumber);
			conditionVec.push_back(tempString);
		}
		if (!accountTypeComment.empty())
		{
			tempString = "";
			tempString += " comment = '";
			tempString += accountTypeComment;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForAccount(int accountID, std::string accountNumber, double accountStartBalance, 
		double accountCurrentBalance)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != accountID)
		{
			tempString = "";
			tempString += " account_id = ";
			tempString += boost::lexical_cast<std::string>(accountID);
			conditionVec.push_back(tempString);
		}
		if (!accountNumber.empty())
		{
			tempString = "";
			tempString += " account_number = '";
			tempString += accountNumber;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != accountStartBalance)
		{
			tempString = "";
			tempString += " start_balance = ";
			tempString += boost::lexical_cast<std::string>(accountStartBalance);
			conditionVec.push_back(tempString);
		}
		if (0 != accountCurrentBalance)
		{
			tempString = "";
			tempString += " current_balance = ";
			tempString += boost::lexical_cast<std::string>(accountCurrentBalance);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForAccountHistory(int accountHistoryID, int accountID, std::string accountNumber,
		double accountStartBalance,	double aCurrentBalance, std::string fromDate, std::string tillDate)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != accountHistoryID)
		{
			tempString = "";
			tempString += " account_history_id = ";
			tempString += boost::lexical_cast<std::string>(accountHistoryID);
			conditionVec.push_back(tempString);
		}
		if (0 != accountID)
		{
			tempString = "";
			tempString += " account_id = ";
			tempString += boost::lexical_cast<std::string>(accountID);
			conditionVec.push_back(tempString);
		}
		if (!accountNumber.empty())
		{
			tempString = "";
			tempString += " account_number = '";
			tempString += accountNumber;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != accountStartBalance)
		{
			tempString = "";
			tempString += " start_balance = ";
			tempString += boost::lexical_cast<std::string>(accountStartBalance);
			conditionVec.push_back(tempString);
		}
		if (0 != aCurrentBalance)
		{
			tempString = "";
			tempString += " current_balance = ";
			tempString += boost::lexical_cast<std::string>(aCurrentBalance);
			conditionVec.push_back(tempString);
		}
		if (!fromDate.empty())
		{
			tempString = "";
			tempString += " from_date = '";
			tempString += fromDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!tillDate.empty())
		{
			tempString = "";
			tempString += " till_date = '";
			tempString += tillDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForBalancePayment(int balancePaymentID, int balanceID, int paymentID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != balancePaymentID)
		{
			tempString = "";
			tempString += " balance_payment_id = ";
			tempString += boost::lexical_cast<std::string>(balancePaymentID);
			conditionVec.push_back(tempString);
		}
		if (0 != balanceID)
		{
			tempString = "";
			tempString += " balance_id = ";
			tempString += boost::lexical_cast<std::string>(balanceID);
			conditionVec.push_back(tempString);
		}
		if (0 != paymentID)
		{
			tempString = "";
			tempString += " payment_id = ";
			tempString += boost::lexical_cast<std::string>(paymentID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForBalancePayslip(int balancePayslipID, int bananceID, int payslipID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != balancePayslipID)
		{
			tempString = "";
			tempString += " balance_payslip_id = ";
			tempString += boost::lexical_cast<std::string>(balancePayslipID);
			conditionVec.push_back(tempString);
		}
		if (0 != bananceID)
		{
			tempString = "";
			tempString += " balance_id = ";
			tempString += boost::lexical_cast<std::string>(bananceID);
			conditionVec.push_back(tempString);
		}
		if (0 != payslipID)
		{
			tempString = "";
			tempString += " payslip_id = ";
			tempString += boost::lexical_cast<std::string>(payslipID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForBalanceRefund(int balanceRefundID, int balanceID, int refundID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != balanceRefundID)
		{
			tempString = "";
			tempString += " balance_refund_id = ";
			tempString += boost::lexical_cast<std::string>(balanceRefundID);
			conditionVec.push_back(tempString);
		}
		if (0 != balanceID)
		{
			tempString = "";
			tempString += " balance_id = ";
			tempString += boost::lexical_cast<std::string>(balanceID);
			conditionVec.push_back(tempString);
		}
		if (0 != refundID)
		{
			tempString = "";
			tempString += " refund_id = ";
			tempString += boost::lexical_cast<std::string>(refundID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForBalanceWithdrawal(int balanceWithdrawalID, int balanceID, int withdrawalID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != balanceWithdrawalID)
		{
			tempString = "";
			tempString += " balance_withdrawal_id = ";
			tempString += boost::lexical_cast<std::string>(balanceWithdrawalID);
			conditionVec.push_back(tempString);
		}
		if (0 != balanceID)
		{
			tempString = "";
			tempString += " balance_id = ";
			tempString += boost::lexical_cast<std::string>(balanceID);
			conditionVec.push_back(tempString);
		}
		if (0 != withdrawalID)
		{
			tempString = "";
			tempString += " withdrawal_id = ";
			tempString += boost::lexical_cast<std::string>(withdrawalID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	/// \brief	Gets filter for balance.
	///
	/// \param bID   	The identifier.
	/// \param uID   	The identifier.
	/// \param bValue	The value.
	/// \param cID   	The identifier.
	///
	/// \return	The filter for balance.

	std::string OrmasDal::GetFilterForBalance(int balanceID, int userID, int subaccountID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != balanceID)
		{
			tempString = "";
			tempString += " balance_id = ";
			tempString += boost::lexical_cast<std::string>(balanceID);
			conditionVec.push_back(tempString);
		}
		if (0 != userID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(userID);
			conditionVec.push_back(tempString);
		}
		if (0 != subaccountID)
		{
			tempString = "";
			tempString += " subaccount_id = ";
			tempString += boost::lexical_cast<std::string>(subaccountID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForClient(int clientID, std::string name, std::string surname, std::string phone,
		std::string address, std::string firm, std::string firmNumber, std::string password, std::string email,
		bool activated, int roleID, int locationID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != clientID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(clientID);
			conditionVec.push_back(tempString);
		}
		if (!name.empty())
		{
			tempString = "";
			tempString += " user_name = '";
			tempString += name;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!surname.empty())
		{
			tempString = "";
			tempString += " user_surname = '";
			tempString += surname;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!phone.empty())
		{
			tempString = "";
			tempString += " user_phone = '";
			tempString += phone;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!address.empty())
		{
			tempString = "";
			tempString += " user_address = '";
			tempString += address;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!firm.empty())
		{
			tempString = "";
			tempString += " firm = '";
			tempString += firm;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!firmNumber.empty())
		{
			tempString = "";
			tempString += " firm_number LIKE '%";
			tempString += firmNumber;
			tempString += "%'";
			conditionVec.push_back(tempString);
		}
		if (!password.empty())
		{
			tempString = "";
			tempString += " password = '";
			tempString += password;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!email.empty())
		{
			tempString = "";
			tempString += " user_email = '";
			tempString += email;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != locationID)
		{
			tempString = "";
			tempString += " location_id = ";
			tempString += boost::lexical_cast<std::string>(locationID);
			conditionVec.push_back(tempString);
		}
		if (0 != roleID)
		{
			tempString = "";
			tempString += " role_id = ";
			tempString += boost::lexical_cast<std::string>(roleID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForChartOfAccount(int chartOfAccountID, std::string chartOfAccountNumber, 
		std::string chartOfAccountName, int accountTypeID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != chartOfAccountID)
		{
			tempString = "";
			tempString += " chart_of_account_id = ";
			tempString += boost::lexical_cast<std::string>(chartOfAccountID);
			conditionVec.push_back(tempString);
		}
		if (!chartOfAccountNumber.empty())
		{
			tempString = "";
			tempString += " number_of_account = '";
			tempString += chartOfAccountNumber;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!chartOfAccountName.empty())
		{
			tempString = "";
			tempString += " name_of_account = '";
			tempString += chartOfAccountName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != accountTypeID)
		{
			tempString = "";
			tempString += " account_type_id = ";
			tempString += boost::lexical_cast<std::string>(accountTypeID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}
	/// \brief	Gets filter for company.C
	///
	/// \param cID	   	The identifier.
	/// \param cName   	The name.
	/// \param cAddress	The address.
	/// \param cPhone  	The phone.
	/// \param cComment	The comment.
	///
	/// \return	The filter for company.

	std::string OrmasDal::GetFilterForCompany(int companyID, std::string companyName, std::string companyAddress,
		std::string companyPhone, std::string comment)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != companyID)
		{
			tempString = "";
			tempString += "company_id = ";
			tempString += boost::lexical_cast<std::string>(companyID);
			conditionVec.push_back(tempString);
		}
		if (!companyName.empty())
		{
			tempString = "";
			tempString += " company_name = '";
			tempString += companyName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!companyAddress.empty())
		{
			tempString = "";
			tempString += " company_address = '";
			tempString += companyAddress;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!companyPhone.empty())
		{
			tempString = "";
			tempString += " company_phone = '";
			tempString += companyPhone;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!comment.empty())
		{
			tempString = "";
			tempString += " comment LIKE '%";
			tempString += comment;
			tempString += "%'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForCompanyAccount(int companyAccountID, int companyID, int accountID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != companyAccountID)
		{
			tempString = "";
			tempString += " company_account_id = ";
			tempString += boost::lexical_cast<std::string>(companyAccountID);
			conditionVec.push_back(tempString);
		}
		if (0 != companyID)
		{
			tempString = "";
			tempString += " company_id = ";
			tempString += boost::lexical_cast<std::string>(companyID);
			conditionVec.push_back(tempString);
		}
		if (0 != accountID)
		{
			tempString = "";
			tempString += " account_id = ";
			tempString += boost::lexical_cast<std::string>(accountID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForCompanyEmployee(int companyEmployeeID, int companyID, int employeeID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != companyEmployeeID)
		{
			tempString = "";
			tempString += " company_employee_id = ";
			tempString += boost::lexical_cast<std::string>(companyEmployeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != companyID)
		{
			tempString = "";
			tempString += " company_id = ";
			tempString += boost::lexical_cast<std::string>(companyID);
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForConsumeProductList(int consumeProductListID, int consumeProductID, int productID,
		double consumeListCount, double consumeListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != consumeProductListID)
		{
			tempString = "";
			tempString += " consume_product_list_id = ";
			tempString += boost::lexical_cast<std::string>(consumeProductListID);
			conditionVec.push_back(tempString);
		}
		if (0 != consumeProductID)
		{
			tempString = "";
			tempString += " consume_product_id = ";
			tempString += boost::lexical_cast<std::string>(consumeProductID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != consumeListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(consumeListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != consumeListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(consumeListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForConsumeProduct(int consumeProductID, int employeeID, std::string consumeProductDate, 
		std::string consumeProductExecutionDate, int stockEmployeeID, double consumeProductCount, double consumeProductSum, 
		int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != consumeProductID)
		{
			tempString = "";
			tempString += " consume_product_id = ";
			tempString += boost::lexical_cast<std::string>(consumeProductID);
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (!consumeProductDate.empty())
		{
			tempString = "";
			tempString += " consume_product_date = '";
			tempString += consumeProductDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!consumeProductExecutionDate.empty())
		{
			tempString = "";
			tempString += " execution_date = '";
			tempString += consumeProductExecutionDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != stockEmployeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(stockEmployeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != consumeProductCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(consumeProductCount);
			conditionVec.push_back(tempString);
		}
		if (0 != consumeProductSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(consumeProductSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForConsumeRawList(int consuleRawListID, int consumeRawID, int productID,
		double consumeRawListCount, double consumeRawListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != consuleRawListID)
		{
			tempString = "";
			tempString += " consume_raw_list_id = ";
			tempString += boost::lexical_cast<std::string>(consuleRawListID);
			conditionVec.push_back(tempString);
		}
		if (0 != consumeRawID)
		{
			tempString = "";
			tempString += " consume_raw_id = ";
			tempString += boost::lexical_cast<std::string>(consumeRawID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != consumeRawListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(consumeRawListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != consumeRawListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(consumeRawListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForConsumeRaw(int consumeRawID, int userID, std::string consumeRawDate, 
		std::string consumeRawExecutionDate, int stockEmployeeID, double consumeRawaCount, double conusmeRawSum, int statusID, 
		int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != consumeRawID)
		{
			tempString = "";
			tempString += " consume_raw_id = ";
			tempString += boost::lexical_cast<std::string>(consumeRawID);
			conditionVec.push_back(tempString);
		}
		if (0 != userID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(userID);
			conditionVec.push_back(tempString);
		}
		if (!consumeRawDate.empty())
		{
			tempString = "";
			tempString += " consume_raw_date = '";
			tempString += consumeRawDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!consumeRawExecutionDate.empty())
		{
			tempString = "";
			tempString += " execution_date = '";
			tempString += consumeRawExecutionDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != stockEmployeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(stockEmployeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != consumeRawaCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(consumeRawaCount);
			conditionVec.push_back(tempString);
		}
		if (0 != conusmeRawSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(conusmeRawSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForCurrency(int currenycID, int currencyCode, std::string currencyShortName, 
		std::string currencyName, int currencyUnit, bool cMainTrade)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != currenycID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currenycID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyCode)
		{
			tempString = "";
			tempString += " currency_code = ";
			tempString += boost::lexical_cast<std::string>(currencyCode);
			conditionVec.push_back(tempString);
		}
		if (!currencyShortName.empty())
		{
			tempString = "";
			tempString += " currency_short_name = '";
			tempString += currencyShortName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!currencyName.empty())
		{
			tempString = "";
			tempString += " currency_name = '";
			tempString += currencyName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != currencyUnit)
		{
			tempString = "";
			tempString += " currency_unit = ";
			tempString += boost::lexical_cast<std::string>(currencyUnit);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForDivisionAccount(int divisionAccountID, int disvisionID, 
		int accountID, std::string accountCode)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != divisionAccountID)
		{
			tempString = "";
			tempString += " division_account_id = ";
			tempString += boost::lexical_cast<std::string>(divisionAccountID);
			conditionVec.push_back(tempString);
		}
		if (0 != disvisionID)
		{
			tempString = "";
			tempString += " division_id = ";
			tempString += boost::lexical_cast<std::string>(disvisionID);
			conditionVec.push_back(tempString);
		}
		if (0 != accountID)
		{
			tempString = "";
			tempString += " account_id = ";
			tempString += boost::lexical_cast<std::string>(accountID);
			conditionVec.push_back(tempString);
		}
		if (!accountCode.empty())
		{
			tempString = "";
			tempString += " account_code = '";
			tempString += accountCode;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForDivisionEmployee(int divisionEmployeeID, int disvisionID, int employeeID, bool isContract)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != divisionEmployeeID)
		{
			tempString = "";
			tempString += " division_employee_id = ";
			tempString += boost::lexical_cast<std::string>(divisionEmployeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != disvisionID)
		{
			tempString = "";
			tempString += " division_id = ";
			tempString += boost::lexical_cast<std::string>(disvisionID);
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForDivision(int divisionID, std::string divisionName, std::string divisionCode)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != divisionID)
		{
			tempString = "";
			tempString += " division_id = ";
			tempString += boost::lexical_cast<std::string>(divisionID);
			conditionVec.push_back(tempString);
		}
		if (!divisionName.empty())
		{
			tempString = "";
			tempString += " division_name = '";
			tempString += divisionName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!divisionCode.empty())
		{
			tempString = "";
			tempString += " division_code = '";
			tempString += divisionCode;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}
	
	std::string OrmasDal::GetFilterForEmployee(int employeeID, std::string name, std::string surname, std::string phone,
		std::string address, std::string birthDate, std::string hireDate, std::string password,
		std::string email, bool activated, int roleID, int positionID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (!name.empty())
		{
			tempString = "";
			tempString += " user_name = '";
			tempString += name;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!surname.empty())
		{
			tempString = "";
			tempString += " user_surname = '";
			tempString += surname;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!phone.empty())
		{
			tempString = "";
			tempString += " user_phone = '";
			tempString += phone;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!address.empty())
		{
			tempString = "";
			tempString += " user_address = '";
			tempString += address;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!birthDate.empty())
		{
			tempString = "";
			tempString += " birth_date = '";
			tempString += birthDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!hireDate.empty())
		{
			tempString = "";
			tempString += " hire_date = '";
			tempString += hireDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!password.empty())
		{
			tempString = "";
			tempString += " password = '";
			tempString += password;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!email.empty())
		{
			tempString = "";
			tempString += " user_email = '";
			tempString += email;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != roleID)
		{
			tempString = "";
			tempString += " role_id = ";
			tempString += boost::lexical_cast<std::string>(roleID);
			conditionVec.push_back(tempString);
		}
		if (0 != positionID)
		{
			tempString = "";
			tempString += " position_id = ";
			tempString += boost::lexical_cast<std::string>(positionID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForEntry(int entryID, std::string entryDate, int debbitingAccountID, double entryValue, 
		int creditingAccountID, std::string description)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != entryID)
		{
			tempString = "";
			tempString += " entry_id = ";
			tempString += boost::lexical_cast<std::string>(entryID);
			conditionVec.push_back(tempString);
		}
		if (!entryDate.empty())
		{
			tempString = "";
			tempString += " entry_date = '";
			tempString += entryDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != debbitingAccountID)
		{
			tempString = "";
			tempString += " debiting_account_id = ";
			tempString += boost::lexical_cast<std::string>(debbitingAccountID);
			conditionVec.push_back(tempString);
		}
		if (0 != entryValue)
		{
			tempString = "";
			tempString += " value = ";
			tempString += boost::lexical_cast<std::string>(entryValue);
			conditionVec.push_back(tempString);
		}
		if (0 != creditingAccountID)
		{
			tempString = "";
			tempString += " crediting_account_id = ";
			tempString += boost::lexical_cast<std::string>(creditingAccountID);
			conditionVec.push_back(tempString);
		}
		if (!description.empty())
		{
			tempString = "";
			tempString += " description = '";
			tempString += description;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForEntryRouting(int entryRoutingID, std::string entryRoutingOperation, int debbitingAccountID, 
		int creditingAccountID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != entryRoutingID)
		{
			tempString = "";
			tempString += " entry_routing_id = ";
			tempString += boost::lexical_cast<std::string>(entryRoutingID);
			conditionVec.push_back(tempString);
		}
		if (!entryRoutingOperation.empty())
		{
			tempString = "";
			tempString += " operation_name = '";
			tempString += entryRoutingOperation;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != debbitingAccountID)
		{
			tempString = "";
			tempString += " debit = ";
			tempString += boost::lexical_cast<std::string>(debbitingAccountID);
			conditionVec.push_back(tempString);
		}
		if (0 != creditingAccountID)
		{
			tempString = "";
			tempString += " credit = ";
			tempString += boost::lexical_cast<std::string>(creditingAccountID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForEntrySubaccount(int entrySubaccountID, int entryID, int subaccountID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != entrySubaccountID)
		{
			tempString = "";
			tempString += " entry_subaccount_id = ";
			tempString += boost::lexical_cast<std::string>(entrySubaccountID);
			conditionVec.push_back(tempString);
		}
		if (0 != entryID)
		{
			tempString = "";
			tempString += " entry_id = ";
			tempString += boost::lexical_cast<std::string>(entryID);
			conditionVec.push_back(tempString);
		}
		if (0 != subaccountID)
		{
			tempString = "";
			tempString += " subaccount_id = ";
			tempString += boost::lexical_cast<std::string>(subaccountID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForFinancialReport(int financialReportID, double account_44010, double account_55010,
		double account_552, double account_55270, double account_553, double account_55321, double account_44020_90,
		double account_66010_66110,	double account_66020_66120, double account_66040_66140, double account_66050_66150, 
		double account_66060_66160,	double account_66130, double account_66070_66170, double tax, std::string fromDate, 
		std::string tillDate)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != financialReportID)
		{
			tempString = "";
			tempString += " financial_report_id = ";
			tempString += boost::lexical_cast<std::string>(financialReportID);
			conditionVec.push_back(tempString);
		}
		if (0 != account_44010)
		{
			tempString = "";
			tempString += " acc_44010 = ";
			tempString += boost::lexical_cast<std::string>(account_44010);
			conditionVec.push_back(tempString);
		}
		if (0 != account_55010)
		{
			tempString = "";
			tempString += " acc_55010 = ";
			tempString += boost::lexical_cast<std::string>(account_55010);
			conditionVec.push_back(tempString);
		}
		if (0 != account_552)
		{
			tempString = "";
			tempString += " acc_552 = ";
			tempString += boost::lexical_cast<std::string>(account_552);
			conditionVec.push_back(tempString);
		}
		if (0 != account_55270)
		{
			tempString = "";
			tempString += " acc_55270 = ";
			tempString += boost::lexical_cast<std::string>(account_55270);
			conditionVec.push_back(tempString);
		}
		if (0 != account_553)
		{
			tempString = "";
			tempString += " acc_553 = ";
			tempString += boost::lexical_cast<std::string>(account_553);
			conditionVec.push_back(tempString);
		}
		if (0 != account_55321)
		{
			tempString = "";
			tempString += " acc_55321 = ";
			tempString += boost::lexical_cast<std::string>(account_55321);
			conditionVec.push_back(tempString);
		}
		if (0 != account_44020_90)
		{
			tempString = "";
			tempString += " acc_44020_90 = ";
			tempString += boost::lexical_cast<std::string>(account_44020_90);
			conditionVec.push_back(tempString);
		}
		if (0 != account_66010_66110)
		{
			tempString = "";
			tempString += " acc_66010_66110 = ";
			tempString += boost::lexical_cast<std::string>(account_66010_66110);
			conditionVec.push_back(tempString);
		}
		if (0 != account_66020_66120)
		{
			tempString = "";
			tempString += " acc_66020_66120 = ";
			tempString += boost::lexical_cast<std::string>(account_66020_66120);
			conditionVec.push_back(tempString);
		}
		if (0 != account_66040_66140)
		{
			tempString = "";
			tempString += " acc_66040_66140 = ";
			tempString += boost::lexical_cast<std::string>(account_66040_66140);
			conditionVec.push_back(tempString);
		}
		if (0 != account_66050_66150)
		{
			tempString = "";
			tempString += " acc_66050_66150 = ";
			tempString += boost::lexical_cast<std::string>(account_66050_66150);
			conditionVec.push_back(tempString);
		}
		if (0 != account_66060_66160)
		{
			tempString = "";
			tempString += " acc_66060_66160 = ";
			tempString += boost::lexical_cast<std::string>(account_66060_66160);
			conditionVec.push_back(tempString);
		}
		if (0 != account_66130)
		{
			tempString = "";
			tempString += " acc_66130 = ";
			tempString += boost::lexical_cast<std::string>(account_66130);
			conditionVec.push_back(tempString);
		}
		if (0 != account_66070_66170)
		{
			tempString = "";
			tempString += " acc_66070_66170 = ";
			tempString += boost::lexical_cast<std::string>(account_66070_66170);
			conditionVec.push_back(tempString);
		}
		if (0 != tax)
		{
			tempString = "";
			tempString += " tax = ";
			tempString += boost::lexical_cast<std::string>(tax);
			conditionVec.push_back(tempString);
		}
		if (!fromDate.empty())
		{
			tempString = "";
			tempString += " from_date = '";
			tempString += fromDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!tillDate.empty())
		{
			tempString = "";
			tempString += " till_date = '";
			tempString += tillDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForInventorizationList(int inventorizationListID, int inventorizationID, int productID, 
		double inventorizationListCount, double inventorizationListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != inventorizationListID)
		{
			tempString = "";
			tempString += " inventorization_list_id = ";
			tempString += boost::lexical_cast<std::string>(inventorizationListID);
			conditionVec.push_back(tempString);
		}
		if (0 != inventorizationID)
		{
			tempString = "";
			tempString += " inventorization_id = ";
			tempString += boost::lexical_cast<std::string>(inventorizationID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != inventorizationListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(inventorizationListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != inventorizationListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(inventorizationListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForInventorization(int inventorizationID, int userID, std::string inventorizationDate, 
		std::string inventorizationExecutionDate, int stockEmployeeID, double inventorizationCount, double inventorizationSum,
		int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != inventorizationID)
		{
			tempString = "";
			tempString += " inventorization_id = ";
			tempString += boost::lexical_cast<std::string>(inventorizationID);
			conditionVec.push_back(tempString);
		}
		if (0 != userID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(userID);
			conditionVec.push_back(tempString);
		}
		if (!inventorizationDate.empty())
		{
			tempString = "";
			tempString += " inventorization_date = '";
			tempString += inventorizationDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!inventorizationExecutionDate.empty())
		{
			tempString = "";
			tempString += " execution_date = '";
			tempString += inventorizationExecutionDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != stockEmployeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(stockEmployeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != inventorizationCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(inventorizationCount);
			conditionVec.push_back(tempString);
		}
		if (0 != inventorizationSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(inventorizationSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForJobprice(int jobpriceID, int producrtID, double jobpriceValue, int currencyID, 
		double jobpriceVolume, int measureID, int positionID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != jobpriceID)
		{
			tempString = "";
			tempString += " jobprice_id = ";
			tempString += boost::lexical_cast<std::string>(jobpriceID);
			conditionVec.push_back(tempString);
		}
		if (0 != producrtID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(producrtID);
			conditionVec.push_back(tempString);
		}
		if (0 != jobpriceValue)
		{
			tempString = "";
			tempString += " value = ";
			tempString += boost::lexical_cast<std::string>(jobpriceValue);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (0 != jobpriceVolume)
		{
			tempString = "";
			tempString += " volume = ";
			tempString += boost::lexical_cast<std::string>(jobpriceVolume);
			conditionVec.push_back(tempString);
		}
		if (0 != measureID)
		{
			tempString = "";
			tempString += " measure_id = ";
			tempString += boost::lexical_cast<std::string>(measureID);
			conditionVec.push_back(tempString);
		}
		if (0 != positionID)
		{
			tempString = "";
			tempString += " position_id = ";
			tempString += boost::lexical_cast<std::string>(positionID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForJobsheet(int jobsheetID, std::string jobsheetDate, double jobsheetCount, int productID,
		int employeeID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != jobsheetID)
		{
			tempString = "";
			tempString += " jobsheet_id = ";
			tempString += boost::lexical_cast<std::string>(jobsheetID);
			conditionVec.push_back(tempString);
		}
		if (!jobsheetDate.empty())
		{
			tempString = "";
			tempString += " jobsheet_date = '";
			tempString += jobsheetDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != jobsheetCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(jobsheetCount);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForJobsheetForPeriod(int jobsheetID, std::string jobsheetDate, double jobsheetCount, 
		int productID, int employeeID, std::string fromDate, std::string toDate)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != jobsheetID)
		{
			tempString = "";
			tempString += " jobsheet_id = ";
			tempString += boost::lexical_cast<std::string>(jobsheetID);
			conditionVec.push_back(tempString);
		}
		if (!jobsheetDate.empty())
		{
			tempString = "";
			tempString += " jobsheet_date = '";
			tempString += jobsheetDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != jobsheetCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(jobsheetCount);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (!fromDate.empty() && !toDate.empty())
		{
			tempString = "";
			tempString += " (DATE(jobsheet_date) BETWEEN '";
			tempString += fromDate;
			tempString += "' AND '";
			if (toDate.empty())
			{
				tempString += GetSystemDate();
			}
			else
			{
				tempString += toDate;
			}
			tempString += "')";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForLocation(int locationID, std::string countryName, std::string countryCode, 
		std::string regionName, std::string cityName)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != locationID)
		{
			tempString = "";
			tempString += " location_id = ";
			tempString += boost::lexical_cast<std::string>(locationID);
			conditionVec.push_back(tempString);
		}
		if (!countryName.empty())
		{
			tempString = "";
			tempString += " country_name = '";
			tempString += countryName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!countryCode.empty())
		{
			tempString = "";
			tempString += " country_code LIKE '%";
			tempString += countryCode;
			tempString += "%'";
			conditionVec.push_back(tempString);
		}
		if (!regionName.empty())
		{
			tempString = "";
			tempString += " region_name = '";
			tempString += regionName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!cityName.empty())
		{
			tempString = "";
			tempString += " cityName = '";
			tempString += cityName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForMeasure(int measureID, std::string measureName, std::string measureShortName, int measureUnit)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != measureID)
		{
			tempString = "";
			tempString += " measure_id = ";
			tempString += boost::lexical_cast<std::string>(measureID);
			conditionVec.push_back(tempString);
		}
		if (!measureName.empty())
		{
			tempString = "";
			tempString += " measure_name = '";
			tempString += measureName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!measureShortName.empty())
		{
			tempString = "";
			tempString += " measure_short_name = '";
			tempString += measureShortName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != measureUnit)
		{
			tempString = "";
			tempString += " measure_unit = ";
			tempString += boost::lexical_cast<std::string>(measureUnit);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForNetCost(int netCostID, std::string netCostDate, double netCostValue, int currencyID, 
		int productID, bool netCostIsOutdated)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != netCostID)
		{
			tempString = "";
			tempString += " net_cost_id = ";
			tempString += boost::lexical_cast<std::string>(netCostID);
			conditionVec.push_back(tempString);
		}
		if (!netCostDate.empty())
		{
			tempString = "";
			tempString += " net_cost_date = '";
			tempString += netCostDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != netCostValue)
		{
			tempString = "";
			tempString += " net_cost_value = ";
			tempString += boost::lexical_cast<std::string>(netCostValue);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}
	
	std::string OrmasDal::GetFilterForOrderList(int orderListID, int orderID, int productID, double orderListCount, 
		double orderListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != orderListID)
		{
			tempString = "";
			tempString += " order_list_id = ";
			tempString += boost::lexical_cast<std::string>(orderListID);
			conditionVec.push_back(tempString);
		}
		if (0 != orderID)
		{
			tempString = "";
			tempString += " order_id = ";
			tempString += boost::lexical_cast<std::string>(orderID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != orderListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(orderListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != orderListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(orderListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForOrderForPeriod(int orderID, int clientID, std::string orderDate, std::string orderExecutionDate,
		int employeeID,	double orderCount, double orderSum, int statusID, int currencyID, std::string fromDate, std::string toDate)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != orderID)
		{
			tempString = "";
			tempString += " order_id = ";
			tempString += boost::lexical_cast<std::string>(orderID);
			conditionVec.push_back(tempString);
		}
		if (0 != clientID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(clientID);
			conditionVec.push_back(tempString);
		}
		if (!fromDate.empty() && !toDate.empty())
		{
			tempString = "";
			tempString += " (DATE(execution_date) BETWEEN '";
			tempString += fromDate;
			tempString += "' AND '";
			if (toDate.empty())
			{
				tempString += GetSystemDate();
			}
			else
			{
				tempString += toDate;
			}
			tempString += "')";
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != orderCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(orderCount);
			conditionVec.push_back(tempString);
		}
		if (0 != orderSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(orderSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForOrder(int orderID, int clientID, std::string orderDate, std::string orderExecutionDate,
		int employeeID, double orderCount, double orderSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != orderID)
		{
			tempString = "";
			tempString += " order_id = ";
			tempString += boost::lexical_cast<std::string>(orderID);
			conditionVec.push_back(tempString);
		}
		if (0 != clientID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(clientID);
			conditionVec.push_back(tempString);
		}
		if (!orderDate.empty())
		{
			tempString = "";
			tempString += " order_date = '";
			tempString += orderDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!orderExecutionDate.empty())
		{
			tempString = "";
			tempString += " execution_date = '";
			tempString += orderExecutionDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != orderCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(orderCount);
			conditionVec.push_back(tempString);
		}
		if (0 != orderSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(orderSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForOrderRawList(int orderRawListID, int orderRawID, int productID, double orderRawListCount, 
		double orderRawSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != orderRawListID)
		{
			tempString = "";
			tempString += " order_raw_list_id = ";
			tempString += boost::lexical_cast<std::string>(orderRawListID);
			conditionVec.push_back(tempString);
		}
		if (0 != orderRawID)
		{
			tempString = "";
			tempString += " order_raw_id = ";
			tempString += boost::lexical_cast<std::string>(orderRawID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != orderRawListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(orderRawListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != orderRawSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(orderRawSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForOrderRaw(int orderRawID, int productID, std::string orderRawDate, std::string orderRawExecutionDate,
		int employeeID, double orderRawCount, double orderRawSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != orderRawID)
		{
			tempString = "";
			tempString += " order_raw_id = ";
			tempString += boost::lexical_cast<std::string>(orderRawID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " purveyor_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (!orderRawDate.empty())
		{
			tempString = "";
			tempString += " order_raw_date = '";
			tempString += orderRawDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!orderRawExecutionDate.empty())
		{
			tempString = "";
			tempString += " execution_date = '";
			tempString += orderRawExecutionDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != orderRawCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(orderRawCount);
			conditionVec.push_back(tempString);
		}
		if (0 != orderRawSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(orderRawSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForPayment(int paymentID, std::string paymatDate, double paymentValue, int userID, int currencyID, int statusID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != paymentID)
		{
			tempString = "";
			tempString += " payment_id = ";
			tempString += boost::lexical_cast<std::string>(paymentID);
			conditionVec.push_back(tempString);
		}
		if (!paymatDate.empty())
		{
			tempString = "";
			tempString += " payment_date = '";
			tempString += paymatDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != paymentValue)
		{
			tempString = "";
			tempString += " payment_value = ";
			tempString += boost::lexical_cast<std::string>(paymentValue);
			conditionVec.push_back(tempString);
		}
		if (0 != userID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(userID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForPaymentEmployee(int paymentEmployeeID, int paymentID, int employeeID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != paymentEmployeeID)
		{
			tempString = "";
			tempString += " payment_employee_id = ";
			tempString += boost::lexical_cast<std::string>(paymentEmployeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != paymentID)
		{
			tempString = "";
			tempString += " payment_id = ";
			tempString += boost::lexical_cast<std::string>(paymentID);
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForPayslip(int payslipID, std::string payslipDate, double payslipValue, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != payslipID)
		{
			tempString = "";
			tempString += " payslip_id = ";
			tempString += boost::lexical_cast<std::string>(payslipID);
			conditionVec.push_back(tempString);
		}
		if (!payslipDate.empty())
		{
			tempString = "";
			tempString += " payslip_date = '";
			tempString += payslipDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != payslipValue)
		{
			tempString = "";
			tempString += " payslip_value = ";
			tempString += boost::lexical_cast<std::string>(payslipValue);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " salary_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForPayslipOrder(int payslipOrderID, int payslipID, int orderID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != payslipOrderID)
		{
			tempString = "";
			tempString += " payslip_order_id = ";
			tempString += boost::lexical_cast<std::string>(payslipOrderID);
			conditionVec.push_back(tempString);
		}
		if (0 != payslipID)
		{
			tempString = "";
			tempString += " payslip_id = ";
			tempString += boost::lexical_cast<std::string>(payslipID);
			conditionVec.push_back(tempString);
		}
		if (0 != orderID)
		{
			tempString = "";
			tempString += " order_id = ";
			tempString += boost::lexical_cast<std::string>(orderID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForPercentRate(int percentRateID, double percentRateValue, std::string percentRateCondition,
		int positionID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != percentRateID)
		{
			tempString = "";
			tempString += " percent_rate_id = ";
			tempString += boost::lexical_cast<std::string>(percentRateID);
			conditionVec.push_back(tempString);
		}
		if (0 != percentRateValue)
		{
			tempString = "";
			tempString += " value = ";
			tempString += boost::lexical_cast<std::string>(percentRateValue);
			conditionVec.push_back(tempString);
		}
		if (!percentRateCondition.empty())
		{
			tempString = "";
			tempString += " condition = '";
			tempString += percentRateCondition;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != positionID)
		{
			tempString = "";
			tempString += " position_id = ";
			tempString += boost::lexical_cast<std::string>(positionID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForPhoto(int photoID, int userID, int productionID, std::string photoSource)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != photoID)
		{
			tempString = "";
			tempString += " photo_id = ";
			tempString += boost::lexical_cast<std::string>(photoID);
			conditionVec.push_back(tempString);
		}
		if (0 != userID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(userID);
			conditionVec.push_back(tempString);
		}
		if (0 != productionID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productionID);
			conditionVec.push_back(tempString);
		}
		if (!photoSource.empty())
		{
			tempString = "";
			tempString += " photo_source = '";
			tempString += photoSource;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForPosition(int positionID, std::string positionName)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != positionID)
		{
			tempString = "";
			tempString += " position_id = ";
			tempString += boost::lexical_cast<std::string>(positionID);
			conditionVec.push_back(tempString);
		}
		if (!positionName.empty())
		{
			tempString = "";
			tempString += " position_name = '";
			tempString += positionName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForPrice(int priceID, std::string priceDate, double priceValue, int currencyID, int productID, 
		bool priceIsOutdated)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != priceID)
		{
			tempString = "";
			tempString += " price_id = ";
			tempString += boost::lexical_cast<std::string>(priceID);
			conditionVec.push_back(tempString);
		}
		if (!priceDate.empty())
		{
			tempString = "";
			tempString += " price_date = '";
			tempString += priceDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != priceValue)
		{
			tempString = "";
			tempString += " price_value = ";
			tempString += boost::lexical_cast<std::string>(priceValue);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForProductType(int productTypeID, std::string productTypeName, std::string productTypeShortName, 
		std::string productTypeCode)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != productTypeID)
		{
			tempString = "";
			tempString += " product_type_id = ";
			tempString += boost::lexical_cast<std::string>(productTypeID);
			conditionVec.push_back(tempString);
		}
		if (!productTypeName.empty())
		{
			tempString = "";
			tempString += " product_type_name = '";
			tempString += productTypeName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!productTypeShortName.empty())
		{
			tempString = "";
			tempString += " product_type_short_name = '";
			tempString += productTypeShortName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}	
		if (!productTypeCode.empty())
		{
			tempString = "";
			tempString += " product_type_code = '";
			tempString += productTypeCode;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForProduction(int productionID, std::string productionDate, std::string productionExpiryDate, 
		std::string productionSessionStart,	std::string productionSessionEnd)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != productionID)
		{
			tempString = "";
			tempString += " production_id = ";
			tempString += boost::lexical_cast<std::string>(productionID);
			conditionVec.push_back(tempString);
		}
		if (!productionDate.empty())
		{
			tempString = "";
			tempString += " production_date = '";
			tempString += productionDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!productionExpiryDate.empty())
		{
			tempString = "";
			tempString += " expiry_date = '";
			tempString += productionExpiryDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!productionSessionStart.empty())
		{
			tempString = "";
			tempString += " session_start = '";
			tempString += productionSessionStart;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!productionSessionEnd.empty())
		{
			tempString = "";
			tempString += " session_end = '";
			tempString += productionSessionEnd;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForProductionForPeriod(int productionID, std::string productionDate, std::string productionExpiryDate,
		std::string productionSessionStart,	std::string productionSessionEnd, std::string fromDate, std::string toDate)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != productionID)
		{
			tempString = "";
			tempString += " production_id = ";
			tempString += boost::lexical_cast<std::string>(productionID);
			conditionVec.push_back(tempString);
		}
		if (!productionDate.empty())
		{
			tempString = "";
			tempString += " production_date = '";
			tempString += productionDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!productionExpiryDate.empty())
		{
			tempString = "";
			tempString += " expiry_date = '";
			tempString += productionExpiryDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!productionSessionStart.empty())
		{
			tempString = "";
			tempString += " session_start = '";
			tempString += productionSessionStart;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!productionSessionEnd.empty())
		{
			tempString = "";
			tempString += " session_end = '";
			tempString += productionSessionEnd;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!fromDate.empty() && !toDate.empty())
		{
			tempString = "";
			tempString += " (DATE(production_date) BETWEEN '";
			tempString += fromDate;
			tempString += "' AND '";
			if (toDate.empty())
			{
				tempString += GetSystemDate();
			}
			else
			{
				tempString += toDate;
			}
			tempString += "')";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForProductionList(int productionListID, int productionID, int productID, double productionListCount, 
		double productionListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != productionListID)
		{
			tempString = "";
			tempString += " production_list_id = ";
			tempString += boost::lexical_cast<std::string>(productionListID);
			conditionVec.push_back(tempString);
		}
		if (0 != productionID)
		{
			tempString += " production_id = ";
			tempString += boost::lexical_cast<std::string>(productionID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != productionListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(productionListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != productionListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(productionListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForProductionListInEnum(int productionListID, int productionID, int productID, 
		double productionListCount, double productionListSum, int statusID, int currencyID, std::vector<int> vecProdID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != productionListID)
		{
			tempString = "";
			tempString += " production_list_id = ";
			tempString += boost::lexical_cast<std::string>(productionListID);
			conditionVec.push_back(tempString);
		}
		if (0 != productionID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productionID);
			conditionVec.push_back(tempString);
		}
		if (0 != productionListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(productionListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != productionListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(productionListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (0 != vecProdID.size())
		{
			tempString = "";
			tempString += " production_id in (";
			for (unsigned int i = 0; i < vecProdID.size(); i++)
			{
				tempString += boost::lexical_cast<std::string>(vecProdID[i]);
				if (i < vecProdID.size() - 1)
				{
					tempString += ", ";
				}
			}
			tempString += ")";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForProductionConsumeRawList(int productionConsRawListID, int productionConsumeRawID, int productID,
		double productionConsRawListCount, double productionConsRawListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != productionConsRawListID)
		{
			tempString = "";
			tempString += " consume_raw_list_id = ";
			tempString += boost::lexical_cast<std::string>(productionConsRawListID);
			conditionVec.push_back(tempString);
		}
		if (0 != productionConsumeRawID)
		{
			tempString = "";
			tempString += " consume_raw_id = ";
			tempString += boost::lexical_cast<std::string>(productionConsumeRawID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != productionConsRawListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(productionConsRawListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != productionConsRawListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(productionConsRawListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForProductionConsumeRaw(int productionConsumeRawID, int userID, std::string productionConsumeRawDate,
		std::string productionConsumeRawExecutionDate, int stockEmployeeID, double productionConsumeRawCount,
		double productionConsumeRawSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != productionConsumeRawID)
		{
			tempString = "";
			tempString += " consume_raw_id = ";
			tempString += boost::lexical_cast<std::string>(productionConsumeRawID);
			conditionVec.push_back(tempString);
		}
		if (0 != userID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(userID);
			conditionVec.push_back(tempString);
		}
		if (!productionConsumeRawDate.empty())
		{
			tempString = "";
			tempString += " consume_raw_date = '";
			tempString += productionConsumeRawDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!productionConsumeRawExecutionDate.empty())
		{
			tempString = "";
			tempString += " execution_date = '";
			tempString += productionConsumeRawExecutionDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != stockEmployeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(stockEmployeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != productionConsumeRawCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(productionConsumeRawCount);
			conditionVec.push_back(tempString);
		}
		if (0 != productionConsumeRawSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(productionConsumeRawSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForProductionPlanList(int productionPlanListID, int productionPlanID, int productID, 
		double productionPlanListCount, double productionPlanListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != productionPlanListID)
		{
			tempString = "";
			tempString += " production_plan_list_id = ";
			tempString += boost::lexical_cast<std::string>(productionPlanListID);
			conditionVec.push_back(tempString);
		}
		if (0 != productionPlanID)
		{
			tempString = "";
			tempString += " production_plan_id = ";
			tempString += boost::lexical_cast<std::string>(productionPlanID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != productionPlanListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(productionPlanListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != productionPlanListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(productionPlanListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForProductionPlan(int productionPlanID, std::string productionPlanDate, int employeeID,
		double productionPlanListCount, double productionPlanListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != productionPlanID)
		{
			tempString = "";
			tempString += " production_plan_id = ";
			tempString += boost::lexical_cast<std::string>(productionPlanID);
			conditionVec.push_back(tempString);
		}
		if (!productionPlanDate.empty())
		{
			tempString = "";
			tempString += " production_plan_date = '";
			tempString += productionPlanDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != productionPlanListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(productionPlanListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != productionPlanListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(productionPlanListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForProductionStock(int productionStockID, int productID, double productionStockCount, 
		double productionStockSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != productionStockID)
		{
			tempString = "";
			tempString += " prosuction_stock_id = ";
			tempString += boost::lexical_cast<std::string>(productionStockID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != productionStockCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(productionStockCount);
			conditionVec.push_back(tempString);
		}
		if (0 != productionStockSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(productionStockSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForProduct(int productID, int companyID, std::string productName, double productVolume,
		int measureID, double productPrice, int productTypeID, int productShelfLife, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != companyID)
		{
			tempString = "";
			tempString += " company_id = ";
			tempString += boost::lexical_cast<std::string>(companyID);
			conditionVec.push_back(tempString);
		}
		if (!productName.empty())
		{
			tempString = "";
			tempString += " product_name = '";
			tempString += productName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != productVolume)
		{
			tempString = "";
			tempString += " volume = ";
			tempString += boost::lexical_cast<std::string>(productVolume);
			conditionVec.push_back(tempString);
		}
		if (0 != measureID)
		{
			tempString = "";
			tempString += " measure_id = ";
			tempString += boost::lexical_cast<std::string>(measureID);
			conditionVec.push_back(tempString);
		}
		if (0 != productPrice)
		{
			tempString = "";
			tempString += " price = ";
			tempString += boost::lexical_cast<std::string>(productPrice);
			conditionVec.push_back(tempString);
		}
		if (0 != productTypeID)
		{
			tempString = "";
			tempString += " product_type_id = ";
			tempString += boost::lexical_cast<std::string>(productTypeID);
			conditionVec.push_back(tempString);
		}
		if (0 != productShelfLife)
		{
			tempString = "";
			tempString += " shelf_life = ";
			tempString += boost::lexical_cast<std::string>(productShelfLife);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForPurveyor(int purveyorID, std::string name, std::string surname, std::string phone,
		std::string address, std::string purveyorCompanyName, std::string password, std::string email,
		bool activated, int roleID, int locationID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != purveyorID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(purveyorID);
			conditionVec.push_back(tempString);
		}
		if (!name.empty())
		{
			tempString = "";
			tempString += " user_name = '";
			tempString += name;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!surname.empty())
		{
			tempString = "";
			tempString += " user_surname = '";
			tempString += surname;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!phone.empty())
		{
			tempString = "";
			tempString += " user_phone = '";
			tempString += phone;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!address.empty())
		{
			tempString = "";
			tempString += " user_address = '";
			tempString += address;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!purveyorCompanyName.empty())
		{
			tempString = "";
			tempString += " company_name = '";
			tempString += purveyorCompanyName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!password.empty())
		{
			tempString = "";
			tempString += " password = '";
			tempString += password;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!email.empty())
		{
			tempString = "";
			tempString += " user_email = '";
			tempString += email;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != locationID)
		{
			tempString = "";
			tempString += " location_id = ";
			tempString += boost::lexical_cast<std::string>(locationID);
			conditionVec.push_back(tempString);
		}
		if (0 != roleID)
		{
			tempString = "";
			tempString += " role_id = ";
			tempString += boost::lexical_cast<std::string>(roleID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForReceiptProductList(int receiptProductListID, int receiptProductID, int productID, 
		double receiptProductCount, double receiptProductSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != receiptProductListID)
		{
			tempString = "";
			tempString += " receipt_product_list_id = ";
			tempString += boost::lexical_cast<std::string>(receiptProductListID);
			conditionVec.push_back(tempString);
		}
		if (0 != receiptProductID)
		{
			tempString = "";
			tempString += " receipt_product_id = ";
			tempString += boost::lexical_cast<std::string>(receiptProductID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != receiptProductCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(receiptProductCount);
			conditionVec.push_back(tempString);
		}
		if (0 != receiptProductSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(receiptProductSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForReceiptProduct(int receiptProductID, int employeeID, std::string receiptProductDate,
		std::string receiptProductExecutionDate, int stockEmployeeID, double receiptProductCount, double receiptProductSum,
		int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != receiptProductID)
		{
			tempString = "";
			tempString += " receipt_product_id = ";
			tempString += boost::lexical_cast<std::string>(receiptProductID);
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (!receiptProductDate.empty())
		{
			tempString = "";
			tempString += " receipt_product_date = '";
			tempString += receiptProductDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!receiptProductExecutionDate.empty())
		{
			tempString = "";
			tempString += " execution_date = '";
			tempString += receiptProductExecutionDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != stockEmployeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(stockEmployeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != receiptProductCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(receiptProductCount);
			conditionVec.push_back(tempString);
		}
		if (0 != receiptProductSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(receiptProductSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForReceiptRawList(int receiptRawListID, int receiptRawID, int productID, 
		double receiptRawListCount, double receiptRawListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != receiptRawListID)
		{
			tempString = "";
			tempString += " consume_raw_list_id = ";
			tempString += boost::lexical_cast<std::string>(receiptRawListID);
			conditionVec.push_back(tempString);
		}
		if (0 != receiptRawID)
		{
			tempString = "";
			tempString += " consume_raw_id = ";
			tempString += boost::lexical_cast<std::string>(receiptRawID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != receiptRawListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(receiptRawListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != receiptRawListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(receiptRawListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForReceiptRaw(int receiptRawID, int employeeID, std::string receiptRawDate, 
		std::string receiptRawExecutionDate, int stockEmployeeID, double receiptRawCount, double receiptRawSum, 
		int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != receiptRawID)
		{
			tempString = "";
			tempString += " consume_raw_id = ";
			tempString += boost::lexical_cast<std::string>(receiptRawID);
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (!receiptRawDate.empty())
		{
			tempString = "";
			tempString += " consume_raw_date = '";
			tempString += receiptRawDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!receiptRawExecutionDate.empty())
		{
			tempString = "";
			tempString += " execution_date = '";
			tempString += receiptRawExecutionDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != stockEmployeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(stockEmployeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != receiptRawCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(receiptRawCount);
			conditionVec.push_back(tempString);
		}
		if (0 != receiptRawSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(receiptRawSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForRefund(int refundID, std::string refuntDate, double refundValue, int userID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != refundID)
		{
			tempString = "";
			tempString += " refund_id = ";
			tempString += boost::lexical_cast<std::string>(refundID);
			conditionVec.push_back(tempString);
		}
		if (!refuntDate.empty())
		{
			tempString = "";
			tempString += " refund_date = '";
			tempString += refuntDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != refundValue)
		{
			tempString = "";
			tempString += " refund_value = ";
			tempString += boost::lexical_cast<std::string>(refundValue);
			conditionVec.push_back(tempString);
		}
		if (0 != userID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(userID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForRelationType(int relationTypeID, std::string relationTypeName, std::string comment)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != relationTypeID)
		{
			tempString = "";
			tempString += " relation_type_id = ";
			tempString += boost::lexical_cast<std::string>(relationTypeID);
			conditionVec.push_back(tempString);
		}
		if (!relationTypeName.empty())
		{
			tempString = "";
			tempString += " relation_name = '";
			tempString += relationTypeName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!comment.empty())
		{
			tempString = "";
			tempString += " comment LIKE '%";
			tempString += comment;
			tempString += "%'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}
	
	std::string OrmasDal::GetFilterForRelation(int relationID, int user1ID, int user2ID, int relationTypeID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != relationID)
		{
			tempString = "";
			tempString += " relation_id = ";
			tempString += boost::lexical_cast<std::string>(relationID);
			conditionVec.push_back(tempString);
		}
		if (0 != user1ID)
		{
			tempString = "";
			tempString += " user_id_1 = ";
			tempString += boost::lexical_cast<std::string>(user1ID);
			conditionVec.push_back(tempString);
		}
		if (0 != user2ID)
		{
			tempString = "";
			tempString += " user_id_2 = ";
			tempString += boost::lexical_cast<std::string>(user2ID);
			conditionVec.push_back(tempString);
		}
		if (0 != relationTypeID)
		{
			tempString = "";
			tempString += " relation_type_id = ";
			tempString += boost::lexical_cast<std::string>(relationTypeID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForReturnList(int returnListID, int returnID, int productID, double returnListCount,
		double returnListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != returnListID)
		{
			tempString = "";
			tempString += " return_list_id = ";
			tempString += boost::lexical_cast<std::string>(returnListID);
			conditionVec.push_back(tempString);
		}
		if (0 != returnID)
		{
			tempString = "";
			tempString += " return_id = ";
			tempString += boost::lexical_cast<std::string>(returnID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != returnListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(returnListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != returnListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(returnListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForReturn(int returnID, int clientID, std::string returnDate, std::string returnExecutionDate,
		int employeeID, double returnCount, double returnSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != returnID)
		{
			tempString = "";
			tempString += " return_id = ";
			tempString += boost::lexical_cast<std::string>(returnID);
			conditionVec.push_back(tempString);
		}
		if (0 != clientID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(clientID);
			conditionVec.push_back(tempString);
		}
		if (!returnDate.empty())
		{
			tempString = "";
			tempString += " return_date = '";
			tempString += returnDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!returnExecutionDate.empty())
		{
			tempString = "";
			tempString += " execution_date = '";
			tempString += returnExecutionDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != returnCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(returnCount);
			conditionVec.push_back(tempString);
		}
		if (0 != returnSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(returnSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForRole(int roleID, std::string roleCode, std::string roleName, std::string comment)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != roleID)
		{
			tempString = "";
			tempString += " role_id = ";
			tempString += boost::lexical_cast<std::string>(roleID);
			conditionVec.push_back(tempString);
		}
		if (!roleCode.empty())
		{
			tempString = "";
			tempString += " role_code = '";
			tempString += roleCode;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!roleName.empty())
		{
			tempString = "";
			tempString += " role_name = '";
			tempString += roleName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!comment.empty())
		{
			tempString = "";
			tempString += " comment LIKE '%";
			tempString += comment;
			tempString += "%'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}
	
	std::string OrmasDal::GetFilterForSalary(int salaryID, int userID, double salaryValue, int currencyID, int salaryTypeID, 
		std::string salaryDate, bool salaryBonus)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != salaryID)
		{
			tempString = "";
			tempString += " salary_id = ";
			tempString += boost::lexical_cast<std::string>(salaryID);
			conditionVec.push_back(tempString);
		}
		if (0 != userID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(userID);
			conditionVec.push_back(tempString);
		}
		if (0 != salaryValue)
		{
			tempString = "";
			tempString += " salary_value = ";
			tempString += boost::lexical_cast<std::string>(salaryValue);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (0 != salaryTypeID)
		{
			tempString = "";
			tempString += " salary_type_id = ";
			tempString += boost::lexical_cast<std::string>(salaryTypeID);
			conditionVec.push_back(tempString);
		}
		if (!salaryDate.empty())
		{
			tempString = "";
			tempString += " salary_date = '";
			tempString += salaryDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForSalaryType(int salaryTypeID, std::string salaryTypeCode, std::string salaryTypeName)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != salaryTypeID)
		{
			tempString = "";
			tempString += "salary_type_id = ";
			tempString += boost::lexical_cast<std::string>(salaryTypeID);
			conditionVec.push_back(tempString);
		}
		if (!salaryTypeCode.empty())
		{
			tempString = "";
			tempString += " salary_type_code = '";
			tempString += salaryTypeCode;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!salaryTypeName.empty())
		{
			tempString = "";
			tempString += " salary_type_name = '";
			tempString += salaryTypeName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForSpecificationList(int specificationListID, int specificationID, int productID, 
		double specificationListCount)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != specificationListID)
		{
			tempString = "";
			tempString += " specification_list_id = ";
			tempString += boost::lexical_cast<std::string>(specificationListID);
			conditionVec.push_back(tempString);
		}
		if (0 != specificationID)
		{
			tempString = "";
			tempString += " specification_id = ";
			tempString += boost::lexical_cast<std::string>(specificationID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != specificationListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(specificationListCount);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForSpecification(int specificationID, int productID, double specificationSum, int currencyID, 
		int employeeID, std::string sDate)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != specificationID)
		{
			tempString = "";
			tempString += " specification_id = ";
			tempString += boost::lexical_cast<std::string>(specificationID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != specificationSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(specificationSum);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (!sDate.empty())
		{
			tempString = "";
			tempString += " specification_date = '";
			tempString += sDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForSpoilageList(int spoilageListID, int spoilageID, int productID, double spoilageListCount, 
		double spoilageListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != spoilageListID)
		{
			tempString = "";
			tempString += " spoilage_list_id = ";
			tempString += boost::lexical_cast<std::string>(spoilageListID);
			conditionVec.push_back(tempString);
		}
		if (0 != spoilageID)
		{
			tempString = "";
			tempString += " spoilage_id = ";
			tempString += boost::lexical_cast<std::string>(spoilageID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != spoilageListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(spoilageListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != spoilageListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(spoilageListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForSpoilage(int spoilageID, std::string spoilageDate, int employeeID, double spoilageCount,
		double spoilageSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != spoilageID)
		{
			tempString = "";
			tempString += " spoilage_id = ";
			tempString += boost::lexical_cast<std::string>(spoilageID);
			conditionVec.push_back(tempString);
		}
		if (!spoilageDate.empty())
		{
			tempString = "";
			tempString += " spoilage_date = '";
			tempString += spoilageDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != spoilageCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(spoilageCount);
			conditionVec.push_back(tempString);
		}
		if (0 != spoilageSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(spoilageSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForState(int stateID, int universalID, int statusID, std::string statusLastChange)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != stateID)
		{
			tempString = "";
			tempString += " state_id = ";
			tempString += boost::lexical_cast<std::string>(stateID);
			conditionVec.push_back(tempString);
		}
		if (0 != universalID)
		{
			tempString = "";
			tempString += " universal_id = ";
			tempString += boost::lexical_cast<std::string>(universalID);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (!statusLastChange.empty())
		{
			tempString = "";
			tempString += " last_change = '";
			tempString += statusLastChange;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForStatus(int statusID, std::string statusCode, std::string statusName, std::string comment)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (!statusCode.empty())
		{
			tempString = "";
			tempString += " status_code = '";
			tempString += statusCode;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!statusName.empty())
		{
			tempString = "";
			tempString += " status_name = '";
			tempString += statusName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!comment.empty())
		{
			tempString = "";
			tempString += " comment LIKE '%";
			tempString += comment;
			tempString += "%'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForStatusRule(int statusRuleID, std::string statusRuleOperation, int statusID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != statusRuleID)
		{
			tempString = "";
			tempString += " status_rule_id = ";
			tempString += boost::lexical_cast<std::string>(statusRuleID);
			conditionVec.push_back(tempString);
		}
		if (!statusRuleOperation.empty())
		{
			tempString = "";
			tempString += " operation = '";
			tempString += statusRuleOperation;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}
	
	std::string OrmasDal::GetFilterForStock(int stockID, int productID, double stockCount, double stockSum, int statusID, 
		int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != stockID)
		{
			tempString = "";
			tempString += " stock_id = ";
			tempString += boost::lexical_cast<std::string>(stockID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != stockCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(stockCount);
			conditionVec.push_back(tempString);
		}
		if (0 != stockSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(stockSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForSubaccount(int subaccountID, int accountID, std::string subaccountNumber, 
		double subaccountStartBalance, double subaccountCurrentBalance, int currencyID, int statusID,
		std::string subaccountOpenDate, std::string subaccountCloseDate, std::string subaccountDetails)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != subaccountID)
		{
			tempString = "";
			tempString += " subaccount_id = ";
			tempString += boost::lexical_cast<std::string>(subaccountID);
			conditionVec.push_back(tempString);
		}
		if (0 != accountID)
		{
			tempString = "";
			tempString += " account_id = ";
			tempString += boost::lexical_cast<std::string>(accountID);
			conditionVec.push_back(tempString);
		}
		if (!subaccountNumber.empty())
		{
			tempString = "";
			tempString += " subaccount_number = '";
			tempString += subaccountNumber;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != subaccountStartBalance)
		{
			tempString = "";
			tempString += " start_balance = ";
			tempString += boost::lexical_cast<std::string>(subaccountStartBalance);
			conditionVec.push_back(tempString);
		}
		if (0 != subaccountCurrentBalance)
		{
			tempString = "";
			tempString += " current_balance = ";
			tempString += boost::lexical_cast<std::string>(subaccountCurrentBalance);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (!subaccountOpenDate.empty())
		{
			tempString = "";
			tempString += " date_opened = '";
			tempString += subaccountOpenDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!subaccountCloseDate.empty())
		{
			tempString = "";
			tempString += " date_closed = '";
			tempString += subaccountCloseDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!subaccountDetails.empty())
		{
			tempString = "";
			tempString += " details = '";
			tempString += subaccountDetails;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForSubaccountHistory(int subaccountHistoryID, int subaccountID, double subaccountStartBalance,
		double subaccountCurrentBalance, std::string fromDate, std::string tillDate)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != subaccountHistoryID)
		{
			tempString = "";
			tempString += " subaccount_history_id = ";
			tempString += boost::lexical_cast<std::string>(subaccountHistoryID);
			conditionVec.push_back(tempString);
		}
		if (0 != subaccountID)
		{
			tempString = "";
			tempString += " subaccount_id = ";
			tempString += boost::lexical_cast<std::string>(subaccountID);
			conditionVec.push_back(tempString);
		}
		if (0 != subaccountStartBalance)
		{
			tempString = "";
			tempString += " start_balance = ";
			tempString += boost::lexical_cast<std::string>(subaccountStartBalance);
			conditionVec.push_back(tempString);
		}
		if (0 != subaccountCurrentBalance)
		{
			tempString = "";
			tempString += " current_balance = ";
			tempString += boost::lexical_cast<std::string>(subaccountCurrentBalance);
			conditionVec.push_back(tempString);
		}
		if (!fromDate.empty())
		{
			tempString = "";
			tempString += " from_date = '";
			tempString += fromDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!tillDate.empty())
		{
			tempString = "";
			tempString += " till_date = '";
			tempString += tillDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForTax(int taxID, std::string taxName, std::string taxCode, double fixedValue, int percentValue,
		std::string formulaValue)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != taxID)
		{
			tempString = "";
			tempString += " tax_id = ";
			tempString += boost::lexical_cast<std::string>(taxID);
			conditionVec.push_back(tempString);
		}
		if (!taxName.empty())
		{
			tempString = "";
			tempString += " tax_name = '";
			tempString += taxName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!taxCode.empty())
		{
			tempString = "";
			tempString += " tax_code = '";
			tempString += taxCode;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != fixedValue)
		{
			tempString = "";
			tempString += " fixed_value = ";
			tempString += boost::lexical_cast<std::string>(fixedValue);
			conditionVec.push_back(tempString);
		}
		if (0 != percentValue)
		{
			tempString = "";
			tempString += " percent_value = ";
			tempString += boost::lexical_cast<std::string>(percentValue);
			conditionVec.push_back(tempString);
		}
		if (!formulaValue.empty())
		{
			tempString = "";
			tempString += " formula_value = '";
			tempString += formulaValue;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForTimesheet(int timesheetID, int salaryID, double workedTime, std::string timesheetDate)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != timesheetID)
		{
			tempString = "";
			tempString += " timesheet_id = ";
			tempString += boost::lexical_cast<std::string>(timesheetID);
			conditionVec.push_back(tempString);
		}
		if (0 != salaryID)
		{
			tempString = "";
			tempString += " salary_id = ";
			tempString += boost::lexical_cast<std::string>(salaryID);
			conditionVec.push_back(tempString);
		}
		if (0 != workedTime)
		{
			tempString = "";
			tempString += " worked_time = ";
			tempString += boost::lexical_cast<std::string>(workedTime);
			conditionVec.push_back(tempString);
		}
		if (!timesheetDate.empty())
		{
			tempString = "";
			tempString += " timesheet_date = '";
			tempString += timesheetDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForTimesheetForPeriod(int timesheetID, int salaryID, double workedTime, std::string timesheetDate,
		std::string fromDate, std::string toDate)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != timesheetID)
		{
			tempString = "";
			tempString += " timesheet_id = ";
			tempString += boost::lexical_cast<std::string>(timesheetID);
			conditionVec.push_back(tempString);
		}
		if (0 != salaryID)
		{
			tempString = "";
			tempString += " salary_id = ";
			tempString += boost::lexical_cast<std::string>(salaryID);
			conditionVec.push_back(tempString);
		}
		if (0 != workedTime)
		{
			tempString = "";
			tempString += " worked_time = ";
			tempString += boost::lexical_cast<std::string>(workedTime);
			conditionVec.push_back(tempString);
		}
		if (!timesheetDate.empty())
		{
			tempString = "";
			tempString += " timesheet_date = '";
			tempString += timesheetDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!fromDate.empty() && !toDate.empty())
		{
			tempString = "";
			tempString += " (DATE(timesheet_date) BETWEEN '";
			tempString += fromDate;
			tempString += "' AND '";
			if (toDate.empty())
			{
				tempString += GetSystemDate();
			}
			else
			{
				tempString += toDate;
			}
			tempString += "')";
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForTransportList(int transportListID, int transportID, int productID, double transportListCount, 
		double transportListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != transportListID)
		{
			tempString = "";
			tempString += " transport_list_id = ";
			tempString += boost::lexical_cast<std::string>(transportListID);
			conditionVec.push_back(tempString);
		}
		if (0 != transportID)
		{
			tempString = "";
			tempString += " transport_id = ";
			tempString += boost::lexical_cast<std::string>(transportID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != transportListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(transportListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != transportListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(transportListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForTransport(int transportID, int employeeID, std::string transportDate, std::string transportExecutionDate,	
		int stockEmployeeID, double transportCount, double transportSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != transportID)
		{
			tempString = "";
			tempString += " transport_id = ";
			tempString += boost::lexical_cast<std::string>(transportID);
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (!transportDate.empty())
		{
			tempString = "";
			tempString += " transport_date = '";
			tempString += transportDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!transportExecutionDate.empty())
		{
			tempString = "";
			tempString += " execution_date = '";
			tempString += transportExecutionDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != stockEmployeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(stockEmployeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != transportCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(transportCount);
			conditionVec.push_back(tempString);
		}
		if (0 != transportSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(transportSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForUser(int userID, std::string userEmail, std::string userName, std::string userSurname, 
		std::string userPhone, std::string userAddress, int roleID, std::string userPassword, bool userActivated)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != userID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(userID);
			conditionVec.push_back(tempString);
		}
		if (!userEmail.empty())
		{
			tempString = "";
			tempString += " user_email = '";
			tempString += userEmail;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!userName.empty())
		{
			tempString = "";
			tempString += " user_name = '";
			tempString += userName;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!userSurname.empty())
		{
			tempString = "";
			tempString += " user_surname = '";
			tempString += userSurname;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!userPhone.empty())
		{
			tempString = "";
			tempString += " user_phone = '";
			tempString += userPhone;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (!userAddress.empty())
		{
			tempString = "";
			tempString += " user_address = '";
			tempString += userAddress;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != roleID)
		{
			tempString = "";
			tempString += " role_id = ";
			tempString += boost::lexical_cast<std::string>(roleID);
			conditionVec.push_back(tempString);
		}
		if (!userPassword.empty())
		{
			tempString = "";
			tempString += " password = '";
			tempString += userPassword;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForWithdrawal(int withdrawalID, std::string withdrawalDate, double withdrawalValue, int userID, 
		int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != withdrawalID)
		{
			tempString = "";
			tempString += " withdrawal_id = ";
			tempString += boost::lexical_cast<std::string>(withdrawalID);
			conditionVec.push_back(tempString);
		}
		if (!withdrawalDate.empty())
		{
			tempString = "";
			tempString += " withdrawal_date = '";
			tempString += withdrawalDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != withdrawalValue)
		{
			tempString = "";
			tempString += " withdrawal_value = ";
			tempString += boost::lexical_cast<std::string>(withdrawalValue);
			conditionVec.push_back(tempString);
		}
		if (0 != userID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(userID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForWriteOffList(int writeOffListID, int writeOffID, int productID, double writeOffListCount, 
		double writeOffListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != writeOffListID)
		{
			tempString = "";
			tempString += " write-off_list_id = ";
			tempString += boost::lexical_cast<std::string>(writeOffListID);
			conditionVec.push_back(tempString);
		}
		if (0 != writeOffID)
		{
			tempString = "";
			tempString += " write_off_id = ";
			tempString += boost::lexical_cast<std::string>(writeOffID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != writeOffListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(writeOffListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != writeOffListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(writeOffListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForWriteOff(int writeOffID, int userID, std::string writeOffDate, int employeeID,
		double writeOffCount, double writeOffSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != writeOffID)
		{
			tempString = "";
			tempString += " write_off_id = ";
			tempString += boost::lexical_cast<std::string>(writeOffID);
			conditionVec.push_back(tempString);
		}
		if (0 != userID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(userID);
			conditionVec.push_back(tempString);
		}
		if (!writeOffDate.empty())
		{
			tempString = "";
			tempString += " write_off_date = '";
			tempString += writeOffDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != writeOffCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(writeOffCount);
			conditionVec.push_back(tempString);
		}
		if (0 != writeOffSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(writeOffSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForWriteOffRawList(int writeOffRawListID, int writeOffRawID, int productID, 
		double writeOffRawListCount, double writeOffRawListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != writeOffRawListID)
		{
			tempString = "";
			tempString += " write_off_raw_list_id = ";
			tempString += boost::lexical_cast<std::string>(writeOffRawListID);
			conditionVec.push_back(tempString);
		}
		if (0 != writeOffRawID)
		{
			tempString = "";
			tempString += " write_off_raw_id = ";
			tempString += boost::lexical_cast<std::string>(writeOffRawID);
			conditionVec.push_back(tempString);
		}
		if (0 != productID)
		{
			tempString = "";
			tempString += " product_id = ";
			tempString += boost::lexical_cast<std::string>(productID);
			conditionVec.push_back(tempString);
		}
		if (0 != writeOffRawListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(writeOffRawListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != writeOffRawListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(writeOffRawListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}

	std::string OrmasDal::GetFilterForWriteOffRaw(int writeOffRawID, int employeeID, std::string writeOffRawListDate, 
		int stockEmployeeID, double writeOffRawListCount, double writeOffRawListSum, int statusID, int currencyID)
	{
		std::string tempString = "";
		std::string filter = " where ";
		std::vector<std::string> conditionVec;
		if (0 != writeOffRawID)
		{
			tempString = "";
			tempString += " write_off_raw_id = ";
			tempString += boost::lexical_cast<std::string>(writeOffRawID);
			conditionVec.push_back(tempString);
		}
		if (0 != employeeID)
		{
			tempString = "";
			tempString += " user_id = ";
			tempString += boost::lexical_cast<std::string>(employeeID);
			conditionVec.push_back(tempString);
		}
		if (!writeOffRawListDate.empty())
		{
			tempString = "";
			tempString += " write_off_raw_date = '";
			tempString += writeOffRawListDate;
			tempString += "'";
			conditionVec.push_back(tempString);
		}
		if (0 != stockEmployeeID)
		{
			tempString = "";
			tempString += " employee_id = ";
			tempString += boost::lexical_cast<std::string>(stockEmployeeID);
			conditionVec.push_back(tempString);
		}
		if (0 != writeOffRawListCount)
		{
			tempString = "";
			tempString += " count = ";
			tempString += boost::lexical_cast<std::string>(writeOffRawListCount);
			conditionVec.push_back(tempString);
		}
		if (0 != writeOffRawListSum)
		{
			tempString = "";
			tempString += " sum = ";
			tempString += boost::lexical_cast<std::string>(writeOffRawListSum);
			conditionVec.push_back(tempString);
		}
		if (0 != statusID)
		{
			tempString = "";
			tempString += " status_id = ";
			tempString += boost::lexical_cast<std::string>(statusID);
			conditionVec.push_back(tempString);
		}
		if (0 != currencyID)
		{
			tempString = "";
			tempString += " currency_id = ";
			tempString += boost::lexical_cast<std::string>(currencyID);
			conditionVec.push_back(tempString);
		}
		if (conditionVec.size() >= 1)
		{
			filter += conditionVec.at(0);
			for (unsigned int i = 1; i < conditionVec.size(); i++)
			{
				filter += " AND ";
				filter += conditionVec.at(i);
			}
		}
		else
		{
			return "";
		}
		return filter;
	}
}