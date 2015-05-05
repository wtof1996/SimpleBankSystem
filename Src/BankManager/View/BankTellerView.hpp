#pragma once

//#ifndef BANKTELLERVIEW_HPP
//#define BANKTELLERVIEW_HPP

#include "BaseView.hpp"
#include "..\Model\UserModel.hpp"
#include "..\Controller\DataController.hpp"
#include <boost\log\trivial.hpp>

namespace view
{
	class BankTellerView : public BaseView
	{
	public:
		model::BankTeller User;
		controller::DataController* Data;

		BankTellerView(model::User &user, controller::DataController *d)
			:User(user), Data(d)
		{
			BOOST_LOG_TRIVIAL(info) << "BankTeller " << User.Name << " logged in.";
		}

		~BankTellerView()
		{
			BOOST_LOG_TRIVIAL(info) << "BankTeller " << User.Name << " logged off.";
		}

		void Show() override;
		void LogOff() { Loop = false; }

		void Transffer();
		void ForeignExchange();
		void AccountManage();


	};

}

//#endif // BANKTELLERVIEW_HPP
