#include "MainView.hpp"
#include "BankTellerView.hpp"
#include "AdministratorView.hpp"
#include <string>

using std::string;

namespace view
{
	void MainView::Show()
	{

		CLI::ShowMenu(
			*this,
			{
				{ "登陆", [](MainView& d){d.Login(); } },
				{ "退出", [](MainView& d){d.Exit(); } }
			},
			"主菜单"
		);
	}

	void MainView::Login()
	{
		string name, password;

		name = CLI::GetInput("用户名:");
		password = CLI::GetInput("密码:");

		if (!Data.VerifyUser(name, password)) {
			CLI::ShowMsg("用户名或密码错误!");
			BOOST_LOG_TRIVIAL(warning) << "Log in failed, User Name:" << name;
			return;
		}

		model::User& user = Data.GetUser(name);

		if (user.isAdmin()) {
			AdministratorView Admin(user, &Data);
			while (Admin.Loop) Admin.Show();
		}
		else {
			BankTellerView Teller(user);
			while (Teller.Loop) Teller.Show();
		}
	}
}
