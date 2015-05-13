#pragma once
//#ifndef ACCOUNTCONTROLLER_HPP
//#define ACCOUNTCONTROLLER_HPP

#include "..\Model\BasicModel.hpp"

namespace controller
{
    class AccountController
    {
	private:
		model::Account Account;
		double DemandRate;
		std::vector<model::Record> Record;
    public:
		bool Closed = false;

        AccountController(model::Account& acc, double demand, decltype(Record) &rec)
			:Account(acc), DemandRate(demand), Record(rec)
		{
			UpdateAmount();
		}

		model::Account GetAccount() { return Account; }
		std::vector<model::Record> GetRecord() { return Record; }

		void UpdateAmount(); //计息
		double UpdateCurrencyAccount(size_t index, double amount, bool save = true); //存取款
		void NewCurrencyAccount(const model::CurrencyAccount &acc);  //开立新的子货币账户
		double DelCurrencyAccount(size_t index);  //销去新的子货币账户
		void ChangePassword(const std::string &pwd) { Account.PasswordHash = helper::SHA1(pwd); }
		void AddRecord(const model::Record &r) { Record.push_back(r); }
		void Transfer(std::string target, double amount); // 转账

    };
}
//#endif // ACCOUNTCONTROLLER_HPP
