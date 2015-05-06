#pragma once

//#ifndef DATACONTROLLER_HPP_INCLUDED
//#define DATACONTROLLER_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>
#include "..\Model\BasicModel.hpp"
#include "..\Model\Config.hpp"
#include "..\Model\UserModel.hpp"
#include <boost\property_tree\ptree.hpp>

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
	//XML路径
	class DataController
	{
	private:
		std::map < string, model::Administrator > AdministratorList; //管理员列表
		std::map < string, model::BankTeller > BankTellerList;		//柜员列表
		std::map < string, model::ForeignExchange > ForeignExchangeList; //外汇汇率列表
		std::map < string, model::Deposit > DepositList;				 //存款业务列表
		std::map < string, model::Account > AccountList;				 //账户列表
		std::map < string, std::vector<model::Record> > RecordByAccount; //按卡号存储的交易记录
		std::vector<model::Record> TotalRecord;							//总交易记录
	public:
		DataController() = default;
		~DataController() = default;

		bool VerifyUser(const string& name, const string& password) const; //用户校验
		bool VerifyAccount(const string& number, const string& password) const; //账户校验
	    model::User& GetUser(const string& name); //根据用户名获取一个用户
		void UpdateUser(model::User &u);				//更新用户信息

		//P.S:整个系统中，用户指柜员和管理员，账户指银行账户，即客户

		ptree GetForeignExchangePtree() const;
		ptree GetDepositListPtree() const;
		ptree GetAccountListPtree() const;
		ptree GetTotalRecordPtree() const;
		ptree GetUserListPtree() const; //获取各个列表的ptree，便于写XML

		const decltype(AccountList)& GetAccountList() const { return AccountList; }
		const decltype(BankTellerList)& GetBankTellerList() const { return BankTellerList; }
		const decltype(ForeignExchangeList)& GetForeignExchangeList() const { return ForeignExchangeList; }
		const decltype(DepositList)& GetDepositList() const { return DepositList; }
		const decltype(TotalRecord)& GetTotalRecord() const { return TotalRecord; }
		//获取各个列表，便于显示和打印

		const std::vector<model::Record>& GetAccountRecord(const string& number) const;//根据卡号获取交易记录
		const model::Account& GetAccount(const string& number) const;//根据卡号获取用户信息
		double GetForeignExchangeRate(const string& code) const;//根据货币代码获取汇率
		const model::Deposit GetDeposit(const string& name) const;//根据存款业务名称获取相应存款业务信息

		void SetForeignExchangeRateList(const ptree& XMLtree);
		void SetDepositList(const ptree& XMLtree);
		void SetTotalRecord(const ptree& XMLtree);
		void SetUserList(const ptree& XMLtree);
		void SetAccountList(const ptree& XMLtree);
		//根据XML中的ptree设置各项数据

		void AddRecord(const model::Record& record, bool addAccount = false);
		//增加单条记录
		void UpdateAccount(const model::Account& account, const std::vector<model::Record>& record);
		//更新账户信息
		void UpdateForeignExchangeRate(const string& code, const model::ForeignExchange& fex);
		//更新汇率信息
	};

}

//#endif // DATACONTROLLER_HPP_INCLUDED
