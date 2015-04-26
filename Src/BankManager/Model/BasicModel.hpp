#pragma once

#include <string>
#include <vector>
#include <boost\date_time\gregorian\gregorian.hpp>
#include <boost\algorithm\string.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>
#include "..\Helper.hpp"


//#ifndef BASICMODEL_HPP_INCLUDED
//#define BASICMODEL_HPP_INCLUDED

namespace model
{
	using std::string;
	using helper::SplitModelString;
	using helper::CombineString;

	class Currency
	{
	public:

		string Name;
		string Code;
		double Amount;

		Currency(string name = "", string code = "", double amount = NAN)
			:Name(name), Code(code), Amount(amount) {}

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

	class ForeignExchange
	{
	public:
		string Name;
		string Code;
		double Rate;

		ForeignExchange(string name = "", string code = "", double rate = NAN)
			:Name(name), Code(code), Rate(rate) { }

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

	class Deposit
	{
	public:
		string Name;
		double IRPerYear;
		boost::gregorian::months Duration;

		Deposit(string name = "", double rate = NAN, decltype(Duration) duration = boost::date_time::not_a_date_time)
			:Name(name), IRPerYear(rate), Duration(duration) { }

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

	class CurrencyAccount
	{
	public:
		Currency Currency;
		Deposit DespoitType;
		boost::gregorian::date_period Period;
		boost::gregorian::date LastUpdateDate;

		CurrencyAccount()
			:Period(boost::gregorian::date(boost::date_time::not_a_date_time), boost::gregorian::date(boost::date_time::not_a_date_time))
		{
		}

		CurrencyAccount(const CurrencyAccount &) = default;
		CurrencyAccount& operator= (const CurrencyAccount&) = default;
		~CurrencyAccount() = default;

		void FromString(const string &str)
		{
			auto t = SplitModelString(str);
			Currency = model::Currency(t[0], t[1], stod(t[2]));
			DespoitType = Deposit(t[3], stod(t[4]), boost::gregorian::months(stoi(t[5])));
			Period = boost::gregorian::date_period(
				boost::gregorian::date(boost::gregorian::from_undelimited_string(t[5])),
				boost::gregorian::date(boost::gregorian::from_undelimited_string(t[6]))
				);
			LastUpdateDate = boost::gregorian::date(boost::gregorian::from_undelimited_string(t[7]));

		}

		string ToString() { return CombineString(Currency.ToString(), DespoitType.ToString(),
							boost::gregorian::to_iso_string(Period.begin()), boost::gregorian::to_iso_string(Period.end()),
							boost::gregorian::to_iso_string(LastUpdateDate)); }

	};

	class Account
	{
	public:
		string Number;
		string Name;
		string PasswordHash;
		std::vector<CurrencyAccount> CurrencyAccountList;

		Account(string number = "", string name = "", string pwd = "", decltype(CurrencyAccountList) list = decltype(CurrencyAccountList)())
			: Number(number), Name(name), PasswordHash(pwd), CurrencyAccountList(list) { }

		Account(const Account &) = default;
		Account& operator= (const Account&) = default;
		~Account() = default;

		void FromString(const string &str)
		{
			auto t = SplitModelString(str);
			Number = t[0]; Name = t[1]; PasswordHash = stod(t[2]);
		}

		string ToString() { return CombineString(Number, Name, PasswordHash); }

	};

	class Record
	{
	public:
		string Number;
		string Name;
		string Source;
		Currency Currency;
		boost::posix_time::ptime Time;

		Record() = default;
		Record(const Record &) = default;
		Record& operator= (const Record&) = default;
		~Record() = default;

		Record(string number, string name, string src, model::Currency cur, boost::posix_time::ptime time = boost::posix_time::not_a_date_time)
			:Number(number), Name(name), Source(src), Currency(cur), Time(time) { }
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
