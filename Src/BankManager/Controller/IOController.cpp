#include "IOController.hpp"
#include <boost\filesystem\fstream.hpp>
#include <boost\format.hpp>
#include <string>
#include <stdexcept>

using namespace boost::property_tree;


//通用XML读写函数
void ReadXMLFile(const std::string &path, ptree &pt)
{
	try{
		read_xml(path, pt);
	}
	catch (std::exception &e)
	{
		CLI::ShowMsg("无法读取文件:" + path);
		BOOST_LOG_TRIVIAL(fatal) << "Can't read file:" + path;
		throw;
	}
}

void WriteXMLFile(const std::string &path, const ptree &pt)
{
	static xml_writer_settings<std::string> settings('\t', 1);
	try{
		write_xml(path, pt, std::locale(), settings);
	}
	catch (std::exception &e)
	{
		CLI::ShowMsg("无法写入文件:" + path);
		BOOST_LOG_TRIVIAL(fatal) << "Can't write file:" + path;
		throw;
	}
}

template<typename T>
void PrintLog(const T &log, const boost::filesystem::path &path)
{
	using std::endl;

	boost::filesystem::ofstream fout(path);

	fout << " 卡号 | 姓名 | 来源 | 货币代码 | 金额 | 时间 | 备注 " << endl;

	auto addSpace = [](const std::string &a) { return " " + a + " "; };

	for (auto i : log) {
		fout << helper::CombineString(addSpace(i.Number), addSpace(i.Name), addSpace(i.Source),
			addSpace(i.Currency.Code), addSpace((boost::format("%.2lf") % i.Currency.Amount).str()),
			addSpace(boost::posix_time::to_iso_extended_string(i.Time)),
			addSpace(i.Description))
			<< endl << endl;
	}

	fout.close();
}

void PrintAccInfo(const model::Account &acc, boost::filesystem::ofstream &fout)
{
	using std::endl;

	fout << "卡号 : " << acc.Number << endl
		<< "姓名 : " << acc.Name << endl;

	for (auto i : acc.CurrencyAccountList){
		fout << i.Currency.Name << "账户: " << endl;
		fout << "        余额 : " << (boost::format("%.2lf") % i.Currency.Amount).str() << endl;
		fout << "        存款类型 : " << i.DespoitType.Name << endl;
		fout << "        利率(%) : " << i.DespoitType.IRPerYear << endl;
		fout << "        计息开始日期　:　" << boost::gregorian::to_iso_extended_string(i.Period.begin()) << endl;
		fout << "        最后更新日期 : " << boost::gregorian::to_iso_extended_string(i.LastUpdateDate) << endl;
		fout << endl;
	}
}

namespace controller { namespace io
{
	void ReadXMLFiles(DataController &data)
	{
		ReadUserData(data);
		ReadAccountData(data);
		ReadAccountLog(data);
		ReadLogicConfig(data);
	}

	void WriteXMLFiles(DataController &data)
	{
		WriteUserData(data);
		WriteAccountData(data);
		WriteAccountLog(data);
		WriteLogicConfig(data);
	}

	void ReadUserData(DataController &data)
	{
		ptree pt;
		ReadXMLFile(Config::get().GetUserDataPath(), pt);

		data.SetUserList(pt.get_child(UserDataRoot));
	}

	void WriteUserData(DataController &data)
	{
		ptree pt = data.GetUserListPtree();
		ptree real_pt;
		real_pt.add_child(UserDataRoot, pt);

		WriteXMLFile(Config::get().GetUserDataPath(), real_pt);

	}

	void ReadAccountData(DataController &data)
	{
		ptree pt;
		ReadXMLFile(Config::get().GetAccountDataPath(), pt);

		data.SetAccountList(pt);
	}

	void WriteAccountData(DataController &data)
	{
		ptree pt = data.GetAccountListPtree();

		WriteXMLFile(Config::get().GetAccountDataPath(), pt);

	}

	void ReadAccountLog(DataController &data)
	{
		ptree pt;
		ReadXMLFile(Config::get().GetAccountLogPath(), pt);

		data.SetTotalRecord(pt);
	}

	void WriteAccountLog(DataController &data)
	{
		ptree pt = data.GetTotalRecordPtree();

		WriteXMLFile(Config::get().GetAccountLogPath(), pt);
	}

	void ReadLogicConfig(DataController &data)
	{
		ptree pt;
		ReadXMLFile(Config::get().GetLogicConfigPath(), pt);

		auto child = pt.get_child(LogicConfigRoot);

		data.SetDepositList(child);
		data.SetForeignExchangeRateList(child);
	}

	void WriteLogicConfig(DataController &data)
	{
		ptree pt;
		ptree pt2 = data.GetDepositListPtree(), pt3 = data.GetForeignExchangePtree();

		auto child = pt2.get_child(DepositListRoot);

		for (auto &i : child){
			pt.add(LogicConfigRoot + "." + DepositListPath, i.second.get_value<string>());
		}

		auto child2 = pt3.get_child(ForeignExchangeListRoot);
		for (auto &i : child2) {
			pt.add(LogicConfigRoot + "." + ForeignExchangeListPath, i.second.get_value<string>());
		}

		WriteXMLFile(Config::get().GetLogicConfigPath(), pt);
	}

	void PrintAccountLog(const DataController &data, const string &number, const file::path &path)
	{
		PrintLog(data.GetAccountRecord(number), path);
	}

	void PrintAccountInfo(const DataController &data, const string &number, const file::path &path)
	{
		auto acc = data.GetAccount(number);
		file::ofstream fout(path);

		PrintAccInfo(acc, fout);

		fout.close();
	}

	void PrintAccountLog(DataController &data, const file::path &path)
	{
		PrintLog(data.GetTotalRecord(), path);
	}

	void PrintAccountList(DataController &data, const file::path &path)
	{
		auto list = data.GetAccountList();

		file::ofstream fout(path);

		for (auto a : list) {

			auto &acc = a.second;
			PrintAccInfo(acc, fout);
		}
		fout.close();
	}
}}
