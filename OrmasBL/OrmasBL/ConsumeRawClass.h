#ifndef CONSUMERAWCLASS_H
#define CONSUMERAWCLASS_H

#include "OrmasDAL.h"
#include <map>

namespace BusinessLayer
{
	class ConsumeRaw
	{
	protected:
		int id = 0;
		int employeeID = 0;
		std::string date = "";
		std::string executionDate = "";
		int stockEmployeeID = 0;
		int count = 0;
		double sum = 0;
		int statusID = 0;
		int currencyID = 0;
	public:
		ConsumeRaw(int crID, int eID, std::string crDate, std::string crExecDate, int seID, int crCount, double crSum, int sID, int cID) :
			id(crID), employeeID(eID), date(crDate), executionDate(crExecDate), stockEmployeeID(seID), count(crCount), sum(crSum),
			statusID(sID), currencyID(cID){};
		ConsumeRaw(DataLayer::consumeRawsCollection);
		ConsumeRaw(){};
		~ConsumeRaw(){};

		//Consume Product class Accessors
		int GetID();
		int GetEmployeeID();
		std::string GetDate();
		std::string GetExecutionDate();
		int GetStockEmployeeID();
		int GetCount();
		double GetSum();
		int GetStatusID();
		int GetCurrencyID();

		//Consume Product class Mutators
		void SetID(int);
		void SetEmployeeID(int);
		void SetDate(std::string);
		void SetExecutionDate(std::string);
		void SetStockEmployeeID(int);
		void SetCount(int);
		void SetSum(double);
		void SetStatusID(int);
		void SetCurrencyID(int);

		//Create, delete, update methods
		bool CreateConsumeRaw(DataLayer::OrmasDal& ormasDal, std::string& errorMessage);
		bool UpdateConsumeRaw(DataLayer::OrmasDal& ormasDal, std::string& errorMessage);
		bool DeleteConsumeRaw(DataLayer::OrmasDal& ormasDal, std::string& errorMessage);
		bool CreateConsumeRaw(DataLayer::OrmasDal& ormasDal, int eID, std::string crDate, std::string crExecDate, int seID,
			int crCount, double crSum, int sID, int cID, std::string& errorMessage);
		bool UpdateConsumeRaw(DataLayer::OrmasDal& ormasDal, int eID, std::string crDate, std::string crExecDate, int seID, 
			int crCount, double crSum, int sID, int cID, std::string& errorMessage);

		//Generate filter string for class
		std::string GenerateFilter(DataLayer::OrmasDal& ormasDal);
		bool GetConsumeRawByID(DataLayer::OrmasDal& ormasDal, int crID, std::string& errorMessage);
		bool IsEmpty();
		void Clear();
	private:
		double prevSum = 0;
		int prevCount = 0;
		bool IsDuplicate(DataLayer::OrmasDal& ormasDal, int eID, std::string crDate, int seID, int crCount, double crSum,
			int cID, std::string& errorMessage);
		bool IsDuplicate(DataLayer::OrmasDal& ormasDal, std::string& errorMessage);
		bool ChangesAtStock(DataLayer::OrmasDal& ormasDal, int crID, std::string& errorMessage);
		bool ChangesAtStock(DataLayer::OrmasDal& ormasDal, int crID, double pSum, int pCount, std::string& errorMessage);
		double GetCurrentSum(DataLayer::OrmasDal& ormasDal, int crID, std::string& errorMessage);
		int GetCurrentCount(DataLayer::OrmasDal& ormasDal, int crID, std::string& errorMessage);
	};
}
#endif //CONSUMERAWCLASS_H