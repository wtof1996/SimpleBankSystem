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

		void UpdateAmount(); //��Ϣ
		double UpdateCurrencyAccount(int index, double amount, bool save = true); //��ȡ��
		void NewCurrencyAccount(model::CurrencyAccount &acc);  //�����µ��ӻ����˻�
		double DelCurrencyAccount(int index);  //��ȥ�µ��ӻ����˻�

    };
}
//#endif // ACCOUNTCONTROLLER_HPP
