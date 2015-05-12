#pragma once
//#ifndef ACCOUNTCONTROLLER_HPP
//#define ACCOUNTCONTROLLER_HPP

#include "..\Model\BasicModel.hpp"

namespace controller
{
    class AccountController
    {
	private:
		std::vector<model::Record> Record;
		model::Account Account;
		double DemandRate;
    public:
        AccountController(model::Account& acc, double demand, decltype(Record) &rec)
			:Account(acc), DemandRate(demand), Record(rec)
		{
			UpdateAmount();
		}

		model::Account GetAccount() { return Account; }
		std::vector<model::Record> GetRecord() { return Record; }

		void UpdateAmount(); //计息 
		double UpdateCurrencyAccount(int index, double amount, bool save = true); //存取款
		void NewCurrencyAccount(model::CurrencyAccount &acc);  //开立新的子货币账户
		double DelCurrencyAccount(int index);  //销去新的子货币账户
		void ChangePassword(const std::string &pwd) { Account.PasswordHash = helper::SHA1(pwd); }

    };
}
//#endif // ACCOUNTCONTROLLER_HPP
