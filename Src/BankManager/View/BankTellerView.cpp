#include "BankTellerView.hpp"
#include "cli.hpp"
#include <string>
#include <boost\format.hpp>

#include "..\Controller\IOController.hpp"

using std::string;

namespace view
{
	void BankTellerView::Show()
	{
		CLI::ShowMenu(
			*this,
			{
				{ "开户", [](BankTellerView &v) {v.NewAccount(); } },
			    { "账户管理", [](BankTellerView &v) {v.AccountManage(); } },
				{ "电汇", [](BankTellerView &v) {v.Transffer(); } },
				{ "外汇兑换", [](BankTellerView &v) {v.ForeignExchange(); } },
				{ "退出", [](BankTellerView &v) {v.LogOff(); } }
            },
			"柜员： " + User.Name
        );
	}

	void BankTellerView::Transffer()
	{
		string name = CLI::GetInput("请输入汇款人姓名:");
		string number = CLI::GetInput("请输入卡号:", boost::regex("^\\d{8}"));
		string amount = CLI::GetInput("请输入金额(定点两位小数):", boost::regex("^\\d*.\\d{2}"));

		CLI::ShowBoxMsg("即将对 " + number + " 账户转入 " + amount + " 元");
		string choose = CLI::GetInput("是否继续？(Y/N)", boost::regex("^[yYnN]"));

		if (choose == "N" || choose == "n") return;

		model::Account acc;

		try {
			acc = Data->GetAccount(number);
		}
		catch (std::invalid_argument &e) {
			CLI::ShowMsg("操作失败，查无此户");
		}

		auto accRecord = Data->GetAccountRecord(number);

		acc.CurrencyAccountList[0].Currency.Amount += std::stod(amount);

		model::Record rec(number, name, "电汇", model::Currency(model::MainCurrency.Name, model::MainCurrency.Code, std::stod(amount)), "");
		accRecord.push_back(rec);

		Data->AddRecord(rec);

		Data->UpdateAccount(acc, accRecord);

		CLI::ShowMsg("操作已成功完成");

		controller::io::WriteAccountData(*Data);
		controller::io::WriteAccountLog(*Data);

	}

	void BankTellerView::ForeignExchange()
	{
		auto FEList = Data->GetForeignExchangeList();
		std::vector<string> list;
		for (auto i : FEList) {
			list.emplace_back(i.second.Name + ": " + (boost::format("%.4lf") % i.second.Rate).str());
		}

		auto size = FEList.size();
		list.emplace_back("退出");

		auto choose = CLI::ShowChooseList("请选择需要兑换的外汇：", list);
		if (choose == size + 1) return;

		model::ForeignExchange FE;
		decltype(choose) iter = 0;
		for (auto i : FEList) {
			if (++iter == choose) {
				FE = i.second;
				break;
			}
		}

		choose = CLI::ShowChooseList("请选择兑换方向:", { "人民币兑外汇", "外汇兑人民币", "退出" });

		if (choose == 3) return;

		string name = CLI::GetInput("请输入客户姓名:");
		string desc;
		double amount = std::stod(CLI::GetInput("请输入金额(定点两位小数):", boost::regex("^\\d*.\\d{2}")));
		double res;
		if (choose == 1) {
			res = amount / FE.Rate;
			desc = "人民币兑外汇, 给付金额: " + (boost::format("%.2lf") % (res)).str();
		}
		else if (choose == 2) {
			res = amount * FE.Rate;
			desc = "外汇兑人民币, 给付金额: " + (boost::format("%.2lf") % (res)).str();
		}

		CLI::ShowMsg(desc);

		model::Record rec("", name, "汇兑", model::Currency(FE.Name, FE.Code, amount), desc);

		Data->AddRecord(rec, true);

		controller::io::WriteAccountLog(*Data);
	}

	void BankTellerView::NewAccount()
	{
		string name = CLI::GetInput("请输入户名:"),
			   number = CLI::GetInput("请输入卡号:", boost::regex("^\\d{8}")),
			   amount = CLI::GetInput("请输入开户金额(定点两位小数):", boost::regex("^\\d*.\\d{2}"));

		CLI::ShowBoxMsg("即将开设 " + number + " 账户" + "户名: " + name + " 金额: " + amount + " 元");
		string choose = CLI::GetInput("是否继续？(Y/N)", boost::regex("^[yYnN]"));

		if (choose == "N" || choose == "n") return;

		string password,password2nd;

		do {
			password = CLI::GetInput("请输入6位数字作为新密码:", boost::regex("^\\d{6}"));
			password2nd	= CLI::GetInput("请再输入一次以确认密码无误:");
		} while (password != password2nd && (CLI::ShowMsg("两次密码不一致，请重试"), true));

		model::Account acc(number, name, helper::SHA1(password));
		model::CurrencyAccount ca;
		ca.Currency = model::MainCurrency;
		ca.Currency.Amount = std::stod(amount);
		ca.DespoitType = Data->GetDeposit("活期");
		ca.LastUpdateDate = boost::gregorian::day_clock::local_day();
		ca.Period = decltype(ca.Period)(ca.LastUpdateDate, ca.LastUpdateDate);

		acc.CurrencyAccountList.push_back(ca);

		model::Record rec(number, name, "开户", ca.Currency, "");
		std::vector<model::Record> accRec;
		accRec.push_back(rec);

		Data->UpdateAccount(acc, accRec);
		Data->AddRecord(rec);
		CLI::ShowMsg("操作已成功完成");

		controller::io::WriteAccountData(*Data);
		controller::io::WriteAccountLog(*Data);

	}

	void BankTellerView::AccountManage()
	{

	}
}
