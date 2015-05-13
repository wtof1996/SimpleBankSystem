#include <boost\format.hpp>

#include "..\Controller\IOController.hpp"

#include "cli.hpp"
#include "AccountManageView.hpp"

using std::string;
using boost::regex;
using boost::format;
using std::stod;
using std::to_string;

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
					{ "开设新子账户", [](AccountManageView &v) { v.NewCurrencyAccount(); } },
					{ "销户", [](AccountManageView &v) { v.CloseAccount(); } },
					{ "更改密码", [](AccountManageView &v) { v.ChangePassword(); } },
					{ "退出", [](AccountManageView &v) { v.Logoff(); } },
				},
				"客户: " + Controller->GetAccount().Name
		);
	}

	void AccountManageView::Deposit()
	{
		auto acc = Controller->GetAccount();
		std::vector<string> list;

		for (auto i : acc.CurrencyAccountList) {
			list.emplace_back(i.Currency.Name + "账户, 余额:" + (format("%.2lf") % i.Currency.Amount).str() + " 存期:" + i.DespoitType.Name);
		}

		list.emplace_back("退出");
		auto choose = CLI::ShowChooseList("请选择需要存入的账户：", list);
		if (choose == list.size()) return;

		auto &CA = acc.CurrencyAccountList[choose - 1];

		string amount = CLI::GetInput("请输入金额(定点两位小数):", regex("^\\d*.\\d{2}"));

		CLI::ShowBoxMsg("即将对 " + CA.Currency.Name + " 账户存入 " + amount + " 元");
		string res = CLI::GetInput("是否继续？(Y/N)", regex("^[yYnN]"));

		if (res == "N" || res == "n") return;

		Controller->UpdateCurrencyAccount(choose - 1, stod(amount));

		Data->AddRecord(Controller->GetRecord().back());

		CLI::ShowMsg("操作成功完成");

		controller::io::WriteAccountData(*Data);
		controller::io::WriteAccountLog(*Data);

	}

	void AccountManageView::Withdrawal()
	{
		auto acc = Controller->GetAccount();
		std::vector<string> list;

		for (auto i : acc.CurrencyAccountList) {
			list.emplace_back(i.Currency.Name + "账户, 余额:" + (format("%.2lf") % i.Currency.Amount).str() + " 存期:" + i.DespoitType.Name);
		}

		list.emplace_back("退出");
		auto choose = CLI::ShowChooseList("请选择需要取现的账户：", list);
		if (choose == list.size()) return;

		auto &CA = acc.CurrencyAccountList[choose - 1];

		string amount = CLI::GetInput("请输入金额(定点两位小数):", regex("^\\d*.\\d{2}"));

		CLI::ShowBoxMsg("即将对 " + CA.Currency.Name + " 账户取现 " + amount + " 元");
		string res = CLI::GetInput("是否继续？(Y/N)", regex("^[yYnN]"));

		if (res == "N" || res == "n") return;

		if (CA.Currency.Amount < stod(amount)) {
			CLI::ShowMsg("账户余额不足, 操作取消");
			return;
		}

		auto finalAmount = Controller->UpdateCurrencyAccount(choose - 1, stod(amount), false);

		CLI::ShowMsg("请给付 " + CA.Currency.Name + (format("%.2lf") % finalAmount).str() + " 元");

		Data->AddRecord(Controller->GetRecord().back());

		CLI::ShowMsg("操作成功完成");

		controller::io::WriteAccountData(*Data);
		controller::io::WriteAccountLog(*Data);
	}

	void AccountManageView::Transffer()
	{
		string number = CLI::GetInput("请输入转入的卡号:", regex("^\\d{8}"));
		string amount = CLI::GetInput("请输入金额(定点两位小数):", regex("^\\d*.\\d{2}"));

		CLI::ShowBoxMsg("即将从本账户对 " + number + " 账户转入 " + amount + " 元");
		string choose = CLI::GetInput("是否继续？(Y/N)", regex("^[yYnN]"));

		if (choose == "N" || choose == "n") return;

		model::Account acc = Controller->GetAccount(), targetAcc;

		try {
			targetAcc = Data->GetAccount(number);
		}
		catch (std::invalid_argument &e) {
			CLI::ShowMsg("操作失败，查无此户");
			return;
		}

		if (acc.CurrencyAccountList[0].Currency.Amount < stod(amount)) {
			CLI::ShowMsg("默认活期账户余额不足, 操作取消");
			return;
		}

		auto targetAccRecord = Data->GetAccountRecord(number);

		targetAcc.CurrencyAccountList[0].Currency.Amount += stod(amount);
		Controller->Transfer(number, stod(amount));

		model::Record rec(number, targetAcc.Name, "转入", model::Currency(model::MainCurrency.Name, model::MainCurrency.Code, stod(amount)), "来源卡号:" + acc.Number);

		targetAccRecord.push_back(rec);

		Data->AddRecord(Controller->GetRecord().back());
		Data->AddRecord(rec);

		Data->UpdateAccount(targetAcc, targetAccRecord);

		CLI::ShowMsg("操作已成功完成");

		controller::io::WriteAccountData(*Data);
		controller::io::WriteAccountLog(*Data);
	}

	void AccountManageView::NewCurrencyAccount()
	{
		std::vector<string> list;
		auto FEList = Data->GetForeignExchangeList();
		auto DPList = Data->GetDepositList();
		list.emplace_back(model::MainCurrency.Name + " " + model::MainCurrency.Code);
		for (auto i : FEList) {
			list.emplace_back(i.second.Name + " " + i.second.Code);
		}
		list.emplace_back("退出");

		auto CurrencyIndex = CLI::ShowChooseList("请选择币种:", list);
		if (CurrencyIndex == list.size()) return;

		model::CurrencyAccount CA;
		if (CurrencyIndex == 1) CA.Currency = model::MainCurrency;

		size_t iter = 1;
		for (auto i : FEList) {
			if (++iter == CurrencyIndex) {
				CA.Currency.Name = i.second.Name;
				CA.Currency.Code = i.second.Code;
				break;
			}
		}

		list.clear();

		for (auto i : DPList) {
			list.emplace_back(i.second.Name + " 利率: " + to_string(i.second.IRPerYear) + "%");
		}
		list.emplace_back("退出");

		auto DepositIndex = CLI::ShowChooseList("请选择存期:", list);

		if (DepositIndex == list.size()) return;

		iter = 0;
		for (auto i : DPList) {
			if (++iter == DepositIndex){
				CA.DespoitType = i.second;
				break;
			}
		}

		string amount = CLI::GetInput("请输入金额(定点两位小数):", regex("^\\d*.\\d{2}"));

		CLI::ShowBoxMsg("即将开设 " + CA.Currency.Name + " 账户 " + "存款类型: " + CA.DespoitType.Name + " 金额: " + amount + " 元");
		string choose = CLI::GetInput("是否继续？(Y/N)", regex("^[yYnN]"));

		if (choose == "N" || choose == "n") return;

		CA.Currency.Amount = stod(amount);
		CA.LastUpdateDate = model::date::day_clock::local_day();
		CA.Period = decltype(CA.Period)(CA.LastUpdateDate, CA.LastUpdateDate);

		Controller->NewCurrencyAccount(CA);

		controller::io::WriteAccountData(*Data);
		controller::io::WriteAccountLog(*Data);

	}

	void AccountManageView::CloseAccount()
	{
		auto acc = Controller->GetAccount();
		std::vector<string> list;

		for (auto i : acc.CurrencyAccountList) {
			list.emplace_back(i.Currency.Name + "账户, 余额:" + (format("%.2lf") % i.Currency.Amount).str() + " 存期:" + i.DespoitType.Name);
		}

		list.emplace_back("退出");
		auto choose = CLI::ShowChooseList("请选择需要销去的账户：", list);
		if (choose == list.size() + 1) return;
		if (choose == 1) { //默认活期账户
			CLI::ShowBoxMsg("即将销去整个账户!");
			string res = CLI::GetInput("是否继续？(Y/N)", regex("^[yYnN]"));

			if (res == "N" || res == "n") return;

			size_t size = acc.CurrencyAccountList.size();
			for (auto i = 0u; i < size; ++i) {
				auto amount = Controller->DelCurrencyAccount(0);
				CLI::ShowMsg("请给付" + acc.CurrencyAccountList[i].Currency.Name + (format("%.2lf") % amount).str() + "元");
				Data->AddRecord(Controller->GetRecord().back());
			}

			Controller->Closed = true;
			this->Logoff();
		}
		else {
			CLI::ShowBoxMsg("即将销去" + acc.CurrencyAccountList[choose - 1].Currency.Name + "账户!");
			string res = CLI::GetInput("是否继续？(Y/N)", regex("^[yYnN]"));

			if (res == "N" || res == "n") return;

			auto amount = Controller->DelCurrencyAccount(choose - 1);
			CLI::ShowMsg("请给付" + acc.CurrencyAccountList[choose - 1].Currency.Name + (format("%.2lf") % amount).str() + "元");
			Data->AddRecord(Controller->GetRecord().back());
		}

		CLI::ShowMsg("操作成功完成");


		controller::io::WriteAccountData(*Data);
		controller::io::WriteAccountLog(*Data);

	}

	void AccountManageView::ChangePassword()
	{
		std::string password, password2nd;

		do {
			password = CLI::GetInput("请输入6位数字作为新密码:", regex("^\\d{6}"));
			password2nd = CLI::GetInput("请再输入一次以确认密码无误:");
		} while (password != password2nd && (CLI::ShowMsg("两次密码不一致，请重试"), true));

		Controller->ChangePassword(password);

		CLI::ShowMsg("操作成功");

		controller::io::WriteAccountData(*Data);

	}

}
