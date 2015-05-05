#pragma once

//#ifndef BASICMODEL_HPP_INCLUDED
//#define BASICMODEL_HPP_INCLUDED

#include <string>
#include <vector>
#include <boost\date_time\gregorian\gregorian.hpp>
#include <boost\algorithm\string.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>
#include "..\Helper.hpp"

namespace model
{
	using std::string;
	using helper::SplitModelString;
	using helper::CombineString;
	
	//货币类，用于表示实际的货币
	class Currency
	{
	public:

		string Name = "";  //货币名称
		string Code = "";  //货币代码
		double Amount = NAN; //金额

		Currency(const string & name, const string & code, double amount = NAN)
			:Name(name), Code(code), Amount(amount) {}

        Currency(const string& serial)
        {
            FromString(serial);
        }

        Currency() = default;
		Currency(const Currency &) = default;
		Currency& operator= (const Currency&) = default;
		~Currency() = default;


		void FromString(const string &str)
		{
			auto t = SplitModelString(str);
			Name = t[0]; Code = t[1]; Amount = stod(t[2]);
		}
		string ToString() { return CombineString(Name, Code, std::to_string(Amount)); }
	};

	static const Currency MainCurrency = Currency("人民币", "CNY");
	//主要货币
	
	//外汇类, 表示一种外汇
	class ForeignExchange
	{
	public:
		string Name = ""; //外汇名称
		string Code = ""; //外汇货币代码
		double Rate = NAN;//对主要货币的汇率, 1:主要货币

		ForeignExchange(const string & name, const string & code, double rate)
			:Name(name), Code(code), Rate(rate) { }

        ForeignExchange(const string& serial)
        {
            FromString(serial);
        }

        ForeignExchange() = default;
		ForeignExchange(const ForeignExchange &) = default;
		ForeignExchange& operator= (const ForeignExchange&) = default;
		~ForeignExchange() = default;

		void FromString(const string &str)
		{
			auto t = SplitModelString(str);
			Name = t[0]; Code = t[1]; Rate = stod(t[2]);
		}

		string ToString() { return CombineString(Name, Code, std::to_string(Rate)); }
	};
	
	//存款业务类，表示一种存款业务
	class Deposit
	{
	public:
		string Name = ""; //业务名称
		double IRPerYear = NAN; //年利率
		boost::gregorian::months Duration = boost::date_time::not_a_date_time; //存期，为方便计算按月数存储
		//活期则表示为not_a_date_time
        Deposit(const string& serial)
        {
            FromString(serial);
        }

		Deposit(const string & name, double rate, const decltype(Duration) &duration = boost::date_time::not_a_date_time)
			:Name(name), IRPerYear(rate), Duration(duration) { }

        Deposit() = default;
		Deposit(const Deposit &) = default;
		Deposit& operator= (const Deposit&) = default;
		~Deposit() = default;

        void FromString(const string &str)
		{
			auto t = SplitModelString(str);
			Name = t[0]; IRPerYear = stod(t[1]); Duration = boost::gregorian::months(stoi(t[2]));
		}

		string ToString() { return CombineString(Name, std::to_string(IRPerYear),
							std::to_string(Duration.number_of_months().as_number())); }
	};
	
	//货币账户，表示用某种货币存储的子账户
	class CurrencyAccount
	{
	public:
		Currency Currency; //货币
		Deposit DespoitType; //存款业务类型
		boost::gregorian::date_period Period {boost::gregorian::date(), boost::gregorian::date()}; //存期起止时间，活期为相同时间 
		boost::gregorian::date LastUpdateDate {boost::date_time::not_a_date_time}; //上一次计算利息的日期

		CurrencyAccount(const string &serial)
		{
			this->FromString(serial);
		}

		CurrencyAccount() = default;
		CurrencyAccount(const CurrencyAccount &) = default;
		CurrencyAccount& operator= (const CurrencyAccount&) = default;
		~CurrencyAccount() = default;

		void FromString(const string &str)
		{
			auto t = SplitModelString(str);
			Currency = model::Currency(t[0], t[1], stod(t[2]));
			DespoitType = Deposit(t[3], stod(t[4]), boost::gregorian::months(stoi(t[5])));
			Period = boost::gregorian::date_period(
				boost::gregorian::date(boost::gregorian::from_undelimited_string(t[6])),
				boost::gregorian::date(boost::gregorian::from_undelimited_string(t[7]))
				);
			LastUpdateDate = boost::gregorian::date(boost::gregorian::from_undelimited_string(t[8]));

		}

		string ToString() { return CombineString(Currency.ToString(), DespoitType.ToString(),
							boost::gregorian::to_iso_string(Period.begin()), boost::gregorian::to_iso_string(Period.end()),
							boost::gregorian::to_iso_string(LastUpdateDate)); }

	};
	//账户类，即客户的银行账户
	class Account
	{
	public:
		string Number = ""; // 卡号
		string Name = "";  //户名
		string PasswordHash = ""; //SHA-1后的密码
		std::vector<CurrencyAccount> CurrencyAccountList = decltype(CurrencyAccountList)();
		//子货币账户列表
		Account(const string & number, const string & name, const string & pwd, decltype(CurrencyAccountList) list = decltype(CurrencyAccountList)())
			: Number(number), Name(name), PasswordHash(pwd), CurrencyAccountList(list) { }

		Account(const string &serial)
		{
			this->FromString(serial);
		}

        Account() = default;
		Account(const Account &) = default;
		Account& operator= (const Account&) = default;
		~Account() = default;

		void FromString(const string &str)
		{
			auto t = SplitModelString(str);
			Number = t[0]; Name = t[1]; PasswordHash = t[2];
		}

		string ToString() { return CombineString(Number, Name, PasswordHash); }

	};
	//交易记录类，即用于表示一条交易记录
	class Record
	{
	public:
		string Number = "";//卡号
		string Name = "";//姓名
		string Source = "";//来源
		Currency Currency;//货币
		string Description = "";//备注
		boost::posix_time::ptime Time = boost::posix_time::not_a_date_time;//交易时间

		Record(const string &number, const string & name, const string & src, model::Currency cur, const string & desc, boost::posix_time::ptime time = boost::posix_time::second_clock::local_time())
			:Number(number), Name(name), Source(src), Currency(cur), Description(desc), Time(time) { }

        Record(const string &serial)
		{
			this->FromString(serial);
		}

		Record() = default;
		Record(Record &&) = default;
		Record(const Record &) = default;
		Record& operator= (const Record&) = default;
		~Record() = default;

		bool operator < (const Record &rhs)
		{
			return this->Time < rhs.Time;
		}

		void FromString(const string &str)
		{
			auto t = SplitModelString(str);
			Number = t[0]; Name = t[1]; Source = t[2];
			Currency = model::Currency(t[3], t[4], stod(t[5]));
			Time = boost::posix_time::from_iso_string(t[6]);
		}
		string ToString() { return CombineString(Number, Name, Source, Currency.ToString(), boost::posix_time::to_iso_string(Time) ); }
	};

}

//#endif
 // BASICMODEL_HPP_INCLUDED