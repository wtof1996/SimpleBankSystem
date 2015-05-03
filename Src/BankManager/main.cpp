#include <stdexcept>
#include <string>
#include <vector>
#include "View\cli.hpp"
#include "Helper.hpp"
#include "Model\BasicModel.hpp"
#include "Model\UserModel.hpp"
#include "Model\Config.hpp"
#include <boost\log\trivial.hpp>
#include <boost\scope_exit.hpp>
#include <clocale>
#include <windows.h>
#include "Controller\DataController.hpp"
#include "Controller\IOController.hpp"
#include "View\MainView.hpp"

using std::string;

int main(int argc, char* argv[])
{

try
{
	//setlocale(LC_ALL, "");

	helper::InitLog();
	Config::get();

	BOOST_LOG_TRIVIAL(info) << "Program Start";

	Config::get().ParseOption(argc, argv);

	view::MainView main;
	while (main.Loop) main.Show();

}
catch(std::exception &e)
{
    BOOST_LOG_TRIVIAL(fatal) << e.what();
}

BOOST_SCOPE_EXIT(void){
	BOOST_LOG_TRIVIAL(info) << "Program stopped";
}BOOST_SCOPE_EXIT_END

	return 0;

}


/*
void Test()
{
	using namespace model;
	using namespace helper;
	using namespace controller;
	using namespace controller::io;
   // xml_writer_settings<string> settings('\t', 1);
   read_xml(Config::get().GetUserDataPath(), pt);

   auto child = pt.get_child("Bank.User.Administrators");

   vector<Administrator>  lists;
   for (auto &i : child){
   vector<string> tmp;
   auto t = i.second.get_value<string>();
   boost::split(tmp, t, boost::is_any_of("|"));
   lists.push_back(Administrator(tmp[0], tmp[1]));
   }

   cout << lists[0].ToString() << endl;
   cout << lists[1].ToString() << endl;*/

	//controller::DataController d;
    /*

	d.AdministratorList["admin"] = Administrator("admin", SHA1("admin"));
	d.BankTellerList["teller1"] = BankTeller("teller1", SHA1("teller1"));
	d.BankTellerList["teller2"] = BankTeller("teller2", SHA1("teller2"));

	d.ForeignExchangeList["USD"] = ForeignExchange(string("美元"), string("USD"), 6.1241);
	d.ForeignExchangeList["EUR"] = ForeignExchange(string("欧元"), string("EUR"), 6.6380);
	d.ForeignExchangeList["JPY"] = ForeignExchange(string("日元"), string("JPY"), 0.051380);
	d.ForeignExchangeList["HKD"] = ForeignExchange(string("港币"), string("HKD"), 0.79021);
	d.ForeignExchangeList["GBP"] = ForeignExchange(string("英镑"), string("GBP"), 9.2366);
	d.ForeignExchangeList["AUD"] = ForeignExchange(string("澳元"), string("AUD"), 4.7740);
	d.ForeignExchangeList["NZD"] = ForeignExchange(string("新西兰元"), string("NZD"), 4.6556);
	d.ForeignExchangeList["SGD"] = ForeignExchange(string("新加坡元"), string("SGD"), 4.5783);
	d.ForeignExchangeList["CAD"] = ForeignExchange(string("加拿大元"), string("CAD"), 5.0450);
	d.ForeignExchangeList["RUB"] = ForeignExchange(string("卢布"), string("RUB"), 0.1206);

	d.DepositList["活期"] = Deposit(string("活期"), 0.35);
	d.DepositList["三个月"] = Deposit(string("三个月"), 2.35, boost::gregorian::months(3));
	d.DepositList["半年"] = Deposit(string("半年"), 2.55, boost::gregorian::months(6));
	d.DepositList["一年"] = Deposit(string("一年"), 2.75, boost::gregorian::months(12));
	d.DepositList["二年"] = Deposit(string("二年"), 3.25, boost::gregorian::months(24));
	d.DepositList["三年"] = Deposit(string("三年"), 3.75, boost::gregorian::months(36));
	d.DepositList["五年"] = Deposit(string("五年"), 4.00, boost::gregorian::months(60));



	d.AccountList["12345678"] = Account(string("12345678"), string("冯懿宣"), string(SHA1("123456")));
	auto ca = CurrencyAccount();
	ca.Currency = MainCurrency;
	ca.Currency.Amount = 100000.00;
	ca.DespoitType = d.DepositList["活期"];
	ca.Period = boost::gregorian::date_period(
	boost::gregorian::date(boost::gregorian::from_undelimited_string("20140423")),
	boost::gregorian::date(boost::gregorian::from_undelimited_string("20140423"))
	);
	ca.LastUpdateDate = boost::gregorian::date(boost::gregorian::from_undelimited_string("20140423"));

	d.AccountList["12345678"].CurrencyAccountList.push_back(ca);

	d.TotalRecord.push_back(Record("12345678", "冯懿宣", "开户", ca.Currency,"无" ,boost::posix_time::ptime(ca.LastUpdateDate)));

	WriteXMLFiles(d);*/

//	ReadXMLFiles(d);

	//PrintAccountInfo(d, "12345678", ".\\12345678.txt");
	//PrintAccountLog(d, "12345678", ".\\12345678.log");

	//PrintAccountList(d, ".\\account.txt");
	//PrintAccountLog(d, ".\\log.log");
	//ptree pt;
	//read_xml(Config::get().GetLogicConfigPath(), pt);
	//
	//auto child = pt.get_child("LogicConfig");
	////xml_writer_settings<string> settings('\t', 1);
	////write_xml("123.xml", child, std::locale(), settings);
	////CLI::ShowMsg(child.begin()->first);
	//
	//d.SetDepositList(child);
	//////
	//d.SetForeignExchangeRateList(child);
	//////
	//CLI::ShowMsg(d.GetDeposit("活期").Name);
	//CLI::ShowMsg(to_string(d.GetForeignExchangeRate("USD")));
//}
