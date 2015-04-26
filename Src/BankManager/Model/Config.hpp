#pragma once

#include "BasicModel.hpp"
#include "..\Helper.hpp"
#include <vector>
#include <string>
#include <boost\program_options.hpp>

//#ifndef CONFIG_HPP_INCLUDED
//#define CONFIG_HPP_INCLUDED

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
		option::options_description optionDescription;

		string accountDataPath = ".\\AccountData.xml";
		string accountLogPath = ".\\AccountLog.xml";
		string userDataPath = ".\\UserData.xml";
		string logicConfigPath = ".\\LogicConfig.xml";

		~Config() = default;

	};

}

using config::Config;


//#endif // CONFIG_HPP_INCLUDED
