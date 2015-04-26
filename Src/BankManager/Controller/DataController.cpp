#include "DataController.hpp"
#include "..\Helper.hpp"
using namespace helper;

namespace controller
{

	bool DataController::VerifyUser(string name, string password)
	{
		if (AdministratorList.count(name) && AdministratorList[name].PasswordHash == SHA1(password)) return true;
		if (BankTellerList.count(name) && BankTellerList[name].PasswordHash == SHA1(password)) return true;

		return false;
	}

	bool DataController::VerifyAccount(string number, string password)
	{
		if (AccountList.count(number) && AccountList[number].PasswordHash == SHA1(password)) return true;
		return false;
	}

	model::User& DataController::GetUser(string name)
	{
		if (AdministratorList.count(name)) return AdministratorList[name];
		return BankTellerList[name];
	}

	void DataController::UpdateUser(model::User &u)
	{
		if (u.isAdmin()) AdministratorList[u.Name] = model::Administrator(u.Name, u.PasswordHash);
		else             BankTellerList[u.Name] = model::BankTeller(u.Name, u.PasswordHash);
	}

	ptree DataController::GetForeignExchangePtree()
	{
		ptree ret;
		for (auto &i : ForeignExchangeList){
			ret.add(ForeignExchangeListPath, i.second.ToString());
		}

		return ret;
	}

	ptree DataController::GetDepositListPtree()
	{
		ptree ret;
		for (auto &i : BankTellerList){
			ret.add(BankTellerListPath, i.second.ToString());
		}

		return ret;
	}

	ptree DataController::GetAccountListPtree()
	{
		ptree ret;
		for (auto &i : AccountList){
			ret.add(AccountListPath, AES_128_EncryptHex(i.second.ToString(), Config::get().AESKey, Config::get().AESIV));
			for (auto &j : i.second.CurrencyAccountList) {
				ret.add(AccountListPath + ".CurrencyAccount", AES_128_EncryptHex(j.ToString(), Config::get().AESKey, Config::get().AESIV));
			}
		}

		return ret;
	}

	ptree DataController::GetBankTellerListPtree()
	{
		ptree ret;
		for (auto &i : BankTellerList){
			ret.add(BankTellerListPath, i.second.ToString());
		}

		return ret;
	}

	ptree DataController::GetTotalRecordPtree()
	{
		ptree ret;
		for (auto &i : TotalRecord){
			ret.add(TotalRecordPath, AES_128_EncryptHex(i.ToString(), Config::get().AESKey, Config::get().AESIV));
		}

		return ret;
	}

	std::vector<model::Record>& DataController::GetAccountRecord(const string& number)
	{
		if (AccountList.count(number)) return RecordByAccount[number];
	}

	double DataController::GetForeignExchangeRate(const string& code)
	{
		if (ForeignExchangeList.count(code)) return ForeignExchangeList[code].Rate;
		return NAN;
	}

	model::Deposit DataController::GetDeposit(const string& name)
	{
		if (DepositList.count(name)) return DepositList[name];
	}

	void DataController::SetForeignExchangeRateList(const ptree& XMLtree)
	{
		ForeignExchangeList.clear();
		auto child = XMLtree.get_child("ForeignExchangeList");
		for (auto &i : child) {
			model::ForeignExchange tmp;
			tmp.FromString(i.second.get_value<string>());
			ForeignExchangeList[tmp.Code] = tmp;
		}
	}

	void DataController::SetDepositList(const ptree& XMLtree)
	{
		DepositList.clear();
		auto child = XMLtree.get_child("DepositList");
		for (auto &i : child) {
			model::Deposit tmp;
			tmp.FromString(i.second.get_value<string>());
			DepositList[tmp.Name] = tmp;
		}
	}

	void DataController::SetTotalRecord(const ptree& XMLtree)
	{
		TotalRecord.clear();
		auto child = XMLtree.get_child("Records");
		for (auto &i : child) {
			model::Record tmp;
			tmp.FromString(AES_128_DecryptHex(i.second.get_value<string>(), Config::get().AESKey, Config::get().AESIV));
			TotalRecord.push_back(tmp);
		}

	}

	void DataController::SetUserList(const ptree& adminTree, const ptree& tellerTree)
	{
		auto child = adminTree.get_child("User.Administrators");
		for (auto &i : child) {
			model::Administrator tmp;
			tmp.FromString(i.second.get_value<string>());
			AdministratorList[tmp.Name] = tmp;
		}

		child = tellerTree.get_child("User.BankTellers");
		for (auto &i : child) {
			model::BankTeller tmp;
			tmp.FromString(i.second.get_value<string>());
			BankTellerList[tmp.Name] = tmp;
		}
	}

	void DataController::UpdateAccount(const model::Account& account, const std::vector<model::Record>& record)
	{
		AccountList[account.Number] = account;
		RecordByAccount[account.Number] = record;
	}

	void DataController::UpdateForeignExchangeRate(const string& code, const model::ForeignExchange& fex)
	{
		ForeignExchangeList[code] = fex;
	}
}
