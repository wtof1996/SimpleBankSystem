#pragma once

//#ifndef CONFIG_HPP_INCLUDED
//#define CONFIG_HPP_INCLUDED

#include <vector>
#include <string>

#include <boost\program_options.hpp>

#include "BasicModel.hpp"
#include "..\Helper.hpp"

namespace config
{

	namespace option = boost::program_options;

	using std::string;

	class Config final
	{
	public:
		static Config& get()
		{
			static Config c;
			return c;
		}

		void ParseOption(int argc, char* argv[]);

		const string& GetAccountDataPath() { return accountDataPath; }
		const string& GetAccountLogPath() { return accountLogPath; }
		const string& GetUserDataPath() { return userDataPath; }
		const string& GetLogicConfigPath() { return logicConfigPath; }

		string AESKey = helper::DefaultAESKey;
		string AESIV = helper::DefaultAESIV;

	private:
		Config();
		option::options_description optionDescription;//命令行参数描述

		string accountDataPath = ".\\AccountData.xml";//账户数据路径
		string accountLogPath = ".\\AccountLog.xml";//账户交易记录路径
		string userDataPath = ".\\UserData.xml";//用户数据路径
		string logicConfigPath = ".\\LogicConfig.xml";//业务逻辑设置文件路径

		~Config() = default;

	};

}

using config::Config;


//#endif // CONFIG_HPP_INCLUDED