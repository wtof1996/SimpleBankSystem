#include "AdministratorView.hpp"
#include "cli.hpp"
#include "..\Helper.hpp"
#include <string>
#include <stdexcept>

#include <boost\filesystem.hpp>
#include <boost\format.hpp>
#include "..\Controller\IOController.hpp"

using std::string;

string GetPath()
{
	string input;
	do{
		input = CLI::GetInput("请输入路径");
	} while (!boost::filesystem::exists(boost::filesystem::path(input).branch_path()) && (CLI::ShowMsg("无效的路径，请重新输入"), true));

	return input;
}

namespace view
{
	void AdministratorView::Show()
	{
		CLI::ShowMenu(
			*this,
			{
				{ "更改汇率", [](AdministratorView& v) { v.ChangeForeignExchange(); } },
				{ "打印全部账户信息", [](AdministratorView& v) { v.PrintAccountList(); } },
				{ "打印全部账户交易记录", [](AdministratorView& v) { v.PrintAccountLog(); } },
				{ "新建/修改 柜员账号", [](AdministratorView& v) { v.ChangeTeller(); } },
				{ "新建/修改 管理员账号", [](AdministratorView& v) { v.ChangeAdmin(); } },
				{ "退出", [](AdministratorView& v) { v.LogOff(); } }
			},
			"管理员: " + User.Name
        );
	}

	void AdministratorView::ChangeForeignExchange()
	{
		auto FEList = Data->GetForeignExchangeList();
		std::vector<string> list;
		for (auto i : FEList) {
			list.emplace_back(i.second.Name + ": " + (boost::format("%.4lf") % i.second.Rate).str());
		}

		auto size = FEList.size();
		list.emplace_back("退出");

		auto choose = CLI::ShowChooseList("请选择需要更改的外汇：", list);
		if (choose == size + 1) return;

		double rate = std::stod(CLI::GetInput("请输入汇率(1外汇:人民币，小数四位):", boost::regex("^\\d*\\.\\d{4}")));

		decltype(choose) iter = 0;
		for (auto i : FEList) {
			if (++iter == choose) {
				model::ForeignExchange fe = i.second;
				fe.Rate = rate;
				Data->UpdateForeignExchangeRate(fe.Code, fe);

				BOOST_LOG_TRIVIAL(info) << "ForeignExchange " << fe.Code << " rate changed to:" << rate;

				break;
			}
		}

		CLI::ShowMsg("已成功更新外汇信息");
	}

	void AdministratorView::ChangeTeller()
	{
		string name, password, password2nd;

		name = CLI::GetInput("请输入柜员名称:");
		password = CLI::GetInput("请输入新密码, 只能由字母和数字组成，长度需不少于6位:", boost::regex("^\\w{6}\\w*"));
		password2nd = CLI::GetInput("请再输入一次以确认密码无误:");

		if (password != password2nd) {
			CLI::ShowMsg("两次密码不一致，操作已取消");
			return;
		}

		try {
			model::User& user = Data->GetUser(name);

			if (!user.isAdmin()){
				CLI::ShowMsg("已存在同名管理员，操作取消。");
				return;
			}
		}
		catch (std::invalid_argument& e)
		{
			string choose = CLI::GetInput("该柜员不存在，是否需要新增柜员？(Y/N)", boost::regex("^[yYnN]"));

			if (choose == "N" || choose == "n") return;
		}

		model::BankTeller teller(name, helper::SHA1(password));
		Data->UpdateUser(teller);

		CLI::ShowMsg("已成功更新柜员信息");

		BOOST_LOG_TRIVIAL(info) << "Bankteller: " << name << " information updated.";
	}

	void AdministratorView::PrintAccountList()
	{
		string path = GetPath();

		controller::io::PrintAccountList(*Data, path);

		CLI::ShowMsg("已成功打印账户列表");
	}

	void AdministratorView::PrintAccountLog()
	{
		string path = GetPath();

		controller::io::PrintAccountLog(*Data, path);

		CLI::ShowMsg("已成功打印账户交易记录");
	}

	void AdministratorView::ChangeAdmin()
	{
		string name, password, password2nd;

		name = CLI::GetInput("请输入管理员名称:");
		password = CLI::GetInput("请输入新密码, 只能由字母和数字组成，长度需大于6位:", boost::regex("^\\w{6}\\w*"));
		password2nd = CLI::GetInput("请再输入一次以确认密码无误:");

		if (password != password2nd) {
			CLI::ShowMsg("两次密码不一致，操作取消");
			return;
		}

		try {
			model::User& user = Data->GetUser(name);

			if (!user.isAdmin()){
				CLI::ShowMsg("已存在同名柜员，操作取消。");
				return;
			}
		}
		catch (std::invalid_argument& e)
		{
			string choose = CLI::GetInput("该管理员不存在，是否需要新增管理员？(Y/N)", boost::regex("^[yYnN]"));

			if (choose == "N" || choose == "n") return;
		}

		model::Administrator teller(name, helper::SHA1(password));
		Data->UpdateUser(teller);

		CLI::ShowMsg("已成功更新管理员信息");

		BOOST_LOG_TRIVIAL(info) << "Administrator: " << name << " information updated.";
	}
}
