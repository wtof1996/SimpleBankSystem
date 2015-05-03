#include "MainView.hpp"
#include <string>

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

	}
}
