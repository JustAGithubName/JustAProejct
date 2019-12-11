#ifndef ORMASDAL_H
#define ORMASDAL_H
#include <string>
//#include <pqxx/pqxx>
#include <libpq-fe.h>
#include "ConnString.h"
#include "TupleCollection.h"

//using namespace pqxx;
namespace DataLayer{
	class OrmasDal{
	public:
		//class constructors
		OrmasDal(ConnectionString connection, PGconn *dbConection);
		OrmasDal(const OrmasDal &ormasDAL);
		OrmasDal();
		~OrmasDal();
		
		//connection methods
		PGconn* GetConnection();
		bool IsConnectionStillALive();
		bool ReConnectToDB();
		bool ConnectToDB(std::string dbname, std::string username, std::string password, std::string host, int port);
		void InitFromConfigFile(std::string path);
		void SetDBParams(std::string dbname, std::string username, std::string password, std::string host, int port);
		int GenerateID();
		int GenerateAccountID();
		std::string GetSystemDate();
		std::string GetSystemDateTime();

		//utility
		bool CloseOfMonthDateCorrection(std::string& closeDate);

		//work with transaction
		bool StartTransaction(std::string& errorMessage);
		bool CommitTransaction(std::string& errorMessage);
		bool CancelTransaction(std::string& errorMessage);

		//view methods
		std::vector<accessItemsCollection> GetAccessItems(std::string& errorMessage, std::string filter = std::string());
		std::vector<accessesViewCollection> GetAccesses(std::string& errorMessage, std::string filter = std::string());
		std::vector<accountTypeCollection> GetAccountType(std::string& errorMessage, std::string filter = std::string());
		std::vector<accountHistoryCollection> GetAccountHistory(std::string& errorMessage, std::string filter = std::string());
		std::vector<accountsCollection> GetAccounts(std::string& errorMessage, std::string filter = std::string());
		std::vector<accountableViewCollection> GetAccountable(std::string& errorMessage, std::string filter = std::string());
		std::vector<accountableTransactionCollection> GetAccountableTransaction(std::string& errorMessage, std::string filter = std::string());
		std::vector<amortizeGroupCollection> GetAmortizeGroup(std::string& errorMessage, std::string filter = std::string());
		std::vector<amortizeTypeCollection> GetAmortizeType(std::string& errorMessage, std::string filter = std::string());
		std::vector<balancePaymentCollection> GetBalancePayment(std::string& errorMessage, std::string filter = std::string());
		std::vector<balancePayslipCollection> GetBalancePayslip(std::string& errorMessage, std::string filter = std::string());
		std::vector<balanceRefundCollection> GetBalanceRefund(std::string& errorMessage, std::string filter = std::string());
		std::vector<balanceWithdrawalCollection> GetBalanceWithdrawal(std::string& errorMessage, std::string filter = std::string());
		std::vector<balancesViewCollection> GetBalances(std::string& errorMessage, std::string filter = std::string());
		std::vector<branchesCollection> GetBranches(std::string& errorMessage, std::string filter = std::string());
		std::vector<borrowersViewCollection> GetBorrowers(std::string& errorMessage, std::string filter = std::string());
		std::vector<cashboxViewCollection> GetCashbox(std::string& errorMessage, std::string filter = std::string());
		std::vector<cashboxEmployeeViewCollection> GetCashboxEmployee(std::string& errorMessage, std::string filter = std::string());
		std::vector<cashboxTransactionCollection> GetCashboxTransaction(std::string& errorMessage, std::string filter = std::string());
		std::vector<chartOfAccountsViewCollection> GetChartOfAccounts(std::string& errorMessage, std::string filter = std::string());
		std::vector<clientsViewCollection> GetClients(std::string& errorMessage, std::string filter = std::string());
		std::vector<companiesCollection> GetCompanies(std::string& errorMessage, std::string filter = std::string());
		std::vector<companyEmployeeViewCollection> GetCompanyEmployee(std::string& errorMessage, std::string filter = std::string());
		std::vector<companyAccountViewCollection> GetCompanyAccount(std::string& errorMessage, std::string filter = std::string());
		std::vector<consumeProductListViewCollection> GetConsumeProductList(std::string& errorMessage, std::string filter = std::string());
		std::vector<consumeProductsViewCollection> GetConsumeProducts(std::string& errorMessage, std::string filter = std::string());
		std::vector<consumeRawListViewCollection> GetConsumeRawList(std::string& errorMessage, std::string filter = std::string());
		std::vector<consumeRawsViewCollection> GetConsumeRaws(std::string& errorMessage, std::string filter = std::string());
		std::vector<creditorsCollection> GetCreditors(std::string& errorMessage, std::string filter = std::string());
		std::vector<currenciesCollection> GetCurrencies(std::string& errorMessage, std::string filter = std::string());
		std::vector<divisionAccountViewCollection> GetDivisionAccount(std::string& errorMessage, std::string filter = std::string());
		std::vector<divisionEmployeeCollection> GetDivisionEmployee(std::string& errorMessage, std::string filter = std::string());
		std::vector<divisionsCollection> GetDivisions(std::string& errorMessage, std::string filter = std::string());
		std::vector<employeesViewCollection> GetEmployees(std::string& errorMessage, std::string filter = std::string());
		std::vector<employeeProductViewCollection> GetEmployeeProduct(std::string& errorMessage, std::string filter = std::string());
		std::vector<entriesViewCollection> GetEntries(std::string& errorMessage, std::string filter = std::string());
		std::vector<entriesFullViewCollection> GetExtendedEntries(std::string& errorMessage, std::string filter = std::string());
		std::vector<entrySubaccountCollection> GetEntrySubaccount(std::string& errorMessage, std::string filter = std::string());
		std::vector<entryOperationCollection> GetEntryOperation(std::string& errorMessage, std::string filter = std::string());
		std::vector<entryRoutingCollection> GetEntryRouting(std::string& errorMessage, std::string filter = std::string());
		std::vector<financialReportCollection> GetFinancialReport(std::string& errorMessage, std::string filter = std::string());
		std::vector<fixedAssetsViewCollection> GetFixedAssets(std::string& errorMessage, std::string filter = std::string());
		std::vector<fixedAssetsDetailsViewCollection> GetFixedAssetsDetails(std::string& errorMessage, std::string filter = std::string());
		std::vector<fixedAssetsSpecificationCollection> GetFixedAssetsSpecification(std::string& errorMessage, std::string filter = std::string());
		std::vector<fixedAssetsOperationsCollection> GetFixedAssetsOperation(std::string& errorMessage, std::string filter = std::string());
		std::vector<inventorizationListViewCollection> GetInventorizationList(std::string& errorMessage, std::string filter = std::string());
		std::vector<inventorizationsViewCollection> GetInventorizations(std::string& errorMessage, std::string filter = std::string());
		std::vector<inventoryViewCollection> GetInventory(std::string& errorMessage, std::string filter = std::string());
		std::vector<inventoryHistoryCollection> GetInventoryHistory(std::string& errorMessage, std::string filter = std::string());
		std::vector<jobpriceViewCollection> GetJobprice(std::string& errorMessage, std::string filter = std::string());
		std::vector<jobsheetViewCollection> GetJobsheet(std::string& errorMessage, std::string filter = std::string());
		std::vector<locationsCollection> GetLocations(std::string& errorMessage, std::string filter = std::string());
		std::vector<measuresCollection> GetMeasures(std::string& errorMessage, std::string filter = std::string());
		std::vector<netCostViewCollection> GetNetCost(std::string& errorMessage, std::string filter = std::string());
		std::vector<orderListViewCollection> GetOrderList(std::string& errorMessage, std::string filter = std::string());
		std::vector<ordersViewCollection> GetOrders(std::string& errorMessage, std::string filter = std::string());
		std::vector<orderRawListViewCollection> GetOrderRawList(std::string& errorMessage, std::string filter = std::string());
		std::vector<orderRawsViewCollection> GetOrderRaws(std::string& errorMessage, std::string filter = std::string());
		std::vector<paymentsViewCollection> GetPayments(std::string& errorMessage, std::string filter = std::string());
		std::vector<paymentEmployeeCollection> GetPaymentEmployee(std::string& errorMessage, std::string filter = std::string());
		std::vector<payslipsViewCollection> GetPayslips(std::string& errorMessage, std::string filter = std::string());
		std::vector<payslipOrderCollection> GetPayslipOrder(std::string& errorMessage, std::string filter = std::string());
		std::vector<percentRateCollection> GetPercentRate(std::string& errorMessage, std::string filter = std::string());
		std::vector<photosCollection> GetPhotos(std::string& errorMessage, std::string filter = std::string());
		std::vector<positionsCollection> GetPositions(std::string& errorMessage, std::string filter = std::string());
		std::vector<productBranchViewCollection> GetProductBranch(std::string& errorMessage, std::string filter = std::string());
		std::vector<pricesViewCollection> GetPrices(std::string& errorMessage, std::string filter = std::string());
		std::vector<productTypeCollection> GetProductTypes(std::string& errorMessage, std::string filter = std::string());
		std::vector<productionCollection> GetProduction(std::string& errorMessage, std::string filter = std::string());
		std::vector<productionListViewCollection> GetProductionList(std::string& errorMessage, std::string filter = std::string());
		std::vector<productionConsumeRawListViewCollection> GetProductionConsumeRawList(std::string& errorMessage, std::string filter = std::string());
		std::vector<productionConsumeRawsViewCollection> GetProductionConsumeRaws(std::string& errorMessage, std::string filter = std::string());
		std::vector<productionPlanViewCollection> GetProductionPlan(std::string& errorMessage, std::string filter = std::string());
		std::vector<productionPlanListViewCollection> GetProductionPlanList(std::string& errorMessage, std::string filter = std::string());
		std::vector<productionStockViewCollection> GetProductionStock(std::string& errorMessage, std::string filter = std::string());
		std::vector<productsViewCollection> GetProducts(std::string& errorMessage, std::string filter = std::string());
		std::vector<purveyorsViewCollection> GetPurveyors(std::string& errorMessage, std::string filter = std::string());
		std::vector<receiptProductListViewCollection> GetReceiptProductList(std::string& errorMessage, std::string filter = std::string());
		std::vector<receiptProductsViewCollection> GetReceiptProducts(std::string& errorMessage, std::string filter = std::string());
		std::vector<stockTransferListViewCollection> GetStockTransferList(std::string& errorMessage, std::string filter = std::string());
		std::vector<stockTransferViewCollection> GetStockTransfer(std::string& errorMessage, std::string filter = std::string());
		std::vector<refundsViewCollection> GetRefunds(std::string& errorMessage, std::string filter = std::string());
		std::vector<relationTypeCollection> GetRelationType(std::string& errorMessage, std::string filter = std::string());
		std::vector<relationsViewCollection> GetRelations(std::string& errorMessage, std::string filter = std::string());
		std::vector<returnListViewCollection> GetReturnList(std::string& errorMessage, std::string filter = std::string());
		std::vector<returnsViewCollection> GetReturns(std::string& errorMessage, std::string filter = std::string());
		std::vector<rolesCollection> GetRoles(std::string& errorMessage, std::string filter = std::string());
		std::vector<salariesViewCollection> GetSalaries(std::string& errorMessage, std::string filter = std::string());
		std::vector<salaryTypeCollection> GetSalaryType(std::string& errorMessage, std::string filter = std::string());
		std::vector<shareholdersViewCollection> GetShareholders(std::string& errorMessage, std::string filter = std::string());
		std::vector<specificationListViewCollection> GetSpecificationList(std::string& errorMessage, std::string filter = std::string());
		std::vector<specificationsViewCollection> GetSpecifications(std::string& errorMessage, std::string filter = std::string());
		std::vector<spoilageListViewCollection> GetSpoilageList(std::string& errorMessage, std::string filter = std::string());
		std::vector<spoilageViewCollection> GetSpoilage(std::string& errorMessage, std::string filter = std::string());
		std::vector<stateCollection> GetState(std::string& errorMessage, std::string filter = std::string());
		std::vector<statusCollection> GetStatus(std::string& errorMessage, std::string filter = std::string());
		std::vector<statusRuleViewCollection> GetStatusRule(std::string& errorMessage, std::string filter = std::string());
		std::vector<stockViewCollection> GetStock(std::string& errorMessage, std::string filter = std::string());
		std::vector<stockHistoryViewCollection> GetStockHistory(std::string& errorMessage, std::string filter = std::string());
		std::vector<subaccountsViewCollection> GetSubaccounts(std::string& errorMessage, std::string filter = std::string());
		std::vector<subaccountHistoryCollection> GetSubaccountHistory(std::string& errorMessage, std::string filter = std::string());
		std::vector<taxesCollection> GetTaxes(std::string& errorMessage, std::string filter = std::string());
		std::vector<timesheetViewCollection> GetTimesheet(std::string& errorMessage, std::string filter = std::string());
		std::vector<transportListViewCollection> GetTransportList(std::string& errorMessage, std::string filter = std::string());
		std::vector<transportsViewCollection> GetTransports(std::string& errorMessage, std::string filter = std::string());
		std::vector<usersViewCollection> GetUsers(std::string& errorMessage, std::string filter = std::string());
		std::vector<warehouseViewCollection> GetWarehouse(std::string& errorMessage, std::string filter = std::string());
		std::vector<warehouseTypeCollection> GetWarehouseType(std::string& errorMessage, std::string filter = std::string());
		std::vector<warehouseEmployeeViewCollection> GetWarehouseEmployee(std::string& errorMessage, std::string filter = std::string());
		std::vector<withdrawalsViewCollection> GetWithdrawals(std::string& errorMessage, std::string filter = std::string());
		std::vector<writeOffsViewCollection> GetWriteOffs(std::string& errorMessage, std::string filter = std::string());
		std::vector<writeOffListViewCollection> GetWriteOffList(std::string& errorMessage, std::string filter = std::string());
		std::vector<writeOffRawsViewCollection> GetWriteOffRaws(std::string& errorMessage, std::string filter = std::string());
		std::vector<writeOffRawListViewCollection> GetWriteOffRawList(std::string& errorMessage, std::string filter = std::string());

