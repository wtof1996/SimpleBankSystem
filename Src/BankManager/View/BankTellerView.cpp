#include "BankTellerView.hpp"
#include "cli.hpp"
namespace view
{
	void BankTellerView::Show()
	{
		CLI::ShowMenu(
			*this,
			{
			    { "退出", [](BankTellerView &v) {v.LogOff(); } }
            },
			"柜员菜单"
        );
	}
}
