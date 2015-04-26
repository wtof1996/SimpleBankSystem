#include "IOController.hpp"
#include <string>

using namespace boost::property_tree;

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
		read_xml(Config::get().GetUserDataPath(), pt);

		data.SetUserList(pt.get_child(UserDataRoot));


	}

	void WriteUserData(DataController &data)
	{

	}

	void ReadAccountData(DataController &data) 
	{
	
	}
	
	void WriteAccountData(DataController &data)
	{

	}

	void ReadAccountLog(DataController &data)
	{
	
	}
	
	void WriteAccountLog(DataController &data)
	{
	
	}

	void ReadLogicConfig(DataController &data) 
	{
	
	}
	
	void WriteLogicConfig(DataController &data)
	{

	}

	void PrintAccountLog(const DataController &data, const model::Account &acc, const file::path &Path)
	{

	}

	void PrintAccountInfo(const DataController &data, const model::Account &acc, const file::path &Path)
	{

	}

	void PrintAccountLog(DataController &data, const file::path &Path)
	{

	}

	void PrintAccountList(DataController &data, const file::path &Path)
	{

	}
}}