		//overload view methods
		std::vector<accessItemsCollection> GetAccessItems(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<accessesViewCollection> GetAccesses(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<accountTypeCollection> GetAccountType(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<accountHistoryCollection> GetAccountHistory(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<accountsCollection> GetAccounts(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<accountableViewCollection> GetAccountable(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<accountableTransactionCollection> GetAccountableTransaction(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<amortizeGroupCollection> GetAmortizeGroup(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<amortizeTypeCollection> GetAmortizeType(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<balancePaymentCollection> GetBalancePayment(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<balancePayslipCollection> GetBalancePayslip(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<balanceRefundCollection> GetBalanceRefund(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<balanceWithdrawalCollection> GetBalanceWithdrawal(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<balancesViewCollection> GetBalances(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<branchesCollection> GetBranches(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<borrowersViewCollection> GetBorrowers(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<cashboxViewCollection> GetCashbox(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<cashboxEmployeeViewCollection> GetCashboxEmployee(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<cashboxTransactionCollection> GetCashboxTransaction(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<chartOfAccountsViewCollection> GetChartOfAccounts(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<clientsViewCollection> GetClients(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<companiesCollection> GetCompanies(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<companyEmployeeViewCollection> GetCompanyEmployee(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<companyAccountViewCollection> GetCompanyAccount(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<consumeProductListViewCollection> GetConsumeProductList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<consumeProductsViewCollection> GetConsumeProducts(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<consumeRawListViewCollection> GetConsumeRawList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<consumeRawsViewCollection> GetConsumeRaws(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<creditorsCollection> GetCreditors(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<currenciesCollection> GetCurrencies(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<divisionAccountViewCollection> GetDivisionAccount(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<divisionEmployeeCollection> GetDivisionEmployee(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<divisionsCollection> GetDivisions(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<employeesViewCollection> GetEmployees(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<entriesViewCollection> GetEntries(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<entriesFullViewCollection> GetExtendedEntries(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<entrySubaccountCollection> GetEntrySubaccount(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<employeeProductViewCollection> GetEmployeeProduct(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<entryRoutingCollection> GetEntryRouting(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<entryOperationCollection> GetEntryOperation(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<financialReportCollection> GetFinancialReport(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<fixedAssetsViewCollection> GetFixedAssets(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<fixedAssetsDetailsViewCollection> GetFixedAssetsDetails(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<fixedAssetsSpecificationCollection> GetFixedAssetsSpecification(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<fixedAssetsOperationsCollection> GetFixedAssetsOperation(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<inventorizationListViewCollection> GetInventorizationList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<inventorizationsViewCollection> GetInventorizations(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<inventoryViewCollection> GetInventory(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<inventoryHistoryCollection> GetInventoryHistory(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<jobpriceViewCollection> GetJobprice(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<jobsheetViewCollection> GetJobsheet(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<locationsCollection> GetLocations(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<measuresCollection> GetMeasures(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<netCostViewCollection> GetNetCost(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<orderListViewCollection> GetOrderList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<ordersViewCollection> GetOrders(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<orderRawListViewCollection> GetOrderRawList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<orderRawsViewCollection> GetOrderRaws(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<paymentsViewCollection> GetPayments(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<paymentEmployeeCollection> GetPaymentEmployee(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<payslipsViewCollection> GetPayslips(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<payslipOrderCollection> GetPayslipOrder(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<percentRateCollection> GetPercentRate(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<photosCollection> GetPhotos(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<positionsCollection> GetPositions(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<productBranchViewCollection> GetProductBranch(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<pricesViewCollection> GetPrices(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<productTypeCollection> GetProductTypes(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<productionCollection> GetProduction(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<productionListViewCollection> GetProductionList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<productionConsumeRawListViewCollection> GetProductionConsumeRawList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<productionConsumeRawsViewCollection> GetProductionConsumeRaws(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<productionPlanViewCollection> GetProductionPlan(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<productionPlanListViewCollection> GetProductionPlanList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<productionStockViewCollection> GetProductionStock(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<productsViewCollection> GetProducts(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<purveyorsViewCollection> GetPurveyors(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<receiptProductListViewCollection> GetReceiptProductList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<receiptProductsViewCollection> GetReceiptProducts(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<stockTransferListViewCollection> GetStockTransferList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<stockTransferViewCollection> GetStockTransfer(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<refundsViewCollection> GetRefunds(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<relationTypeCollection> GetRelationType(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<relationsViewCollection> GetRelations(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<returnListViewCollection> GetReturnList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<returnsViewCollection> GetReturns(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<rolesCollection> GetRoles(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<salariesViewCollection> GetSalaries(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<salaryTypeCollection> GetSalaryType(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<shareholdersViewCollection> GetShareholders(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<specificationListViewCollection> GetSpecificationList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<specificationsViewCollection> GetSpecifications(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<spoilageListViewCollection> GetSpoilageList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<spoilageViewCollection> GetSpoilage(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<stateCollection> GetState(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<statusCollection> GetStatus(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<statusRuleViewCollection> GetStatusRule(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<stockViewCollection> GetStock(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<stockHistoryViewCollection> GetStockHistory(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<subaccountsViewCollection> GetSubaccounts(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<subaccountHistoryCollection> GetSubaccountHistory(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<taxesCollection> GetTaxes(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<timesheetViewCollection> GetTimesheet(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<transportListViewCollection> GetTransportList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<transportsViewCollection> GetTransports(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<usersViewCollection> GetUsers(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<warehouseViewCollection> GetWarehouse(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<warehouseTypeCollection> GetWarehouseType(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<warehouseEmployeeViewCollection> GetWarehouseEmployee(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<withdrawalsViewCollection> GetWithdrawals(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<writeOffsViewCollection> GetWriteOffs(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<writeOffListViewCollection> GetWriteOffList(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<writeOffRawsViewCollection> GetWriteOffRaws(int offset, std::string& errorMessage, std::string filter = std::string());
		std::vector<writeOffRawListViewCollection> GetWriteOffRawList(int offset, std::string& errorMessage, std::string filter = std::string());

		//create methods
		bool CreateAccessItem(int accessItemID, std::string accessItemEng, std::string accessItemRu,std::string accessItemDivision, std::string& errorMessage);
		bool CreateAccess(int accessID, int roleID, int accessItemID, std::string& errorMessage);
		bool CreateAccountType(int accountTypeID, std::string accountTypeName, int accountTypeNumber, std::string accountTypeComment, std::string& errorMessage);
		bool CreateAccount(int accountID, std::string accountNumber, double accountStartBalance, double accountCurrentBalance, std::string& errorMessage);
		bool CreateAccountHistory(int accountHistoryID, int accountID, std::string accountNumber, double accountStartBalance, 
			double aCurrentBalance, std::string fromDate, std::string tillDate, std::string& errorMessage);
		bool CreateAccountable(int userID, std::string aInformation, std::string& errorMessage);
		bool CreateAccountableTransaction(int accTransID, int accountableID, double startValue, double endValue, int operationID, std::string& errorMessage);
		bool CreateAmortizeGroup(int amortizeGroupID, int groupNumber, int fromMonth, int toMonth, std::string& errorMessage);
		bool CreateAmortizeType(int amortizeTypeID, std::string typeName, std::string typeCode, int amortizePercent, double valueDepOnSales, int amortize_year, double amortize_coef, std::string& errorMessage);
		bool CreateBalancePayment(int balancePaymentID, int balanceID, int paymentID, std::string& errorMessage);
		bool CreateBalancePayslip(int balancePayslipID, int bananceID, int payslipID, std::string& errorMessage);
		bool CreateBalanceRefund(int balanceRefundID, int balanceID, int refundID, std::string& errorMessage);
		bool CreateBalanceWithdrawal(int balanceWithdrawalID, int balanceID, int withdrawalID, std::string& errorMessage);
		bool CreateBalance(int balanceID, int userID, int subaccountID, std::string& errorMessage);
		bool CreateBranch(int branchID, std::string branchName, std::string branchAddress, std::string branchPhone, std::string bCommnet, std::string& errorMessage);
		bool CreateBorrower(int userID, std::string bComment, std::string& errorMessage);
		bool CreateCashbox(int cashboxID, int subAccID, std::string information, std::string cashboxAddress, std::string& errorMessage);
		bool CreateCashboxTransaction(int cashboxTransactionID, int cashboxID, int cashierID, int accountantID, int ownerID, int paymentID, int withdrawalID, std::string& errorMessage);
		bool CreateCashboxEmployee(int cashboxEmployeeID, int ceCashboxID, int ceEmployeeID, std::string& errorMessage);
		bool CreateChartOfAccount(int chartOfAccountID, std::string chartOfAccountNumber, std::string chartOfAccountName, int accountTypeID, std::string& errorMessage);
		bool CreateClient(int userID, std::string firm, std::string firmNumber, int locationID, std::string& errorMessage);
		bool CreateCompany(int companyID, std::string companyName, std::string companyAddress, std::string companyPhone, std::string comment,
			std::string& errorMessage);
		bool CreateCompanyAccount(int companyAccountID, int companyID, int accountID, std::string& errorMessage);
		bool CreateCompanyEmployee(int companyEmployeeID, int companyID, int employeeID, int branchID, std::string& errorMessage);
		bool CreateConsumeProductList(int consumeProductListID, int consumeProductID, int productID, double consumeListCount,
			double consumeListSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateConsumeProduct(int consumeProductID, int employeeID, std::string consumeProductDate, std::string consumeProductExecutionDate, 
			int stockEmployeeID, double consumeProductCount, double consumeProductSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateConsumeRawList(int consuleRawListID, int consumeRawID, int productID, double consumeRawListCount, double consumeRawListSum, 
			int statusID, int currencyID, std::string& errorMessage);
		bool CreateConsumeRaw(int consumeRawID, int userID, std::string consumeRawDate, std::string consumeRawExecutionDate, 
			int stockEmployeeID, double consumeRawaCount, double conusmeRawSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateCreditor(int creditorID, std::string creditorName, std::string creditorAddress, std::string creditorPhone, int companyID, int userID, std::string& errorMessage);
		bool CreateCurrency(int currenycID, int currencyCode, std::string currencyShortName, std::string currencyName, int currencyUnit, bool cMainTrade, std::string& errorMessage);
		bool CreateDivisionAccount(int divisionAccountID, int disvisionID, int accountID, std::string accountCode, std::string& errorMessage);
		bool CreateDivisionEmployee(int divisionEmployeeID, int disvisionID, int employeeID, bool isContract, std::string& errorMessage);
		bool CreateDivision(int divisionID, std::string divisionName, std::string divisionCode, std::string& errorMessage);
		bool CreateEmployee(int userID, int positionID, std::string birthDate, std::string hireDate, std::string& errorMessage);
		bool CreateEmployeeProduct(int epID, int employeeID, int productID, std::string& errorMessage);
		bool CreateEntry(int entryID, std::string entryDate, int debbitingAccountID, double entryValue, int creditingAccountID, std::string description,  std::string& errorMessage);
		bool CreateEntryRouting(int entryRoutingID, std::string entryRoutingOperation, int debbitingAccountID, int creditingAccountID, std::string& errorMessage);
		bool CreateEntryOperation(int eoID, int entryID, int operationID, std::string& errorMessage);
		bool CreateEntrySubaccount(int entrySubaccountID, int entryID, int subaccountID, std::string& errorMessage);
		bool CreateFinancialReport(int financialReportID, double account_44010, double account_55010, double account_552, 
			double account_55270, double account_553, double account_55321, double account_44020_90, double account_66010_66110, 
			double account_66020_66120, double account_66040_66140, double account_66050_66150, double account_66060_66160, 
			double account_66130, double account_66070_66170, double tax, std::string fromDate, std::string tillDate, std::string& errorMessage);
		bool CreateFixedAssets(int fixedAssetsID, int fixedAssetsSpecID, std::string inventoryNumber, double primaryCost, double stopCost,
			int serviceLife, bool isAmortize, std::string buyDate, std::string startOfOperDate, std::string endOfOperDate, int statusID,
			int fixedAssetsDeID, std::string& errorMessage);
		bool CreateFixedAssetsSpecification(int fixedAssetsSpecID, std::string fixedAssetsName, std::string factoryNumner, std::string developer,
			std::string document, std::string objectCharacters, std::string condition, std::string dateOfConstruction, std::string& errorMessage);
		bool CreateFixedAssetsDetails(int fixedAssetsDeID, int amGroupID, int amTypeID, int departmentID, std::string fixedAssetsLocation, 
			int primaryAccID, int amortizeAccID, std::string barcodeNumber, std::string& errorMessage);
		bool CreateFixedAssetsOperations(int fixedAssetsOperID, std::string operationDate, std::string operationName,
			double operationValue, bool increment, bool decrement, int fixedAssetsID, std::string& errorMessage);
		bool CreateInventorizationList(int inventorizationListID, int inventorizationID, int productID, double inventorizationListCount, 
			double inventorizationListSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateInventorization(int inventorizationID, int userID, std::string inventorizationDate, std::string inventorizationExecutionDate, 
			int stockEmployeeID, double inventorizationCount, double inventorizationSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateInventory(int inventoryID, std::string inventoryName, double inventoryCost, int departamentID, std::string location,
			int statusID, std::string startOfOperDate, std::string endOfOperDate, std::string inventoryNumber, std::string barcodeNumber, std::string& errorMessage);
		bool CreateInventoryHistory(int inventoryHistoryID, int inventoryID, std::string comment, std::string changeDate, std::string& errorMessage);
		bool CreateJobprice(int jobpriceID, int producrtID, double jobpriceValue, int currencyID, double jobpriceVolume, int measureID, int positionID, std::string& errorMessage);
		bool CreateJobsheet(int jobsheetID, std::string jobsheetDate, double jobsheetCount, int productID, int employeeID, std::string& errorMessage);
		bool CreateLocation(int locationID, std::string countryName, std::string countryCode, std::string regionName
			, std::string cityName, std::string& errorMessage);
		bool CreateMeasure(int measureID, std::string measureName, std::string measureShortName, int measureUnit, std::string& errorMessage);
		bool CreateNetCost(int netCostID, std::string netCostDate, double netCostValue, int currencyID, int productID, bool netCostIsOutdated, std::string& errorMessage);
		bool CreateOrderList(int orderListID, int orderID, int productID, double orderListCount, double orderListSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateOrder(int orderID, int clientID, std::string orderDate, std::string orderExecutionDate, int employeeID,
			double orderCount, double orderSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateOrderRawList(int orderRawListID, int orderRawID, int productID, double orderRawListCount, double orderRawSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateOrderRaw(int orderRawID, int productID, std::string orderRawDate, std::string orderRawExecutionDate, 
			int employeeID, double orderRawCount, double orderRawSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreatePayment(int paymentID, std::string paymentDate, double paymentValue, std::string paymentTarget, int userID, int currencyID,
			int statusID, int accountID, int subaccountID, std::string pWho, int cahsboxAccID, std::string& errorMessage);
		bool CreatePaymentEmployee(int paymentEmployeeID, int paymentID, int employeeID, std::string& errorMessage);
		bool CreatePayslip(int payslipID, std::string payslipDate, double payslipValue, int statusID, int currencyID, std::string& errorMessage);
		bool CreatePayslipOrder(int payslipOrderID, int payslipID, int orderID, std::string& errorMessage);
		bool CreatePercentRate(int percentRateID, double percentRateValue, std::string percentRateCondition, int positionID, std::string& errorMessage);
		bool CreatePhoto(int photoID, int userID, int productionID, std::string photoSource, std::string& errorMessage);
		bool CreatePosition(int positionID, std::string positionName, std::string& errorMessage);
		bool CreatePrice(int priceID, std::string priceDate, double priceValue, int currencyID, int productID, bool priceIsOutdated, std::string& errorMessage);
		bool CreateProductType(int productTypeID, std::string productTypeName, std::string productTypeShortName, std::string productTypeCode, std::string& errorMessage);
		bool CreateProductBranch(int pbID, int productID, int branchID, std::string& errorMessage);
		bool CreateProduction(int productionID, std::string productionDate, std::string productionExpiryDate, std::string productionSessionStart, 
			std::string productionSessionEnd, std::string& errorMessage);
		bool CreateProductionList(int productionListID, int productionID, int productID, double productionListCount, double productionListSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateProductionConsumeRawList(int productionConsRawListID, int productionConsumeRawID, int productID, double productionConsRawListCount, double productionConsRawListSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateProductionConsumeRaw(int productionConsumeRawID, int userID, std::string productionConsumeRawDate, 
			std::string productionConsumeRawExecutionDate, int stockEmployeeID, double productionConsumeRawCount,
			double productionConsumeRawSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateProductionPlanList(int productionPlanListID, int productionPlanID, int productID, double productionPlanListCount, double productionPlanListSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateProductionPlan(int productionPlanID, std::string productionPlanDate, int employeeID, double productionPlanListCount, double productionPlanListSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateProductionStock(int productionStockID, int productID, double productionStockCount, double productionStockSum, int statusID, int currencyID, 
			int warehouseID, std::string& errorMessage);
		bool CreateProduct(int productID, int companyID, std::string productName, double productVolume, int measureID, double productPrice, int productTypeID, int productShelfLife, 
			int currencyID, std::string& errorMessage);
		bool CreatePurveyor(int userID, std::string purveyorCompanyName, int locationID, std::string& errorMessage);
		bool CreateReceiptProductList(int receiptProductListID, int receiptProductID, int productID, double receiptProductCount,
			double receiptProductSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateReceiptProduct(int receiptProductID, int employeeID, std::string receiptProductDate, 
			std::string receiptProductExecutionDate, int stockEmployeeID, double receiptProductCount, double receiptProductSum, 
			int statusID, int currencyID, std::string& errorMessage);
		bool CreateStockTransferList(int stockTransferListID, int stockTransferID, int productID, double stockTransferListCount, double stockTransferListSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateStockTransfer(int stockTransferID, int employeeID, std::string stockTransferDate, std::string stockTransferExecutionDate, int stockEmployeeID, double stockTransferCount, double stockTransferSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateRefund(int refundID, std::string refuntDate, double refundValue, int userID, int currencyID, std::string& errorMessage);
		bool CreateRelationType(int relationTypeID, std::string relationTypeName, std::string comment, std::string& errorMessage);
		bool CreateRelation(int relationID, int user1ID, int user2ID, int relationTypeID, std::string& errorMessage);
		bool CreateReturnList(int returnListID, int returnID, int productID, double returnListCount, double returnListSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateReturn(int returnID, int clientID, std::string returnDate, std::string returnExecutionDate, int employeeID, double returnCount, double returnSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateRole(int roleID, std::string roleCode, std::string roleName, std::string comment, std::string& errorMessage);
		bool CreateSalary(int salaryID, int userID, double salaryValue, int currencyID, int salaryTypeID, std::string salaryDate, bool salaryBonus, std::string& errorMessage);
		bool CreateSalaryType(int salaryTypeID, std::string salaryTypeCode, std::string salaryTypeName, std::string& errorMessage);
		bool CreateShareholder(int userID, double countOfStock, std::string sInformation, std::string& errorMessage);
		bool CreateSpecificationList(int specificationListID, int specificationID, int productID, double specificationListCount, std::string& errorMessage);
		bool CreateSpecification(int specificationID, int productID, double specificationSum, int currencyID, int employeeID, std::string sDate, std::string& errorMessage);
		bool CreateSpoilageList(int spoilageListID, int spoilageID, int productID, double spoilageListCount, double spoilageListSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateSpoilage(int spoilageID, std::string spoilageDate, int employeeID, double spoilageCount, double spoilageSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateState(int stateID, int universalID, int statusID, std::string statusLastChange, std::string& errorMessage);
		bool CreateStatus(int statusID, std::string statusCode, std::string statusName, std::string comment, std::string& errorMessage);
		bool CreateStatusRule(int statusRuleID, std::string statusRuleOperation, int statusID, std::string& errorMessage);
		bool CreateStock(int stockID, int productID, double stockCount, double stockSum, int statusID, int currencyID, 
			int warehouseID, std::string& errorMessage);
		bool CreateStockHistory(int stockID, int productID, double stockCount, double stockSum, int statusID, int currencyID,
			int warehouseID, std::string historyDate, std::string& errorMessage);
		bool CreateSubaccount(int subaccountID, int accountID, std::string subaccountNumber, double subaccountStartBalance, double subaccountCurrentBalance, int currencyID, int statusID,
			std::string subaccountOpenDate, std::string subaccountCloseDate, std::string subaccountDetails, std::string& errorMessage);
		bool CreateSubaccountHistory(int subaccountHistoryID, int subaccountID, double subaccountStartBalance, double subaccountCurrentBalance,
			std::string fromDate, std::string tillDate, std::string& errorMessage);
		bool CreateTax(int taxID, std::string taxName, std::string taxCode, double fixedValue, int percentValue,
			std::string formulaValue, std::string& errorMessage);
		bool CreateTimesheet(int timesheetID, int salaryID, double workedTime, std::string timesheetDate, std::string& errorMessage);
		bool CreateTransportList(int transportListID, int transportID, int productID, double transportListCount, double transportListSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateTransport(int transportID, int employeeID, std::string transportDate, std::string transportExecutionDate, 
			int stockEmployeeID, double transportCount, double transportSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateUser(int userID, std::string userEmail, std::string userName, std::string userSurname, std::string userPhone, std::string userAddress,
			int roleID, std::string userPassword, bool userActivated, std::string& errorMessage);
		bool CreateWarehouse(int warehouseID, std::string warehouseName, std::string warehouseAddress, std::string warehousePhone,
			int warehouseTypeID, int subaccountID, std::string& errorMessage);
		bool CreateWarehouseType(int warehouseTypeID, std::string wTypeCode, std::string wTypePurpose, std::string wTypeName, std::string& errorMessage);
		bool CreateWarehouseEmployee(int weID, int weWarehouseID, int weEmployeeID, std::string& errorMessage);
		bool CreateWithdrawal(int withdrawalID, std::string withdrawalDate, double withdrawalValue, int userID, int sabID, std::string withdrawalTarget, int currencyID,
			int statusID, int accountID, std::string pWho, int cashboxAccID, std::string& errorMessage);
		bool CreateWriteOffList(int writeOffListID, int writeOffID, int productID, double writeOffListCount, double writeOffListSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateWriteOff(int writeOffID, int userID, std::string writeOffDate, std::string executionDate, int employeeID, double writeOffCount, double writeOffSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateWriteOffRawList(int writeOffRawListID, int writeOffRawID, int productID, double writeOffRawListCount, double writeOffRawListSum, int statusID, int currencyID, std::string& errorMessage);
		bool CreateWriteOffRaw(int writeOffRawID, int employeeID, std::string writeOffRawDate, std::string executionDate, int stockEmployeeID, double writeOffRawListCount, double writeOffRawListSum, int statusID, int currencyID, std::string& errorMessage);

		//delete methods
		bool DeleteAccessItem(int id, std::string& errorMessage);
		bool DeleteAccess(int id, std::string& errorMessage);
		bool DeleteAccountType(int id, std::string& errorMessage);
		bool DeleteAccount(int id, std::string& errorMessage);
		bool DeleteAccountable(int id, std::string& errorMessage);
		bool DeleteAccountableTransaction(int id, std::string& errorMessage);
		bool DeleteAmortizeGroup(int id, std::string& errorMessage);
		bool DeleteAmortizeType(int id, std::string& errorMessage);
		bool DeleteAccountHistory(int id, std::string& errorMessage);
		bool DeleteBalancePayment(int bID, int pID, std::string& errorMessage);
		bool DeleteBalancePayslip(int bID, int pID, std::string& errorMessage);
		bool DeleteBalanceRefund(int bID, int rID, std::string& errorMessage);
		bool DeleteBalanceWithdrawal(int bID, int wID, std::string& errorMessage);
		bool DeleteBalance(int id, std::string& errorMessage);
		bool DeleteBranch(int id, std::string& errorMessage);
		bool DeleteBorrower(int id, std::string& errorMessage);
		bool DeleteCashbox(int id, std::string& errorMessage);
		bool DeleteCashboxTransaction(int id, std::string& errorMessage);
		bool DeleteCashboxEmployee(int cID, int eID, std::string& errorMessage);
		bool DeleteChartOfAccount(int id, std::string& errorMessage);
		bool DeleteClient(int id, std::string& errorMessage);
		bool DeleteCompany(int id, std::string& errorMessage);
		bool DeleteCompanyAccount(int cID, int aID, std::string& errorMessage);
		bool DeleteCompanyEmployee(int cID, int eID, std::string& errorMessage);
		bool DeleteCurrency(int id, std::string& errorMessage);
		bool DeleteCreditor(int id, std::string& errorMessage);
		bool DeleteConsumeProduct(int id, std::string& errorMessage);
		bool DeleteItemInConsumeProductList(int id, std::string& errorMessage);
		bool DeleteListByConsumeProductID(int id, std::string& errorMessage);
		bool DeleteConsumeRaw(int id, std::string& errorMessage);
		bool DeleteItemInConsumeRawList(int id, std::string& errorMessage);
		bool DeleteListByConsumeRawID(int id, std::string& errorMessage);
		bool DeleteDivisionAccount(int divisionID, int accountID, std::string& errorMessage);
		bool DeleteDivisionEmployee(int divisionID, int employeeID, std::string& errorMessage);
		bool DeleteDivision(int id, std::string& errorMessage);
		bool DeleteEmployee(int id, std::string& errorMessage);
		bool DeleteEmployeeProduct(int eID, int pID, std::string& errorMessage);
		bool DeleteEntry(int id, std::string& errorMessage); 
		bool DeleteEntrySubaccount(int eID, int sID, std::string& errorMessage); 
		bool DeleteEntryOperation(int eID, int oID, std::string& errorMessage);
		bool DeleteEntryRouting(int id, std::string& errorMessage);
		bool DeleteFinancialReport(int id, std::string& errorMessage);
		bool DeleteFixedAssets(int id, std::string& errorMessage);
		bool DeleteFixedAssetsOperation(int id, std::string& errorMessage);
		bool DeleteFixedAssetsDetails(int id, std::string& errorMessage);
		bool DeleteFixedAssetsSpecification(int id, std::string& errorMessage);
		bool DeleteItemInInventorizationList(int id, std::string& errorMessage);
		bool DeleteListByInventorizationID(int id, std::string& errorMessage);
		bool DeleteInventorization(int id, std::string& errorMessage);
		bool DeleteInventory(int id, std::string& errorMessage);
		bool DeleteInventoryHistory(int id, std::string& errorMessage);
		bool DeleteJobprice(int id, std::string& errorMessage);
		bool DeleteJobsheet(int id, std::string& errorMessage);
		bool DeleteLocation(int id, std::string& errorMessage);
		bool DeleteMeasure(int id, std::string& errorMessage);
		bool DeleteNetCost(int id, std::string& errorMessage);
		bool DeleteItemInOrderList(int id, std::string& errorMessage);
		bool DeleteListByOrderID(int id, std::string& errorMessage);
		bool DeleteOrder(int id, std::string& errorMessage);
		bool DeleteItemInOrderRawList(int id, std::string& errorMessage);
		bool DeleteListByOrderRawID(int id, std::string& errorMessage);
		bool DeleteOrderRaw(int id, std::string& errorMessage);
		bool DeletePayment(int id, std::string& errorMessage);
		bool DeletePaymentEmployee(int pID, int eID, std::string& errorMessage);
		bool DeletePayslip(int id, std::string& errorMessage);
		bool DeletePayslipOrder(int pID, int oID, std::string& errorMessage);
		bool DeletePercentRate(int id, std::string& errorMessage);
		bool DeletePhoto(int id, std::string& errorMessage);
		bool DeletePosition(int id, std::string& errorMessage);
		bool DeleteProductBranch(int pID, int bID, std::string& errorMessage);
		bool DeletePrice(int id, std::string& errorMessage);
		bool DeleteProductType(int id, std::string& errorMessage);
		bool DeleteProduction(int id, std::string& errorMessage);
		bool DeleteItemInProductionList(int id, std::string& errorMessage);
		bool DeleteListByProductionID(int id, std::string& errorMessage);
		bool DeleteProductionConsumeRaw(int id, std::string& errorMessage);
		bool DeleteItemInProductionConsumeRawList(int id, std::string& errorMessage);
		bool DeleteListByProductionConsumeRawID(int id, std::string& errorMessage);
		bool DeleteProductionPlan(int id, std::string& errorMessage);
		bool DeleteItemInProductionPlanList(int id, std::string& errorMessage);
		bool DeleteListByProductionPlanID(int id, std::string& errorMessage);
		bool DeleteProductionStock(int id, std::string& errorMessage);
		bool DeleteProduct(int id, std::string& errorMessage);
		bool DeletePurveyor(int id, std::string& errorMessage);
		bool DeleteReceiptProduct(int id, std::string& errorMessage);
		bool DeleteItemInReceiptProductList(int id, std::string& errorMessage);
		bool DeleteListByReceiptProductID(int id, std::string& errorMessage);
		bool DeleteStockTransfer(int id, std::string& errorMessage);
		bool DeleteListByStockTransferID(int id, std::string& errorMessage);
		bool DeleteItemInStockTransferList(int id, std::string& errorMessage);
		bool DeleteRefund(int id, std::string& errorMessage);
		bool DeleteRelationType(int id, std::string& errorMessage);
		bool DeleteRelation(int id, std::string& errorMessage);
		bool DeleteItemInReturnList(int id, std::string& errorMessage);
		bool DeleteListByReturnID(int id, std::string& errorMessage);
		bool DeleteReturn(int id, std::string& errorMessage);
		bool DeleteRole(int id, std::string& errorMessage);
		bool DeleteSalary(int id, std::string& errorMessage);
		bool DeleteSalaryType(int id, std::string& errorMessage);
		bool DeleteShareholder(int id, std::string& errorMessage);
		bool DeleteSpecification(int id, std::string& errorMessage);
		bool DeleteItemInSpecificationList(int id, std::string& errorMessage);
		bool DeleteListBySpecificationID(int id, std::string& errorMessage);
		bool DeleteSpoilage(int id, std::string& errorMessage);
		bool DeleteItemInSpoilageList(int id, std::string& errorMessage);
		bool DeleteListBySpoilageID(int id, std::string& errorMessage);
		bool DeleteState(int id, std::string& errorMessage);
		bool DeleteStatus(int id, std::string& errorMessage);
		bool DeleteStatusRule(int id, std::string& errorMessage);
		bool DeleteStock(int id, std::string& errorMessage);
		bool DeleteStockHistory(int id, std::string& errorMessage);
		bool DeleteSubaccount(int id, std::string& errorMessage);
		bool DeleteSubaccountHistory(int id, std::string& errorMessage);
		bool DeleteTax(int id, std::string& errorMessage);
		bool DeleteTimesheet(int id, std::string& errorMessage);
		bool DeleteTransport(int id, std::string& errorMessage);
		bool DeleteListByTransportID(int id, std::string& errorMessage);
		bool DeleteItemInTransportList(int id, std::string& errorMessage);
		bool DeleteUser(int id, std::string& errorMessage);
		bool DeleteWarehouse(int id, std::string& errorMessage);
		bool DeleteWarehouseType(int id, std::string& errorMessage);
		bool DeleteWarehouseEmployee(int wID, int eID, std::string& errorMessage);
		bool DeleteWithdrawal(int id, std::string& errorMessage);
		bool DeleteWriteOff(int id, std::string& errorMessage);
		bool DeleteListByWriteOffID(int id, std::string& errorMessage);
		bool DeleteItemInWriteOffList(int id, std::string& errorMessage);
		bool DeleteWriteOffRaw(int id, std::string& errorMessage);
		bool DeleteListByWriteOffRawID(int id, std::string& errorMessage);
		bool DeleteItemInWriteOffRawList(int id, std::string& errorMessage);

		//update methods
		bool UpdateAccessItem(int accessItemID, std::string accessItemEng, std::string accessItemRu, std::string accessItemDivision, std::string& errorMessage);
		bool UpdateAccess(int accessID, int roleID, int accessItemID, std::string& errorMessage);
		bool UpdateAccountType(int accountTypeID, std::string accountTypeName, int accountTypeNumber, std::string accountTypeComment, std::string& errorMessage);
		bool UpdateAccount(int accountID, std::string accountNumber, double accountStartBalance, double accountCurrentBalance, std::string& errorMessage);
		bool UpdateAccountable(int userID, std::string aInformation, std::string& errorMessage);
		bool UpdateAccountableTransaction(int accTransID, int accountableID, double startValue, double endValue, int operationID, std::string& errorMessage);
		bool UpdateAccountHistory(int accountHistoryID, int accountID, std::string accountNumber, double accountStartBalance,
			double accountCurrentBalance, std::string fromDate, std::string tillDate, std::string& errorMessage);
		bool UpdateAmortizeGroup(int amortizeGroupID, int groupNumber, int fromMonth, int toMonth, std::string& errorMessage);
		bool UpdateAmortizeType(int amortizeTypeID, std::string typeName, std::string typeCode, int amortizePercent, double valueDepOnSales, int amortize_year, double amortize_coef, std::string& errorMessage);
		bool UpdateBalancePayment(int balancePaymentID, int balanceID, int paymentID, std::string& errorMessage);
		bool UpdateBalancePayslip(int balancePayslipID, int bananceID, int payslipID, std::string& errorMessage);
		bool UpdateBalanceRefund(int balanceRefundID, int balanceID, int refundID, std::string& errorMessage);
		bool UpdateBalanceWithdrawal(int balanceWithdrawalID, int balanceID, int withdrawalID, std::string& errorMessage);
		bool UpdateBalance(int balanceID, int userID, int subaccountID, std::string& errorMessage);
		bool UpdateBranch(int branchID, std::string branchName, std::string branchAddress, std::string branchPhone, std::string bCommnet, std::string& errorMessage);
		bool UpdateBorrower(int userID, std::string bComment, std::string& errorMessage);
		bool UpdateCashbox(int cashboxID, int subAccID, std::string information, std::string cashboxAddress, std::string& errorMessage);
		bool UpdateCashboxTransaction(int cashboxTransactionID, int cashboxID, int cashierID, int accountantID, int ownerID, int paymentID, int withdrawalID, std::string& errorMessage);
		bool UpdateCashboxEmployee(int cashboxEmployeeID, int ceCashboxID, int ceEmployeeID, std::string& errorMessage);
		bool UpdateChartOfAccount(int chartOfAccountID, std::string chartOfAccountNumber, std::string chartOfAccountName, int accountTypeID, std::string& errorMessage);
		bool UpdateClient(int userID, std::string firm, std::string firmNumber, int locationID, std::string& errorMessage);
		bool UpdateCompany(int companyID, std::string companyName, std::string companyAddress, std::string companyPhone, std::string comment,
			std::string& errorMessage);
		bool UpdateCompanyAccount(int companyAccountID, int companyID, int accountID, std::string& errorMessage);
		bool UpdateCompanyEmployee(int companyEmployeeID, int companyID, int employeeID, int branchID, std::string& errorMessage);
		bool UpdateConsumeProductList(int consumeProductListID, int consumeProductID, int productID, double consumeListCount,
			double consumeListSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateConsumeProduct(int consumeProductID, int employeeID, std::string consumeProductDate, std::string consumeProductExecutionDate,
			int stockEmployeeID, double consumeProductCount, double consumeProductSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateConsumeRawList(int consuleRawListID, int consumeRawID, int productID, double consumeRawListCount, double consumeRawListSum,
			int statusID, int currencyID, std::string& errorMessage);
		bool UpdateConsumeRaw(int consumeRawID, int userID, std::string consumeRawDate, std::string consumeRawExecutionDate,
			int stockEmployeeID, double consumeRawaCount, double conusmeRawSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateCurrency(int currenycID, int currencyCode, std::string currencyShortName, std::string currencyName, int currencyUnit, bool cMainTrade, std::string& errorMessage);
		bool UpdateCreditor(int creditorID, std::string creditorName, std::string creditorAddress, std::string creditorPhone, int companyID, int userID, std::string& errorMessage);
		bool UpdateDivisionAccount(int divisionAccountID, int divisionID, int accountID, std::string accountCode, std::string& errorMessage);
		bool UpdateDivisionEmployee(int divisionEmployeeID, int divisionID, int employeeID, bool isContract, std::string& errorMessage);
		bool UpdateDivision(int divisionID, std::string divisionName, std::string divisionCode, std::string& errorMessage);
		bool UpdateEmployee(int userID, int positionID, std::string birthDate, std::string hireDate, std::string& errorMessage);
		bool UpdateEmployeeProduct(int epID, int employeeID, int productID, std::string& errorMessage);
		bool UpdateEntry(int entryID, std::string entryDate, int debbitingAccountID, double entryValue, int creditingAccountID, std::string description, std::string& errorMessage);
		bool UpdateEntryRouting(int entryRoutingID, std::string entryRoutingOperation, int debbitingAccountID, int creditingAccountID, std::string& errorMessage);
		bool UpdateEntryOperation(int eoID, int entryID, int operationID, std::string& errorMessage);
		bool UpdateEntrySubaccount(int entrySubaccountID, int entryID, int subaccountID, std::string& errorMessage);
		bool UpdateFinancialReport(int financialReportID, double account_44010, double account_55010, double account_552,
			double account_55270, double account_553, double account_55321, double account_44020_90, double account_66010_66110,
			double account_66020_66120, double account_66040_66140, double account_66050_66150, double account_66060_66160,
			double account_66130, double account_66070_66170, double tax, std::string fromDate, std::string tillDate, std::string& errorMessage);
		bool UpdateFixedAssets(int fixedAssetsID, int fixedAssetsSpecID, std::string inventoryNumber, double primaryCost, double stopCost,
			int serviceLife, bool isAmortize, std::string buyDate, std::string startOfOperDate, std::string endOfOperDate, int statusID,
			int fixedAssetsDeID, std::string& errorMessage);
		bool UpdateFixedAssetsSpecification(int fixedAssetsSpecID, std::string fixedAssetsName, std::string factoryNumner, std::string developer,
			std::string document, std::string objectCharacters, std::string condition, std::string dateOfConstruction, std::string& errorMessage);
		bool UpdateFixedAssetsDetails(int fixedAssetsDeID, int amGroupID, int amTypeID, int departmentID, std::string fixedAssetsLocation,
			int primaryAccID, int amortizeAccID, std::string barcodeNumber, std::string& errorMessage);
		bool UpdateFixedAssetsOperations(int fixedAssetsOperID, std::string operationDate, std::string operationName,
			double operationValue, bool increment, bool decrement, int fixedAssetsID, std::string& errorMessage);
		bool UpdateInventorizationList(int inventorizationListID, int inventorizationID, int productID, double inventorizationListCount,
			double inventorizationListSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateInventorization(int inventorizationID, int userID, std::string inventorizationDate, std::string inventorizationExecutionDate,
			int stockEmployeeID, double inventorizationCount, double inventorizationSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateInventory(int inventoryID, std::string inventoryName, double inventoryCost, int departamentID, std::string location,
			int statusID, std::string startOfOperDate, std::string endOfOperDate, std::string inventoryNumber, std::string barcodeNumber, std::string& errorMessage);
		bool UpdateInventoryHistory(int inventoryHistoryID, int inventoryID, std::string comment, std::string changeDate, std::string& errorMessage);
		bool UpdateJobprice(int jobpriceID, int producrtID, double jobpriceValue, int currencyID, double jobpriceVolume, int measureID, int positionID, std::string& errorMessage);
		bool UpdateJobsheet(int jobsheetID, std::string jobsheetDate, double jobsheetCount, int productID, int employeeID, std::string& errorMessage);
		bool UpdateLocation(int locationID, std::string countryName, std::string countryCode, std::string regionName
			, std::string cityName, std::string& errorMessage);
		bool UpdateMeasure(int measureID, std::string measureName, std::string measureShortName, int measureUnit, std::string& errorMessage);
		bool UpdateNetCost(int netCostID, std::string netCostDate, double netCostValue, int currencyID, int productID, bool netCostIsOutdated, std::string& errorMessage);
		bool UpdateOrderList(int orderListID, int orderID, int productID, double orderListCount, double orderListSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateOrder(int orderID, int clientID, std::string orderDate, std::string orderExecutionDate, int employeeID,
			double orderCount, double orderSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateOrderRawList(int orderRawListID, int orderRawID, int productID, double orderRawListCount, double orderRawSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateOrderRaw(int orderRawID, int productID, std::string orderRawDate, std::string orderRawExecutionDate,
			int employeeID, double orderRawCount, double orderRawSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdatePayment(int paymentID, std::string paymentDate, double paymentValue, std::string paymentTarget, int userID, int currencyID, 
			int statusID, int accountID, int subaccountID, std::string pWho, int cashboxAccID, std::string& errorMessage);
		bool UpdatePaymentEmployee(int paymentEmployeeID, int paymentID, int employeeID, std::string& errorMessage);
		bool UpdatePayslip(int payslipID, std::string payslipDate, double payslipValue, int statusID, int currencyID, std::string& errorMessage);
		bool UpdatePercentRate(int percentRateID, double percentRateValue, std::string percentRateCondition, int positionID, std::string& errorMessage);
		bool UpdatePhoto(int photoID, int userID, int productionID, std::string photoSource, std::string& errorMessage);
		bool UpdatePosition(int positionID, std::string positionName, std::string& errorMessage);
		bool UpdatePrice(int priceID, std::string priceDate, double priceValue, int currencyID, int productID, bool priceIsOutdated, std::string& errorMessage);
		bool UpdateProductType(int productTypeID, std::string productTypeName, std::string productTypeShortName, std::string productTypeCode, std::string& errorMessage);
		bool UpdateProductBranch(int pbID, int productID, int branchID, std::string& errorMessage);
		bool UpdateProduction(int productionID, std::string productionDate, std::string productionExpiryDate, std::string productionSessionStart,
			std::string productionSessionEnd, std::string& errorMessage);
		bool UpdateProductionList(int productionListID, int productionID, int productID, double productionListCount, double productionListSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateProductionConsumeRawList(int productionConsRawListID, int productionConsumeRawID, int productID, double productionConsRawListCount, double productionConsRawListSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateProductionConsumeRaw(int productionConsumeRawID, int userID, std::string productionConsumeRawDate,
			std::string productionConsumeRawExecutionDate, int stockEmployeeID, double productionConsumeRawCount,
			double productionConsumeRawSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateProductionPlanList(int productionPlanListID, int productionPlanID, int productID, double productionPlanListCount, double productionPlanListSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateProductionPlan(int productionPlanID, std::string productionPlanDate, int employeeID, double productionPlanListCount, double productionPlanListSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateProductionStock(int productionStockID, int productID, double productionStockCount, double productionStockSum, 
			int statusID, int currencyID, int warehouseID, std::string& errorMessage);
		bool UpdateProduct(int productID, int companyID, std::string productName, double productVolume, int measureID, double productPrice, int productTypeID, int productShelfLife,
			int currencyID, std::string& errorMessage);
		bool UpdatePurveyor(int userID, std::string purveyorCompanyName, int locationID, std::string& errorMessage);
		bool UpdateReceiptProductList(int receiptProductListID, int receiptProductID, int productID, double receiptProductCount, double receiptProductSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateReceiptProduct(int receiptProductID, int employeeID, std::string receiptProductDate,
			std::string receiptProductExecutionDate, int stockEmployeeID, double receiptProductCount, double receiptProductSum,
			int statusID, int currencyID, std::string& errorMessage);
		bool UpdateStockTransferList(int stockTransferListID, int stockTransferID, int productID, double stockTransferListCount, double stockTransferListSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateStockTransfer(int stockTransferID, int employeeID, std::string stockTransferDate, std::string stockTransferExecutionDate, int stockEmployeeID, double stockTransferCount, double stockTransferSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateRefund(int refundID, std::string refuntDate, double refundValue, int userID, int currencyID, std::string& errorMessage);
		bool UpdateRelationType(int relationTypeID, std::string relationTypeName, std::string comment, std::string& errorMessage);
		bool UpdateRelation(int relationID, int user1ID, int user2ID, int relationTypeID, std::string& errorMessage);
		bool UpdateReturnList(int returnListID, int returnID, int productID, double returnListCount, double returnListSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateReturn(int returnID, int clientID, std::string returnDate, std::string returnExecutionDate, int employeeID, double returnCount, double returnSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateRole(int roleID, std::string roleCode, std::string roleName, std::string comment, std::string& errorMessage);
		bool UpdateSalary(int salaryID, int userID, double salaryValue, int currencyID, int salaryTypeID, std::string salaryDate, bool salaryBonus, std::string& errorMessage);
		bool UpdateSalaryType(int salaryTypeID, std::string salaryTypeCode, std::string salaryTypeName, std::string& errorMessage);
		bool UpdateShareholder(int userID, double countOfStock, std::string sInformation, std::string& errorMessage);
		bool UpdateSpecificationList(int specificationListID, int specificationID, int productID, double specificationListCount, std::string& errorMessage);
		bool UpdateSpecification(int specificationID, int productID, double specificationSum, int currencyID, int employeeID, std::string sDate, std::string& errorMessage);
		bool UpdateSpoilageList(int spoilageListID, int spoilageID, int productID, double spoilageListCount, double spoilageListSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateSpoilage(int spoilageID, std::string spoilageDate, int employeeID, double spoilageCount, double spoilageSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateState(int stateID, int universalID, int statusID, std::string statusLastChange, std::string& errorMessage);
		bool UpdateStatus(int statusID, std::string statusCode, std::string statusName, std::string comment, std::string& errorMessage);
		bool UpdateStatusRule(int statusRuleID, std::string statusRuleOperation, int statusID, std::string& errorMessage);
		bool UpdateStock(int stockID, int productID, double stockCount, double stockSum, int statusID, int currencyID, 
			int warehouseID, std::string& errorMessage);
		bool UpdateStockHistory(int stockID, int productID, double stockCount, double stockSum, int statusID, int currencyID,
			int warehouseID, std::string historyDate, std::string& errorMessage);
		bool UpdateSubaccount(int subaccountID, int accountID, std::string subaccountNumber, double subaccountStartBalance, double subaccountCurrentBalance, int currencyID, int statusID,
			std::string subaccountOpenDate, std::string subaccountCloseDate, std::string subaccountDetails, std::string& errorMessage);
		bool UpdateSubaccountHistory(int subaccountHistoryID, int subaccountID, double subaccountStartBalance, double subaccountCurrentBalance,
			std::string fromDate, std::string tillDate, std::string& errorMessage);
		bool UpdateTax(int taxID, std::string taxName, std::string taxCode, double fixedValue, int percentValue,
			std::string formulaValue, std::string& errorMessage);
		bool UpdateTimesheet(int timesheetID, int salaryID, double workedTime, std::string timesheetDate, std::string& errorMessage);
		bool UpdateTransportList(int transportListID, int transportID, int productID, double transportListCount, double transportListSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateTransport(int transportID, int employeeID, std::string transportDate, std::string transportExecutionDate,
			int stockEmployeeID, double transportCount, double transportSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateUser(int userID, std::string userEmail, std::string userName, std::string userSurname, std::string userPhone, std::string userAddress,
			int roleID, std::string userPassword, bool userActivated, std::string& errorMessage);
		bool UpdateWarehouse(int warehouseID, std::string warehouseName, std::string warehouseAddress, std::string warehousePhone,
			int warehouseTypeID, int subaccountID, std::string& errorMessage);
		bool UpdateWarehouseType(int warehouseTypeID, std::string wTypeCode, std::string wTypePurpose, std::string wTypeName, std::string& errorMessage);
		bool UpdateWarehouseEmployee(int weID, int weWarehouseID, int weEmployeeID, std::string& errorMessage);
		bool UpdateWithdrawal(int withdrawalID, std::string withdrawalDate, double withdrawalValue, int userID, int sabID, 
			std::string withdrawalTarget, int currencyID, int statusID, int accountID, std::string pWho, int cashboxAccID, std::string& errorMessage);
		bool UpdateWriteOffList(int writeOffListID, int writeOffID, int productID, double writeOffListCount, double writeOffListSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateWriteOff(int writeOffID, int userID, std::string writeOffDate, std::string executionDate, int employeeID, double writeOffCount, double writeOffSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateWriteOffRawList(int writeOffRawListID, int writeOffRawID, int productID, double writeOffRawListCount, double writeOffRawListSum, int statusID, int currencyID, std::string& errorMessage);
		bool UpdateWriteOffRaw(int writeOffRawID, int employeeID, std::string writeOffRawListDate, std::string executionDate, int stockEmployeeID, double writeOffRawListCount, double writeOffRawListSum, int statusID, int currencyID, std::string& errorMessage);

		//Concatenate filters
		std::string ConcatenateFilters(std::vector<std::string> filterList);
		
		//Filter factory
		std::string GetFilterForAccessItem(int accessItemID, std::string accessItemEng, std::string accessItemRu, std::string accessItemDivision);
		std::string GetFilterForAccess(int accessID, int roleID, int accessItemID);
		std::string GetFilterForAccountType(int accountTypeID, std::string accountTypeName, int accountTypeNumber, std::string accountTypeComment);
		std::string GetFilterForAccount(int accountID, std::string accountNumber, double accountStartBalance, double accountCurrentBalance);
		std::string GetFilterForAccountable(int userID, std::string name, std::string surname, std::string phone,
			std::string address, int roleID, std::string aInformation);
		std::string GetFilterForAccountableTransaction(int accTransID, int accountableID, double startValue, double endValue, int operationID);
		std::string GetFilterForAccountHistory(int accountHistoryID, int accountID, std::string accountNumber, double accountStartBalance,
			double aCurrentBalance, std::string fromDate, std::string tillDate);
		std::string GetFilterForAmortizeGroup(int amortizeGroupID, int groupNumber, int fromMonth, int toMonth);
		std::string GetFilterForAmortizeType(int amortizeTypeID, std::string typeName, std::string typeCode, int amortizePercent, double valueDepOnSales, int amortize_year, double amortize_coef);
		std::string GetFilterForBalancePayment(int balancePaymentID, int balanceID, int paymentID);
		std::string GetFilterForBalancePayslip(int balancePayslipID, int bananceID, int payslipID);
		std::string GetFilterForBalanceRefund(int balanceRefundID, int balanceID, int refundID);
		std::string GetFilterForBalanceWithdrawal(int balanceWithdrawalID, int balanceID, int withdrawalID);
		std::string GetFilterForBalance(int balanceID, int userID, int subaccountID);
		std::string GetFilterForBranch(int branchID, std::string branchName, std::string branchAddress, std::string branchPhone, std::string bCommnet);
		std::string GetFilterForBorrower(int userID, std::string name, std::string surname, std::string phone,
			std::string address, int roleID, std::string bComment);
		std::string GetFilterForCashbox(int cashboxID, int subAccID, std::string information, std::string cashboxAddress);
		std::string GetFilterForCashboxTransaction(int cashboxTransactionID, int cashboxID, int cashierID, int accountantID, int ownerID, int paymentID, int withdrawalID);
		std::string GetFilterForCashboxEmployee(int cashboxEmployeeID, int ceCashboxID, int ceEmployeeID);
		std::string GetFilterForChartOfAccount(int chartOfAccountID, std::string chartOfAccountNumber, std::string chartOfAccountName, int accountTypeID);
		std::string GetFilterForClient(int clientID, std::string name, std::string surname, std::string phone,
			std::string address, std::string firm, std::string firmNumber, std::string password, std::string email, 
			bool activated, int roleID, int locationID);
		std::string GetFilterForCompany(int companyID, std::string companyName, std::string companyAddress, std::string companyPhone, std::string comment);
		std::string GetFilterForCompanyAccount(int companyAccountID, int companyID, int accountID);
		std::string GetFilterForCompanyEmployee(int companyEmployeeID, int companyID, int employeeID, int branchID);
		std::string GetFilterForConsumeProductList(int consumeProductListID, int consumeProductID, int productID, double consumeListCount,
			double consumeListSum, int statusID, int currencyID);
		std::string GetFilterForConsumeProduct(int consumeProductID, int employeeID, std::string consumeProductDate, std::string consumeProductExecutionDate,
			int stockEmployeeID, double consumeProductCount, double consumeProductSum, int statusID, int currencyID);
		std::string GetFilterForConsumeProductForPeriod(int consumeProductID, int employeeID, std::string consumeProductDate, std::string consumeProductExecutionDate,
			int stockEmployeeID, double consumeProductCount, double consumeProductSum, int statusID, int currencyID, std::string fromDate, std::string toDate);
		std::string GetFilterForConsumeRawList(int consuleRawListID, int consumeRawID, int productID, double consumeRawListCount, double consumeRawListSum,
			int statusID, int currencyID);
		std::string GetFilterForConsumeRaw(int consumeRawID, int userID, std::string consumeRawDate, std::string consumeRawExecutionDate,
			int stockEmployeeID, double consumeRawaCount, double conusmeRawSum, int statusID, int currencyID);
		std::string GetFilterForConsumeRawForPeriod(int consumeRawID, int userID, std::string consumeRawDate, std::string consumeRawExecutionDate,
			int stockEmployeeID, double consumeRawaCount, double conusmeRawSum, int statusID, int currencyID, std::string fromDate, std::string toDate);
		std::string GetFilterForCurrency(int currenycID, int currencyCode, std::string currencyShortName, std::string currencyName, int currencyUnit, bool cMainTrade);
		std::string GetFilterForCreditor(int creditorID, std::string creditorName, std::string creditorAddress, std::string creditorPhone, int companyID, int userID);
		std::string GetINFilterForCreditorID(std::vector<int> creditorIDList);
		std::string GetFilterForDivisionAccount(int divisionAccountID, int disvisionID, int accountID, std::string accountCode);
		std::string GetFilterForDivisionEmployee(int divisionEmployeeID, int disvisionID, int employeeID, bool isContract);
		std::string GetFilterForDivision(int divisionID, std::string divisionName, std::string divisionCode);
		std::string GetFilterForEmployee(int employeeID, std::string name, std::string surname, std::string phone, 
			std::string address, std::string birthDate, std::string hireDate, std::string password, 
			std::string email, bool activated, int roleID, int positionID);
		std::string GetINFilterForEmployeeID(std::vector<int> employeeIDList);
		std::string GetINFilterForStockEmployeeID(std::vector<int> stockEmployeeIDList);
		std::string GetFilterForEmployeeProduct(int epID, int employeeID, int productID);
		std::string GetFilterForEntry(int entryID, std::string entryDate, int debbitingAccountID, double entryValue, int creditingAccountID, std::string description);
		std::string GetFilterForEntryForPeriod(int entryID, std::string entryDate, int debbitingAccountID, double entryValue, int creditingAccountID, std::string description,
			std::string fromDate, std::string toDate);
		std::string GetFilterForExtendedEntry(int entryID, std::string entryDate, int debbitingAccountID, double entryValue, int creditingAccountID, int operationID, int subaccountID, std::string description);
		std::string GetFilterForExtendedEntryForPeriod(int entryID, std::string entryDate, int debbitingAccountID, double entryValue, int creditingAccountID, int operationID, int subaccountID, std::string description,
			std::string fromDate, std::string toDate);
		std::string GetFilterForEntryRouting(int entryRoutingID, std::string entryRoutingOperation, int debbitingAccountID, int creditingAccountID);
		std::string GetFilterForEntryOperation(int eoID, int entryID, int operationID);
		std::string GetFilterForEntrySubaccount(int entrySubaccountID, int entryID, int subaccountID);
		std::string GetFilterForFinancialReport(int financialReportID, double account_44010, double account_55010, double account_552,
			double account_55270, double account_553, double account_55321, double account_44020_90, double account_66010_66110,
			double account_66020_66120, double account_66040_66140, double account_66050_66150, double account_66060_66160,
			double account_66130, double account_66070_66170, double tax, std::string fromDate, std::string tillDate);
		std::string GetFilterForFixedAssets(int fixedAssetsID, int fixedAssetsSpecID, std::string inventoryNumber, double primaryCost, double stopCost,
			int serviceLife, bool isAmortize, std::string buyDate, std::string startOfOperDate, std::string endOfOperDate, int statusID,
			int fixedAssetsDeID);
		std::string GetFilterForFixedAssetsSpecification(int fixedAssetsSpecID, std::string fixedAssetsName, std::string factoryNumner, std::string developer,
			std::string document, std::string objectCharacters, std::string condition, std::string dateOfConstruction);
		std::string GetFilterForFixedAssetsDetails(int fixedAssetsDeID, int amGroupID, int amTypeID, int departmentID, std::string fixedAssetsLocation,
			int primaryAccID, int amortizeAccID, std::string barcodeNumber);
		std::string GetFilterForFixedAssetsOperations(int fixedAssetsOperID, std::string operationDate, std::string operationName,
			double operationValue, bool increment, bool decrement, int fixedAssetsID);
		std::string GetFilterForInventorizationList(int inventorizationListID, int inventorizationID, int productID, double inventorizationListCount,
			double inventorizationListSum, int statusID, int currencyID);
		std::string GetFilterForInventorization(int inventorizationID, int userID, std::string inventorizationDate, std::string inventorizationExecutionDate,
			int stockEmployeeID, double inventorizationCount, double inventorizationSum, int statusID, int currencyID);
		std::string GetFilterForInventory(int inventoryID, std::string inventoryName, double inventoryCost, int departamentID, std::string location,
			int statusID, std::string startOfOperDate, std::string endOfOperDate, std::string inventoryNumber, std::string barcodeNumber);
		std::string GetFilterForInventoryHistory(int inventoryHistoryID, int inventoryID, std::string comment, std::string changeDate);
		std::string GetFilterForJobprice(int jobpriceID, int producrtID, double jobpriceValue, int currencyID, double jobpriceVolume, int measureID, int positionID);
		std::string GetFilterForJobsheet(int jobsheetID, std::string jobsheetDate, double jobsheetCount, int productID, int employeeID);
		std::string GetFilterForJobsheetForPeriod(int jobsheetID, std::string jobsheetDate, double jobsheetCount, int productID, int employeeID, std::string fromDate, std::string toDate);
		std::string GetFilterForLocation(int locationID, std::string countryName, std::string countryCode, std::string regionName
			, std::string cityName);
		std::string GetFilterForMeasure(int measureID, std::string measureName, std::string measureShortName, int measureUnit);
		std::string GetFilterForNetCost(int netCostID, std::string netCostDate, double netCostValue, int currencyID, int productID, bool netCostIsOutdated);
		std::string GetFilterForOrderList(int orderListID, int orderID, int productID, double orderListCount, double orderListSum, int statusID, int currencyID);
		std::string GetFilterForOrder(int orderID, int clientID, std::string orderDate, std::string orderExecutionDate, int employeeID,
			double orderCount, double orderSum, int statusID, int currencyID);
		std::string GetFilterForOrderForPeriod(int orderID, int clientID, std::string orderDate, std::string orderExecutionDate, int employeeID,
			double orderCount, double orderSum, int statusID, int currencyID, std::string fromDate, std::string toDate);
		std::string GetFilterForOrderRawList(int orderRawListID, int orderRawID, int productID, double orderRawListCount, double orderRawSum, int statusID, int currencyID);
		std::string GetFilterForOrderRaw(int orderRawID, int productID, std::string orderRawDate, std::string orderRawExecutionDate,
			int employeeID, double orderRawCount, double orderRawSum, int statusID, int currencyID);
		std::string GetFilterForOrderRawForPeriod(int orderRawID, int productID, std::string orderRawDate, std::string orderRawExecutionDate,
			int employeeID, double orderRawCount, double orderRawSum, int statusID, int currencyID, std::string fromDate, std::string toDate);
		std::string GetFilterForPayment(int paymentID, std::string paymentDate, double paymentValue, std::string paymentTarget, int userID,
			int currencyID, int statusID, int accountID, int subaccountID, std::string pWho, int cashboxAccID);
		std::string GetFilterForPaymentForPeriod(int paymentID, std::string paymentDate, double paymentValue, std::string paymentTarget, int userID,
			int currencyID, int statusID, int accountID, int subaccountID, std::string pWho, int cashboxAccID, std::string fromDate, std::string toDate);
		std::string GetFilterForPaymentEmployee(int paymentEmployeeID, int paymentID, int employeeID);
		std::string GetFilterForPayslip(int payslipID, std::string payslipDate, double payslipValue, int statusID, int currencyID);
		std::string GetFilterForPayslipOrder(int payslipOrderID, int payslipID, int orderID);
		std::string GetFilterForPercentRate(int percentRateID, double percentRateValue, std::string percentRateCondition, int positionID);
		std::string GetFilterForPhoto(int photoID, int userID, int productionID, std::string photoSource);
		std::string GetFilterForPosition(int positionID, std::string positionName);
		std::string GetFilterForPrice(int priceID, std::string priceDate, double priceValue, int currencyID, int productID, bool priceIsOutdated);
		std::string GetFilterForProductBranch(int pbID, int productID, int branchID);
		std::string GetFilterForProductType(int productTypeID, std::string productTypeName, std::string productTypeShortName, std::string productTypeCode);
		std::string GetFilterForProduction(int productionID, std::string productionDate, std::string productionExpiryDate, std::string productionSessionStart,
			std::string productionSessionEnd);
		std::string GetFilterForProductionForPeriod(int productionID, std::string productionDate, std::string productionExpiryDate, std::string productionSessionStart,
			std::string productionSessionEnd, std::string fromDate, std::string toDate);
		std::string GetFilterForProductionList(int productionListID, int productionID, int productID, double productionListCount, double productionListSum, int statusID, int currencyID);
		std::string GetFilterForProductionListInEnum(int productionListID, int productionID, int productID, double productionListCount, double productionListSum, int statusID, int currencyID, std::vector<int> vecProdID);
		std::string GetFilterForProductionConsumeRawList(int productionConsRawListID, int productionConsumeRawID, int productID, double productionConsRawListCount, double productionConsRawListSum, int statusID, int currencyID);
		std::string GetFilterForProductionConsumeRaw(int productionConsumeRawID, int userID, std::string productionConsumeRawDate,
			std::string productionConsumeRawExecutionDate, int stockEmployeeID, double productionConsumeRawCount,
			double productionConsumeRawSum, int statusID, int currencyID);
		std::string GetFilterForProductionConsumeRawForPeriod(int productionConsumeRawID, int userID, std::string productionConsumeRawDate,
			std::string productionConsumeRawExecutionDate, int stockEmployeeID, double productionConsumeRawCount,
			double productionConsumeRawSum, int statusID, int currencyID, std::string fromDate, std::string toDate);
		std::string GetFilterForProductionPlanList(int productionPlanListID, int productionPlanID, int productID, double productionPlanListCount, double productionPlanListSum, int statusID, int currencyID);
		std::string GetFilterForProductionPlan(int productionPlanID, std::string productionPlanDate, int employeeID, double productionPlanListCount, double productionPlanListSum, int statusID, int currencyID);
		std::string GetFilterForProductionStock(int productionStockID, int productID, double productionStockCount, double productionStockSum,
			int statusID, int currencyID, int warehouseID);
		std::string GetFilterForProduct(int productID, int companyID, std::string productName, double productVolume, int measureID, double productPrice, int productTypeID, int productShelfLife,
			int currencyID);
		std::string GetINFilterForProductID(std::vector<int> productIDList);
		std::string GetLikeFilterForProductName(std::string searchKey);
		std::string GetFilterForPurveyor(int purveyorID, std::string name, std::string surname, std::string phone,
			std::string address, std::string purveyorCompanyName, std::string password, std::string email,
			bool activated, int roleID, int locationID);
		std::string GetFilterForReceiptProductList(int receiptProductListID, int receiptProductID, int productID, double receiptProductCount, double receiptProductSum, int statusID, int currencyID);
		std::string GetFilterForReceiptProduct(int receiptProductID, int employeeID, std::string receiptProductDate,
			std::string receiptProductExecutionDate, int stockEmployeeID, double receiptProductCount, double receiptProductSum,
			int statusID, int currencyID);
		std::string GetFilterForReceiptProductForPeriod(int receiptProductID, int employeeID, std::string receiptProductDate,
			std::string receiptProductExecutionDate, int stockEmployeeID, double receiptProductCount, double receiptProductSum,
			int statusID, int currencyID, std::string fromDate, std::string toDate);
		std::string GetFilterForStockTransferList(int stockTransferListID, int stockTransferID, int productID, double stockTransferListCount, double stockTransferListSum, int statusID, int currencyID);
		std::string GetFilterForStockTransfer(int stockTransferID, int employeeID, std::string stockTransferDate, std::string stockTransferExecutionDate, int stockEmployeeID,
			double stockTransferCount, double stockTransferSum, int statusID, int currencyID);
		std::string GetFilterForStockTransferForPeriod(int stockTransferID, int employeeID, std::string stockTransferDate, std::string stockTransferExecutionDate, int stockEmployeeID,
			double stockTransferCount, double stockTransferSum, int statusID, int currencyID, std::string fromDate, std::string toDate);
		std::string GetFilterForRefund(int refundID, std::string refuntDate, double refundValue, int userID, int currencyID);
		std::string GetFilterForRelationType(int relationTypeID, std::string relationTypeName, std::string comment);
		std::string GetFilterForRelation(int relationID, int user1ID, int user2ID, int relationTypeID);
		std::string GetFilterForReturnList(int returnListID, int returnID, int productID, double returnListCount, double returnListSum, int statusID, int currencyID);
		std::string GetFilterForReturn(int returnID, int clientID, std::string returnDate, std::string returnExecutionDate, int employeeID, double returnCount, double returnSum, int statusID, int currencyID);
		std::string GetFilterForReturnForPeriod(int orderID, int clientID, std::string orderDate, std::string orderExecutionDate, int employeeID,
			double orderCount, double orderSum, int statusID, int currencyID, std::string fromDate, std::string toDate);
		std::string GetFilterForRole(int roleID, std::string roleCode, std::string roleName, std::string comment);
		std::string GetFilterForSalary(int salaryID, int userID, double salaryValue, int currencyID, int salaryTypeID, std::string salaryDate, bool salaryBonus);
		std::string GetFilterForSalaryType(int salaryTypeID, std::string salaryTypeCode, std::string salaryTypeName);
		std::string GetFilterForShareholder(int shareholderID, int userID, double countOfStock, std::string sInformation);
		std::string GetFilterForSpecificationList(int specificationListID, int specificationID, int productID, double specificationListCount);
		std::string GetFilterForSpecification(int specificationID, int productID, double specificationSum, int currencyID, int employeeID, std::string sDate);
		std::string GetFilterForSpoilageList(int spoilageListID, int spoilageID, int productID, double spoilageListCount, double spoilageListSum, int statusID, int currencyID);
		std::string GetFilterForSpoilage(int spoilageID, std::string spoilageDate, int employeeID, double spoilageCount, double spoilageSum, int statusID, int currencyID);
		std::string GetFilterForState(int stateID, int universalID, int statusID, std::string statusLastChange);
		std::string GetFilterForStatus(int statusID, std::string statusCode, std::string statusName, std::string comment);
		std::string GetFilterForStatusRule(int statusRuleID, std::string statusRuleOperation, int statusID);
		std::string GetFilterForStock(int stockID, int productID, double stockCount, double stockSum, int statusID, int currencyID, int warehouseID);
		std::string GetFilterForStockHistory(int stockID, int productID, double stockCount, double stockSum, int statusID, int currencyID,
			int warehouseID, std::string historyDate);
		std::string GetFilterForSubaccount(int subaccountID, int accountID, std::string subaccountNumber, double subaccountStartBalance, double subaccountCurrentBalance, int currencyID, int statusID,
			std::string subaccountOpenDate, std::string subaccountCloseDate, std::string subaccountDetails);
		std::string GetFilterForSubaccountHistory(int subaccountHistoryID, int subaccountID, double subaccountStartBalance, double subaccountCurrentBalance,
			std::string fromDate, std::string tillDate);
		std::string GetFilterForTax(int taxID, std::string taxName, std::string taxCode, double fixedValue, int percentValue,
			std::string formulaValue);
		std::string GetFilterForTimesheet(int timesheetID, int salaryID, double workedTime, std::string timesheetDate);
		std::string GetFilterForTimesheetForPeriod(int timesheetID, int salaryID, double workedTime, std::string timesheetDate, std::string fromDate, std::string toDate);
		std::string GetFilterForTransportList(int transportListID, int transportID, int productID, double transportListCount, double transportListSum, int statusID, int currencyID);
		std::string GetFilterForTransport(int transportID, int employeeID, std::string transportDate, std::string transportExecutionDate,
			int stockEmployeeID, double transportCount, double transportSum, int statusID, int currencyID);
		std::string GetFilterForUser(int userID, std::string userEmail, std::string userName, std::string userSurname, std::string userPhone, std::string userAddress,
			int roleID, std::string userPassword, bool userActivated);
		std::string GetINFilterForUserID(std::vector<int> userIDList);
		std::string GetFilterForWarehouse(int warehouseID, std::string warehouseName, std::string warehouseAddress, std::string warehousePhone,
			int warehouseTypeID, int subaccountID);
		std::string GetFilterForWarehouseType(int warehouseTypeID, std::string wTypeCode, std::string wTypePurpose, std::string wTypeName);
		std::string GetFilterForWarehouseEmployee(int weID, int weWarehouseID, int weEmployeeID);
		std::string GetFilterForWithdrawal(int withdrawalID, std::string withdrawalDate, double withdrawalValue, 
			int userID, int sabID, std::string withdrawalTarget, int currencyID, int statusID, int accountID, std::string pWho, int cashboxAccID);
		std::string GetFilterForWithdrawalForPeriod(int withdrawalID, std::string withdrawalDate, double withdrawalValue, int userID, int sabID, std::string withdrawalTarget,
			int currencyID, int statusID, int accountID, std::string pWho, int cashboxAccID, std::string fromDate, std::string toDate);
		std::string GetFilterForWriteOffList(int writeOffListID, int writeOffID, int productID, double writeOffListCount, double writeOffListSum, int statusID, int currencyID);
		std::string GetFilterForWriteOff(int writeOffID, int userID, std::string writeOffDate, std::string executionDate, int employeeID, double writeOffCount, double writeOffSum, int statusID, int currencyID);
		std::string GetFilterForWriteOffForPeriod(int writeOffID, int userID, std::string writeOffDate, std::string executionDate, int employeeID, double writeOffCount,
			double writeOffSum, int statusID, int currencyID, std::string fromDate, std::string toDate);
		std::string GetFilterForWriteOffRawList(int writeOffRawListID, int writeOffRawID, int productID, double writeOffRawListCount, double writeOffRawListSum, int statusID, int currencyID);
		std::string GetFilterForWriteOffRaw(int writeOffRawID, int employeeID, std::string writeOffRawDate, std::string executionDate, int stockEmployeeID, double writeOffRawListCount, double writeOffRawListSum,
			int statusID, int currencyID);
		std::string GetFilterForWriteOffRawForPeriod(int writeOffRawID, int employeeID, std::string writeOffRawDate, std::string executionDate, int stockEmployeeID, double writeOffRawListCount, double writeOffRawListSum, 
			int statusID, int currencyID, std::string fromDate, std::string toDate);

		//counting methods
		bool HaveChildAccount(std::string rootNumber);

		//overloaded  operators
		OrmasDal& operator=(const OrmasDal& ormasDal)
		{
			if (this == &ormasDal)
			{
				return *this;
			}
			else
			{
				dbConnection = ormasDal.dbConnection;
				connectionString = ormasDal.connectionString;
			}
			return *this;
		}
	private:
		PGconn *dbConnection;
		ConnectionString connectionString;
	};
}

#endif //ORMASDAL_H
