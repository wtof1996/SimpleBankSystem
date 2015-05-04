#pragma once

//#ifndef BANKTELLERVIEW_HPP
//#define BANKTELLERVIEW_HPP

#include "BaseView.hpp"
#include "..\Model\UserModel.hpp"
#include <boost\log\trivial.hpp>

namespace view
{
	class BankTellerView : public BaseView
	{
	public:
		model::BankTeller User;

		BankTellerView(model::User &user)
			:User(user)
		{
			BOOST_LOG_TRIVIAL(info) << "BankTeller " << User.Name << " logged in.";
		}

		~BankTellerView()
		{
			BOOST_LOG_TRIVIAL(info) << "BankTeller " << User.Name << " logged off.";
		}

		void Show() override;
		void LogOff() { Loop = false; }


	};

}

//#endif // BANKTELLERVIEW_HPP
