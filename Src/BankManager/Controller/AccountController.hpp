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

		void UpdateAmount(); //��Ϣ 
		double UpdateCurrencyAccount(int index, double amount, bool save = true); //��ȡ��
		void NewCurrencyAccount(model::CurrencyAccount &acc);  //�����µ��ӻ����˻�
		double DelCurrencyAccount(int index);  //��ȥ�µ��ӻ����˻�
		void ChangePassword(const std::string &pwd) { Account.PasswordHash = helper::SHA1(pwd); }

    };
}
//#endif // ACCOUNTCONTROLLER_HPP
