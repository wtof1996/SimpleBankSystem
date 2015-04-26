#pragma once

#include <string>
#include "..\Helper.hpp"
//#ifndef USERMODEL_HPP_INCLUDED
//#define USERMODEL_HPP_INCLUDED


namespace model
{
	using std::string;
	using helper::SplitModelString;
	using helper::CombineString;

	class User
	{
	public:

		string Name;
		string PasswordHash;
		virtual bool isAdmin() { return false;}

		void FromString(const string &str)
		{
			auto t = SplitModelString(str);
			Name = t[0]; PasswordHash = t[1];
		}
		string ToString() { return CombineString(Name, PasswordHash); }

		User(const string &name = "", const string &pwd = "")
			:Name(name), PasswordHash(pwd) { }
	};

	class BankTeller : public User
	{
	public:
		BankTeller(const string &name = "", const string &pwd = "")
			:User(name, pwd) { }
	};

	class Administrator : public User
	{
    public:

		Administrator(const string &name = "", const string &pwd = "")
			:User(name, pwd) { }
        virtual bool isAdmin() { return true;}
	};
}


//#endif // USERMODEL_HPP_INCLUDED
