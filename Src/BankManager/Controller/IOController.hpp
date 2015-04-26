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
				, AccountDataRoot = "Accounts"
				, AccountLogRoot = "AccountLog"
				, LogicConfigRoot = "LogicConfig";

	void ReadXMLFiles(DataController &data);
	void WriteXMLFiles(DataController &data);

	void ReadUserData(DataController &data);
	void WriteUserData(DataController &data);

	void ReadAccountData(DataController &data);
	void WriteAccountData(DataController &data);

	void ReadAccountLog(DataController &data);
	void WriteAccountLog(DataController &data);

	void ReadLogicConfig(DataController &data);
	void WriteLogicConfig(DataController &data);

	void PrintAccountLog(const DataController &data,const string &acc, const file::path &Path);
	void PrintAccountInfo(const DataController &data,const string &acc, const file::path &Path);

	void PrintAccountLog(DataController &data, const file::path &Path);
	
	void PrintAccountList(DataController &data, const file::path &Path);
	

}}

//#endif // IOCONTROLLER_HPP_INCLUDED
