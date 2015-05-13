#pragma once

//#ifndef ADMINISTRATORVIEW_HPP
//#define ADMINISTRATORVIEW_HPP

#include <boost\log\trivial.hpp>

#include "..\Model\UserModel.hpp"

#include "..\Controller\DataController.hpp"
#include "cli.hpp"
#include "BaseView.hpp"

namespace view
{
	class AdministratorView : public BaseView
	{
	public:

		model::Administrator User;
		controller::DataController* Data;

		AdministratorView(model::User user, controller::DataController *d)
			:User(user), Data(d)
		{
			BOOST_LOG_TRIVIAL(info) << "Administrator " << User.Name << " logged in.";
		}

		~AdministratorView()
		{
			CLI::CleanCLI();
			BOOST_LOG_TRIVIAL(info) << "Administrator " << User.Name << " logged off.";
		}

		void Show() override;

		void LogOff() { Loop = false; }
		void ChangeForeignExchange();
		void ChangeTeller();
		void PrintAccountList();
		void PrintAccountLog();
		void ChangeAdmin();


	};

}

//#endif // ADMINISTRATORVIEW_HPP
