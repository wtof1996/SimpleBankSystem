#pragma once

//#ifndef ACCOUNTMANAGEVIEW_HPP
//#define ACCOUNTMANAGEVIEW_HPP

#include "BaseView.hpp"
#include "..\Controller\AccountController.hpp"
#include "..\Model\BasicModel.hpp"
#include "..\Controller\DataController.hpp"
#include <boost\log\trivial.hpp>

namespace view
{
	class AccountManageView : public BaseView
	{
	private:
		controller::AccountController* Controller;
		controller::DataController* Data;
	public:
		AccountManageView(controller::DataController* data, controller::AccountController* control)
			:Data(data), Controller(control)
		{
			BOOST_LOG_TRIVIAL(info) << "Account " << Controller->GetAccount().Name << " loged in";
		}

		~AccountManageView()
		{
			BOOST_LOG_TRIVIAL(info) << "Account " << Controller->GetAccount().Name << " loged off";
		}

		void Show() override;
		void Deposit();
		void Withdrawal();
		void Transffer();
		void NewCurrencyAccount();
		void CloseAccount();
		void ChangePassword();
		void Logoff() { Loop = false; }
	};
}


//#endif // ACCOUNTMANAGEVIEW_HPP
