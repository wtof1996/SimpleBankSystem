#pragma once

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



//#ifndef HELPER_HPP_INCLUDED
//#define HELPER_HPP_INCLUDED
namespace helper
{

	using std::string;
	static CryptoPP::SHA1 _SHA1;
	const string DefaultAESKey = "DefaultAESKey123";
	const string DefaultAESIV = "1234567890123456";
	const string NullString = "Null";

	string SHA1(const string &msg);
	string AES_128_EncryptHex(const string &plain, const string &key = DefaultAESKey, const string &iv = DefaultAESIV);
	string AES_128_DecryptHex(const string &cipherHex, const string &key = DefaultAESKey, const string &iv = DefaultAESIV);

	void InitLog();

	inline std::vector<string> SplitModelString(string str)
	{
		std::vector<string> t;
		boost::trim(str);
		boost::split(t, str, boost::is_any_of("|"));
		return t;
	}

	template<typename T>
	string CombineString(const T &t)
	{
		return t;
	}

	template<typename T, typename... Args>
	string CombineString(const T &t, const Args&... rest)
	{
		return t + "|" + CombineString(rest...);
	}


}


//#endif // HELPER_HPP_INCLUDED
