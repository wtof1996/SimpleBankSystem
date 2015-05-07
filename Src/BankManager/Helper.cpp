#include "Helper.hpp"

#include <array>
#include <iostream>
#include <random>
#include <ctime>

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>


const std::array<std::string, 256> uCharHexTable =
{ "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b",
  "0c", "0d", "0e", "0f", "10", "11", "12", "13", "14", "15", "16", "17",
  "18", "19", "1a", "1b", "1c", "1d", "1e", "1f", "20", "21", "22", "23",
  "24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f",
  "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3a", "3b",
  "3c", "3d", "3e", "3f", "40", "41", "42", "43", "44", "45", "46", "47",
  "48", "49", "4a", "4b", "4c", "4d", "4e", "4f", "50", "51", "52", "53",
  "54", "55", "56", "57", "58", "59", "5a", "5b", "5c", "5d", "5e", "5f",
  "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b",
  "6c", "6d", "6e", "6f", "70", "71", "72", "73", "74", "75", "76", "77",
  "78", "79", "7a", "7b", "7c", "7d", "7e", "7f", "80", "81", "82", "83",
  "84", "85", "86", "87", "88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
  "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9a", "9b",
  "9c", "9d", "9e", "9f", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
  "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af", "b0", "b1", "b2", "b3",
  "b4", "b5", "b6", "b7", "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf",
  "c0", "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8", "c9", "ca", "cb",
  "cc", "cd", "ce", "cf", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
  "d8", "d9", "da", "db", "dc", "dd", "de", "df", "e0", "e1", "e2", "e3",
  "e4", "e5", "e6", "e7", "e8", "e9", "ea", "eb", "ec", "ed", "ee", "ef",
  "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "fa", "fb",
  "fc", "fd", "fe", "ff" };
  //unsigned char转HEX字符串表

inline byte Hex2uChar(const char &high, const char &low)
{

	auto convert = [](char a){return (a >= 'a') ? a - 'a' + 0x0a : a - '0'; };

	byte ret;
	ret = convert(high);
	ret <<= 4;
	ret += convert(low);

	return ret;

}
//将一对HEX char转回unsigned  char(byte)

namespace helper
{

	//SHA-1 Hash函数, 输出为hex字符串
	string SHA1(const string &msg)
	{
		byte* ResultBytes = new byte[_SHA1.DigestSize()];
		_SHA1.Update(reinterpret_cast<const byte*>(msg.c_str()), msg.size());
		_SHA1.Final(ResultBytes);

		std::stringstream s;

		s << std::hex;
		for (unsigned i = 0; i < _SHA1.DigestSize(); ++i) s << uCharHexTable[static_cast<unsigned>(ResultBytes[i])];

		delete[] ResultBytes;
		ResultBytes = nullptr;

		return s.str();
	}

	//AES－128－CBC 加密算法
	string AES_128_EncryptHex(const string &plain, const string &key, const string &iv)
	{
#ifdef NO_AES
		return plain; // Only for debug
#endif
		byte bkey[CryptoPP::AES::DEFAULT_KEYLENGTH], biv[CryptoPP::AES::BLOCKSIZE];

		for (size_t i = 0; i < CryptoPP::AES::DEFAULT_KEYLENGTH; ++i) bkey[i] = key[i];
		for (size_t i = 0; i < CryptoPP::AES::BLOCKSIZE; ++i) biv[i] = iv[i];

		string cipherText;

		CryptoPP::AES::Encryption AES_128_Encypter(bkey, CryptoPP::AES::DEFAULT_KEYLENGTH);
		CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncypter(AES_128_Encypter, biv);
		CryptoPP::StreamTransformationFilter sftEncypter(cbcEncypter, new CryptoPP::StringSink(cipherText));

		sftEncypter.Put(reinterpret_cast<const byte*>(plain.c_str()), plain.length());
		sftEncypter.MessageEnd();

		string cipherTextHex;
		for (auto i : cipherText) {
			cipherTextHex += uCharHexTable[static_cast<byte>(i)];
		}

		return cipherTextHex;

	}

	//AES-128-CBC解密算法，要求输入字符串为Hex字符串
	string AES_128_DecryptHex(string cipherHex, const string &key, const string &iv)
	{

#ifdef NO_AES
		return cipherHex;
#endif
		byte bkey[CryptoPP::AES::DEFAULT_KEYLENGTH], biv[CryptoPP::AES::BLOCKSIZE];

		for (size_t i = 0; i < CryptoPP::AES::DEFAULT_KEYLENGTH; ++i) bkey[i] = key[i];
		for (size_t i = 0; i < CryptoPP::AES::BLOCKSIZE; ++i) biv[i] = iv[i];

		boost::trim(cipherHex);//去除空白字符
		string cipherText(cipherHex.size() / 2, '\0');
		for (size_t i = 0; i < cipherHex.size(); i += 2){
			cipherText[i / 2] = Hex2uChar(cipherHex[i], cipherHex[i + 1]);
		}

		string plainText;

		CryptoPP::AES::Decryption AES_128_Decypter(bkey, CryptoPP::AES::DEFAULT_KEYLENGTH);
		CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecypter(AES_128_Decypter, biv);
		CryptoPP::StreamTransformationFilter sftDecypter(cbcDecypter, new CryptoPP::StringSink(plainText));

		sftDecypter.Put(reinterpret_cast<const byte*>(cipherText.c_str()), cipherText.length());
		sftDecypter.MessageEnd();

		return plainText;
	}

	//初始化Log

    void InitLog()
	{
		boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
		boost::log::add_file_log(
			boost::log::keywords::file_name = ".\\logs\\%Y-%m-%d_%H-%M-%S.%N.log", //Log路径
			boost::log::keywords::format = "[%TimeStamp%] (%Severity%) : %Message%" // Log格式
			);
		boost::log::add_common_attributes();

		BOOST_LOG_TRIVIAL(info) << "Log Start";
	}

	static std::mt19937 eng(static_cast<unsigned long>(time(nullptr)));
	const int CardNumberLength = 8;

	string CardNumberGenerator()
	{
		std::uniform_int_distribution<char> dist('0', '9');
		string ret;
		for (int i = 0; i < CardNumberLength; ++i)
			ret.push_back(dist(eng));
		return ret;
	}
}
