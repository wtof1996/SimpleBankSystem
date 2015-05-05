#include "BankTellerView.hpp"
#include "cli.hpp"
namespace view
{
	void BankTellerView::Show()
	{
		CLI::ShowMenu(
			*this,
			{
			    { "账户管理", [](BankTellerView &v) {v.LogOff(); } },
				{ "转账", [](BankTellerView &v) {v.LogOff(); } },
				{ "外汇兑换", [](BankTellerView &v) {v.LogOff(); } },
				{ "退出", [](BankTellerView &v) {v.LogOff(); } }
            },
			"柜员： " + User.Name
        );
	}

	void BankTellerView::Transffer()
	{

	}

	void BankTellerView::ForeignExchange()
	{

	}

	void BankTellerView::AccountManage()
	{

	}
}
