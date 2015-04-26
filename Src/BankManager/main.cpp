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
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <boost\algorithm\string.hpp>
#include "Controller\DataController.hpp"

using std::string;

using namespace boost::property_tree;
using namespace std;


const string WelcomeInfo = "欢迎使用银行业务系统!";

void Test();

int main(int argc, char* argv[])
{

try
{
    helper::InitLog();

	BOOST_LOG_TRIVIAL(info) << "Program Start";

	Test();

	//Config::get().ParseOption(argc, argv);

	CLI::ShowMsg(WelcomeInfo);

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

void Test()
{
	using namespace model;
	using namespace helper;
	using namespace controller;
	/*

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

	controller::DataController d;

	d.AdministratorList["admin"] = Administrator("admin", SHA1("admin"));
	d.BankTellerList["teller1"] = BankTeller("teller1", SHA1("teller1"));
	d.BankTellerList["teller2"] = BankTeller("teller2", SHA1("teller2"));

	d.ForeignExchangeList["USD"] = ForeignExchange(string("美元"), string("USD"), 6.1290);
	d.ForeignExchangeList["USD"] = ForeignExchange(string("英镑"), string("GBP"), 9.1645);

	d.DepositList["活期"] = Deposit(string("活期"), 0.35);

	d.AccountList["123456789"] = Account(string("123456789"), string("冯懿宣"), string(SHA1("123456")));
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

	d.TotalRecord.push_back(Record("12345678", "冯懿宣", "开户", ca.Currency, boost::posix_time::ptime(ca.LastUpdateDate)));


	ptree pt, pt2 = d.GetBankTellerListPtree();
	xml_writer_settings<string> settings('\t', 1);

	for (auto &i : d.AdministratorList){
		pt.add(AdministratorListPath, i.second.ToString());
	}

    auto child = pt2.get_child("User.BankTellers");
	for(auto &i : child){
        pt.add(BankTellerListPath, i.second.get_value<string>());
	}

	write_xml(Config::get().GetUserDataPath(), pt, std::locale(), settings);

}
