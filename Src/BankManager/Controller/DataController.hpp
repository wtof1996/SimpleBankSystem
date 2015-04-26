#pragma once

#include <map>
#include <string>
#include <vector>
#include "..\Model\BasicModel.hpp"
#include "..\Model\Config.hpp"
#include "..\Model\UserModel.hpp"
#include <boost\property_tree\ptree.hpp>

//#ifndef DATACONTROLLER_HPP_INCLUDED
//#define DATACONTROLLER_HPP_INCLUDED

namespace controller
{
	using std::string;
	using boost::property_tree::ptree;

	const string DepositListRoot = "DepositList"
		, DepositListPath = DepositListRoot + ".Deposit"
		, ForeignExchangeListRoot = "ForeignExchangeList"
		, ForeignExchangeListPath = ForeignExchangeListRoot + ".ForeignExchange"
		, BankTellerListRoot = "BankTellers"
		, BankTellerListPath = BankTellerListRoot + ".BankTeller"
		, AdministratorListRoot = "Administrators"
		, AdministratorListPath = AdministratorListRoot + ".Administrator"
		, TotalRecordRoot = "Records"
		, TotalRecordPath = TotalRecordRoot + ".Record"
		, AccountListRoot = "AccountList"
		, AccountListPath = AccountListRoot + ".Account"
		, AccountListCAPath = AccountListPath + ".CurrencyAccount";

	class DataController
	{
	private:
		std::map < string, model::Administrator > AdministratorList; //管理员列表
		std::map < string, model::BankTeller > BankTellerList;		//柜员列表
		std::map < string, model::ForeignExchange > ForeignExchangeList; //外汇汇率列表
		std::map < string, model::Deposit > DepositList;				 //存款业务列表
		std::map < string, model::Account > AccountList;				 //账户列表
		std::map < string, std::vector<model::Record> > RecordByAccount; //按账户存储的交易记录
		std::vector<model::Record> TotalRecord;							//总交易记录
	public:
		DataController() = default;
		~DataController() = default;

		bool VerifyUser(const string& name, const string& password) const;
		bool VerifyAccount(const string& number, const string& password) const;
		const model::User& GetUser(const string& name) const;
		void UpdateUser(model::User &u);

		ptree GetForeignExchangePtree() const;
		ptree GetDepositListPtree() const;
		ptree GetAccountListPtree() const;
		ptree GetBankTellerListPtree() const;
		ptree GetTotalRecordPtree() const;

		const decltype(AccountList)& GetAccountList() const { return AccountList; }
		const decltype(BankTellerList)& GetBankTellerList() const { return BankTellerList; }
		const decltype(ForeignExchangeList)& GetForeignExchangeList() const { return ForeignExchangeList; }
		const decltype(DepositList)& GetDepositList() const { return DepositList; }
		const decltype(TotalRecord)& GetTotalRecord() const { return TotalRecord; }

		const std::vector<model::Record>& GetAccountRecord(const string& number) const;
		const model::Account& GetAccount(const string& number) const;
		double GetForeignExchangeRate(const string& code) const;
		const model::Deposit GetDeposit(const string& name) const;

		void SetForeignExchangeRateList(const ptree& XMLtree);
		void SetDepositList(const ptree& XMLtree);
		void SetTotalRecord(const ptree& XMLtree);
		void SetUserList(const ptree& XMLtree);
		void SetAccountList(const ptree& XMLtree);
		void UpdateAccount(const model::Account& account, const std::vector<model::Record>& record);
		void UpdateForeignExchangeRate(const string& code, const model::ForeignExchange& fex);
	};

}




//#endif // DATACONTROLLER_HPP_INCLUDED
