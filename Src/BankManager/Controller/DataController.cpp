#include "DataController.hpp"
#include "..\Helper.hpp"
#include <stdexcept>
#include <iostream>
using namespace helper;

namespace controller
{

	bool DataController::VerifyUser(const string &name, const string &password) const
	{
		if (AdministratorList.count(name) && AdministratorList.at(name).PasswordHash == SHA1(password)) return true;
		if (BankTellerList.count(name) && BankTellerList.at(name).PasswordHash == SHA1(password)) return true;

		return false;
	}

	bool DataController::VerifyAccount(const string &number, const string &password) const
	{
		if (AccountList.count(number) && AccountList.at(number).PasswordHash == SHA1(password)) return true;
		return false;
	}

	const model::User& DataController::GetUser(const string &name) const
	{
		if (AdministratorList.count(name)) return AdministratorList.at(name);
		if (BankTellerList.count(name)) return BankTellerList.at(name);

		throw std::invalid_argument("No such user:" + name);
	}

	void DataController::UpdateUser(model::User &u)
	{
		if (u.isAdmin()) AdministratorList[u.Name] = model::Administrator(u.Name, u.PasswordHash);
		else             BankTellerList[u.Name] = model::BankTeller(u.Name, u.PasswordHash);
	}

	ptree DataController::GetUserListPtree() const
	{
		ptree ret;
		
 		for (auto i : AdministratorList) {
			ret.add(AdministratorListPath, i.second.ToString());
		}
		if (AdministratorList.empty()) ret.add(AdministratorListPath, helper::NullString);

		for (auto i : BankTellerList) {
			ret.add(BankTellerListPath, i.second.ToString());
		}
		if (BankTellerList.empty()) ret.add(BankTellerListPath, helper::NullString);

		return ret;
	}

	ptree DataController::GetForeignExchangePtree() const
	{
		ptree ret;
		for (auto i : ForeignExchangeList){
			ret.add(ForeignExchangeListPath, i.second.ToString());
		}

		return ret;
	}

	ptree DataController::GetDepositListPtree() const
	{
		ptree ret;
		for (auto i : DepositList){
			ret.add(DepositListPath, i.second.ToString());
		}

		return ret;
	}

	ptree DataController::GetAccountListPtree() const
	{
		ptree ret;
		for (auto i : AccountList){
			ret.add(AccountListPath, AES_128_EncryptHex(i.second.ToString(), Config::get().AESKey, Config::get().AESIV));
			for (auto j : i.second.CurrencyAccountList) {
				ret.add(AccountListCAPath, AES_128_EncryptHex(j.ToString(), Config::get().AESKey, Config::get().AESIV));
			}
		}

		return ret;
	}

	ptree DataController::GetTotalRecordPtree() const
	{
		ptree ret;
		for (auto i : TotalRecord){
			ret.add(TotalRecordPath, AES_128_EncryptHex(i.ToString(), Config::get().AESKey, Config::get().AESIV));
		}

		return ret;
	}

	const model::Account& DataController::GetAccount(const string& number) const
	{
		if (AccountList.count(number)) return AccountList.at(number);

		throw std::invalid_argument("No Record for " + number);
	}

	const std::vector<model::Record>& DataController::GetAccountRecord(const string& number) const
	{
		if (AccountList.count(number)) return RecordByAccount.at(number);

		throw std::invalid_argument("No Record for " + number);
	}

	double DataController::GetForeignExchangeRate(const string& code) const
	{
		if (ForeignExchangeList.count(code)) return ForeignExchangeList.at(code).Rate;
		return NAN;
	}

	const model::Deposit DataController::GetDeposit(const string& name) const
	{
		if (DepositList.count(name)) return DepositList.at(name);

		throw std::invalid_argument("No such deposit:" + name);
	}

	void DataController::SetForeignExchangeRateList(const ptree& XMLtree)
	{
		ForeignExchangeList.clear();
		auto child = XMLtree.get_child(ForeignExchangeListRoot);
		for (auto &i : child) {
			model::ForeignExchange tmp;
			tmp.FromString(i.second.get_value<string>());
			ForeignExchangeList[tmp.Code] = tmp;
		}
	}

	void DataController::SetDepositList(const ptree& XMLtree)
	{
		DepositList.clear();
		auto child = XMLtree.get_child(DepositListRoot);
		for (auto &i : child) {
			model::Deposit tmp;
			tmp.FromString(i.second.get_value<string>());
			DepositList[tmp.Name] = tmp;
		}
	}

	void DataController::SetTotalRecord(const ptree& XMLtree)
	{
		TotalRecord.clear();
		auto child = XMLtree.get_child(TotalRecordRoot);
		for (auto &i : child) {
			model::Record tmp;
			tmp.FromString(AES_128_DecryptHex(i.second.get_value<string>(), Config::get().AESKey, Config::get().AESIV));
			TotalRecord.push_back(tmp);
		}

	}

	void DataController::SetUserList(const ptree& XMLtree)
	{
		AdministratorList.clear();
		auto child = XMLtree.get_child(AdministratorListRoot);
		for (auto &i : child) {
			model::Administrator tmp;
			tmp.FromString(i.second.get_value<string>());
			AdministratorList[tmp.Name] = tmp;
		}

		BankTellerList.clear();
		child = XMLtree.get_child(BankTellerListRoot);
		for (auto &i : child) {
			model::BankTeller tmp;
			tmp.FromString(i.second.get_value<string>());
			BankTellerList[tmp.Name] = tmp;
		}
	}

	void DataController::SetAccountList(const ptree& XMLtree)
	{
		auto child = XMLtree.get_child(AccountListRoot);
		for (auto &i : child) {
			model::Account tmp;
			tmp.FromString(i.second.get_value<string>());
			AccountList[tmp.Number] = tmp;
			auto &child_list = i.second;
			auto &acc = AccountList[tmp.Number];
			for (auto &j : child_list) {
				model::CurrencyAccount tmp;
				tmp.FromString(j.second.get_value<string>());
				acc.CurrencyAccountList.push_back(tmp);
			}
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