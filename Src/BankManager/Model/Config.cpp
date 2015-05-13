#include <iostream>
#include <stdexcept>

#include <boost\log\trivial.hpp>
#include <boost\filesystem.hpp>
#include <boost\filesystem\fstream.hpp>

#include "Config.hpp"
#include "..\View\cli.hpp"

#ifdef CONV_UTF8_GBK
#include <boost\locale.hpp>
#endif // CONV_UTF8_GBK

using namespace boost::program_options;

inline std::string convert(const std::string &s)
{

#ifdef CONV_UTF8_GBK
	return boost::locale::conv::from_utf(s, "GBK");
#else
	return s;
#endif // CONV_UTF8_GBK
}

namespace config
{
	Config::Config()
		:optionDescription(convert("使用说明").c_str())
	{
		optionDescription.add_options()
			("help", convert("显示帮助").c_str())
			("userpath,u", value<string>(&userDataPath), convert("指定用户数据文件路径,默认.\\UserData.xml").c_str())
			("logpath,l", value<string>(&accountLogPath), convert("指定账户交易记录路径, 默认.\\AccountLog.xml").c_str())
			("account,a", value<string>(&accountDataPath), convert("指定账户数据文件路径, 默认.\\AccountData.xml").c_str())
			("config,c", value<string>(&logicConfigPath), convert("指定业务逻辑设置文件路径, 默认.\\LogicConfig.xml").c_str())
			("aeskey", value<string>(), convert("自定义AES 密钥").c_str())
			("aesiv", value<string>(), convert("自定义AES IV").c_str())
			;
	}

	void Config::ParseOption(int argc, char* argv[])
	{
 		variables_map vm;
		//优先检查设置文件
		if (boost::filesystem::status(".\\config.ini").type() == boost::filesystem::regular_file) {
			BOOST_LOG_TRIVIAL(info) << "Read Config from .\\config.ini";
			
			store(parse_config_file<char>(".\\config.ini", optionDescription), vm);
			notify(vm);
		}
		else {
			BOOST_LOG_TRIVIAL(warning) << "No option file dectected, some options may use default values if unspecificed.";

		}

		store(parse_command_line(argc, argv, optionDescription), vm);
		notify(vm);

		if (vm.count("help")) {
			std::cout << optionDescription;
			throw std::logic_error("Show Help Message");
		}

		if (vm.count("aeskey")) {
			BOOST_LOG_TRIVIAL(info) << "Using custom AES key";
			string aes = vm["aeskey"].as<string>();

			if (aes.size() != 16) CLI::ShowMsg("无效的AES 密钥"), throw std::length_error("Invalid AES key:" + aes);
			AESKey = aes;
		}

		if (vm.count("aesiv")) {
			BOOST_LOG_TRIVIAL(info) << "Using custom AES iv";
			string iv = vm["aesiv"].as<string>();
			if (iv.size() != 16) CLI::ShowMsg("无效的AES IV"), throw std::length_error("Invalid AES IV:" + iv);
			AESIV = iv;
		}

		auto checkPath = [](string &p){if (boost::filesystem::status(p).type() != boost::filesystem::regular_file)
										CLI::ShowMsg("无效的文件路径: " + p),  throw std::invalid_argument("Invalid Path:" + p);};
		//保证数据文件有效性
		checkPath(accountDataPath);
		checkPath(accountLogPath);
		checkPath(userDataPath);
		checkPath(logicConfigPath);

	}
}