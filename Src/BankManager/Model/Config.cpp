#include "Config.hpp"
#include <iostream>
#include <boost\log\trivial.hpp>
#include <boost\filesystem.hpp>
#include <boost\filesystem\fstream.hpp>
#include <stdexcept>

using namespace boost::program_options;

namespace config
{
	Config::Config()
		:optionDescription("使用说明")
	{
		optionDescription.add_options()
			("help", "显示帮助")
			("userpath,u", value<string>(&userDataPath), "指定用户数据文件路径,默认.\\UserData.xml")
			("logpath,l", value<string>(&accountLogPath), "指定账户交易记录路径, 默认.\\AccountLog.xml")
			("account,a", value<string>(&accountDataPath), "指定账户数据文件路径, 默认.\\AccountData.xml")
			("config,c", value<string>(&logicConfigPath), "指定业务逻辑设置文件路径, 默认.\\LogicConfig.xml")
			("aeskey", value<string>(), "自定义AES KEY")
			("aesiv", value<string>(), "自定义AES IV")
			;
	}

	void Config::ParseOption(int argc, char* argv[])
	{
 		variables_map vm;
		store(parse_command_line(argc, argv, optionDescription), vm);
		notify(vm);

		if (vm.empty()) {
			if (boost::filesystem::status(".\\config.ini").type() == boost::filesystem::regular_file) {
				BOOST_LOG_TRIVIAL(info) << "Read Config from .\\config.ini";
				store(parse_config_file<char>(".\\config.ini", optionDescription), vm);
			}
			else {
				BOOST_LOG_TRIVIAL(warning) << "No option dectected, use default values";

			}
		}
		
		if (vm.count("aeskey")) {
			BOOST_LOG_TRIVIAL(info) << "Use custom AES key";
			string aes = vm["aeskey"].as<string>();
			std::cout << "无效的AES Key" << std::endl;
			if (aes.size() != 16) throw std::length_error("Invalid AES key");
			AESKey = aes;
		}

		if (vm.count("aesiv")) {
			BOOST_LOG_TRIVIAL(info) << "Use custom AES iv";
			string iv = vm["aeskey"].as<string>();
			if (iv.size() != 16) throw std::length_error("Invalid AES IV");
			std::cout << "无效的AES iv" << std::endl;
			AESIV = iv;
		}

		auto checkPath = [](string &p)
		{
			if (boost::filesystem::status(p).type() != boost::filesystem::regular_file){
				std::cout << "无效的文件路径: " << p;
				throw std::invalid_argument("Invalid Path:" + p);
			}
		};

		checkPath(accountDataPath);
		checkPath(accountLogPath);
		checkPath(userDataPath);
		checkPath(logicConfigPath);

	}
}
