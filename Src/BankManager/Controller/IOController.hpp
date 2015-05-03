#pragma once

#include <boost\filesystem.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>

#include "..\View\cli.hpp"
#include "..\Helper.hpp"
#include "..\Model\BasicModel.hpp"
#include "..\Model\UserModel.hpp"
#include "..\Model\Config.hpp"
#include <boost\log\trivial.hpp>
#include "DataController.hpp"

//#ifndef IOCONTROLLER_HPP_INCLUDED
//#define IOCONTROLLER_HPP_INCLUDED

namespace controller { namespace io
{
	namespace file = boost::filesystem;
	using std::string;

	const string  UserDataRoot = "Users"
				, LogicConfigRoot = "LogicConfig";

	void ReadXMLFiles(DataController &data);
	void WriteXMLFiles(DataController &data);//读写全部XML文件

	void ReadUserData(DataController &data);
	void WriteUserData(DataController &data);//读写用户数据

	void ReadAccountData(DataController &data);
	void WriteAccountData(DataController &data);//读写账户数据

	void ReadAccountLog(DataController &data);
	void WriteAccountLog(DataController &data);//读写账户交易记录

	void ReadLogicConfig(DataController &data);
	void WriteLogicConfig(DataController &data);//读写业务逻辑数据

	void PrintAccountLog(const DataController &data, const string &number, const file::path &path);//打印某个账户的交易记录
	void PrintAccountInfo(const DataController &data, const string &number, const file::path &path);//打印账户信息

	void PrintAccountLog(DataController &data, const file::path &path);//打印全部的交易记录

	void PrintAccountList(DataController &data, const file::path &path);//打印全部账户的信息


}}

//#endif // IOCONTROLLER_HPP_INCLUDED
