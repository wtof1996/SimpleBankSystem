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

	const string DepositListPath = "DepositList.Deposit";
	const string ForeignExchangeListPath = "ForeignExchangeList.ForeignExchange";
	const string BankTellerListPath = "User.BankTellers.BankTeller";
	const string AdministratorListPath = "User.Administrators.Administrator";
	const string TotalRecordPath = "Records.Record";
	const string AccountListPath = "AccountList.Account";
	class DataController
	{
	public:
		std::map < string, model::Administrator > AdministratorList; //����Ա�б�
		std::map < string, model::BankTeller > BankTellerList;		//��Ա�б�
		std::map < string, model::ForeignExchange > ForeignExchangeList; //�������б�
		std::map < string, model::Deposit > DepositList;				 //���ҵ���б�
		std::map < string, model::Account > AccountList;				 //�˻��б�
		std::map < string, std::vector<model::Record> > RecordByAccount; //���˻��洢�Ľ��׼�¼
		std::vector<model::Record> TotalRecord;							//�ܽ��׼�¼
	public:
		DataController() = default;
		~DataController() = default;



		bool VerifyUser(string name, string password);
		bool VerifyAccount(string number, string password);
		model::User& GetUser(string name);
		void UpdateUser(model::User &u);

		ptree GetForeignExchangePtree();
		ptree GetDepositListPtree();
		ptree GetAccountListPtree();
		ptree GetBankTellerListPtree();
		ptree GetTotalRecordPtree();

		const decltype(AccountList)& GetAccountList() { return AccountList; }
		const decltype(BankTellerList)& GetBankTellerList() { return BankTellerList; }
		const decltype(ForeignExchangeList)& GetForeignExchangeList() { return ForeignExchangeList; }
		const decltype(DepositList)& GetDepositList() { return DepositList; }
		const decltype(TotalRecord)& GetTotalRecord() { return TotalRecord; }



		std::vector<model::Record>& GetAccountRecord(const string& number);
		double GetForeignExchangeRate(const string& code);
		model::Deposit GetDeposit(const string& name);

		void SetForeignExchangeRateList(const ptree& XMLtree);
		void SetDepositList(const ptree& XMLtree);
		void SetTotalRecord(const ptree& XMLtree);
		void SetUserList(const ptree& adminTree, const ptree& tellerTree);
		void UpdateAccount(const model::Account& account, const std::vector<model::Record>& record);
		void UpdateForeignExchangeRate(const string& code, const model::ForeignExchange& fex);
	};

}




//#endif // DATACONTROLLER_HPP_INCLUDED
