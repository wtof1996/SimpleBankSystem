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
    public:
        AccountController(model::Account& acc)
			:Account(acc)
		{
			UpdateAmount();
		}

		model::Account GetAccount() { return Account; }

		void UpdateAmount();
		void UpdateCurrencyAccount(int index, model::CurrencyAccount &acc);
		void NewCurrencyAccount(model::CurrencyAccount &acc);
		void DelCurrencyAccount(int index);

    };
}
//#endif // ACCOUNTCONTROLLER_HPP
