#include "AccountManageView.hpp"
#include "cli.hpp"

namespace view
{
	void AccountManageView::Show()
	{
		CLI::ShowMenu(
				*this,
				{
					{ "存款" , [](AccountManageView &v) { v.Deposit(); } },
					{ "取款", [](AccountManageView &v) { v.Withdrawal(); } },
					{ "转账", [](AccountManageView &v) { v.Transffer(); } },
					{ "销户", [](AccountManageView &v) { v.CloseAccount(); } },
					{ "更改密码", [](AccountManageView &v) { v.CloseAccount(); } },
					{ "退出", [](AccountManageView &v) { v.Logoff(); } },
				},
				"客户: " + Controller->GetAccount().Name
		);
	}

	void AccountManageView::Deposit()
	{

	}

	void AccountManageView::Withdrawal()
	{

	}

	void AccountManageView::Transffer()
	{

	}

	void AccountManageView::CloseAccount()
	{

	}

	void AccountManageView::ChangePassword()
	{
		std::string password, password2nd;

		do {
			password = CLI::GetInput("请输入6位数字作为新密码:", boost::regex("^\\d{6}"));
			password2nd = CLI::GetInput("请再输入一次以确认密码无误:");
		} while (password != password2nd && (CLI::ShowMsg("两次密码不一致，请重试"), true));

		Controller->ChangePassword(password);

		CLI::ShowMsg("操作成功");
	}

}