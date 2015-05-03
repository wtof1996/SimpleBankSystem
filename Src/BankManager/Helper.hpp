#pragma once

//#ifndef HELPER_HPP_INCLUDED
//#define HELPER_HPP_INCLUDED

#include <string>
#include <sstream>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"

#include <cryptopp\cryptlib.h>
#include <cryptopp\sha.h>
#include <cryptopp\aes.h>
#include <cryptopp\modes.h>
#include <cryptopp\filters.h>
#include <boost\algorithm\string.hpp>

#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop

namespace helper
{

	using std::string;
	static CryptoPP::SHA1 _SHA1;
	const string DefaultAESKey = "DefaultAESKey123"; //默认AES Key
	const string DefaultAESIV = "1234567890123456"; //默认AES IV
	const string NullString = "Null";

	string SHA1(const string &msg);
	string AES_128_EncryptHex(const string &plain, const string &key = DefaultAESKey, const string &iv = DefaultAESIV);
	string AES_128_DecryptHex(string cipherHex, const string &key = DefaultAESKey, const string &iv = DefaultAESIV);

	void InitLog();
	
	//分解Model的序列化字符串，方便构造对象
	inline std::vector<string> SplitModelString(const string &str)
	{
		std::vector<string> t;
		boost::split(t, str, boost::is_any_of("|"));
		return t;
	}
	
	//将CombineString(s1, s2, ...)组合为 s1|s2|...的形式，以进行序列化
	template<typename T>
	string CombineString(const T &t)
	{
		return t;
	}
	//P.S:旧式C的可变长度参数列表一般不能正常用于C++的类，因此这里使用了变长参数模板
	template<typename T, typename... Args>
	string CombineString(const T &t, const Args&... rest)
	{
		return t + "|" + CombineString(rest...); //递归展开参数包
	}


}

//#endif // HELPER_HPP_INCLUDED