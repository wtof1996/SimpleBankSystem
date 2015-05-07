#include "AccountManageView.hpp"
#include "cli.hpp"

namespace view
{
	void AccountManageView::Show()
	{
		CLI::ShowMenu(
				*this,
				{
					{ "���" , [](AccountManageView &v) { v.Deposit(); } },
					{ "ȡ��", [](AccountManageView &v) { v.Withdrawal(); } },
					{ "ת��", [](AccountManageView &v) { v.Transffer(); } },
					{ "����", [](AccountManageView &v) { v.CloseAccount(); } },
					{ "��������", [](AccountManageView &v) { v.CloseAccount(); } },
					{ "�˳�", [](AccountManageView &v) { v.Logoff(); } },
				},
				"�ͻ�: " + Controller->GetAccount().Name
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
			password = CLI::GetInput("������6λ������Ϊ������:", boost::regex("^\\d{6}"));
			password2nd = CLI::GetInput("��������һ����ȷ����������:");
		} while (password != password2nd && (CLI::ShowMsg("�������벻һ�£�������"), true));

		Controller->ChangePassword(password);

		CLI::ShowMsg("�����ɹ�");
	}

}