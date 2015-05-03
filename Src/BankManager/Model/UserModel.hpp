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
	//用户基类
	class User
	{
	public:
		string Name = ""; //姓名
		string PasswordHash = "";//SHA-1后的密码
		virtual bool isAdmin() { return false;} //是否为管理员

		void FromString(const string &str)
		{
			auto t = SplitModelString(str);
			Name = t[0]; PasswordHash = t[1];
		}
		string ToString() { return CombineString(Name, PasswordHash); }

		User() = default;

		User(const string &name, const string &pwd)
			:Name(name), PasswordHash(pwd) { }

        User(const string& serial)
		{
			FromString(serial);
		}

        virtual ~User() = default;
	};
	
	//柜员
	class BankTeller : public User
	{
	public:
		using User::User;
	};
	
	//管理员
	class Administrator : public User
	{
    public:
		using User::User;
        virtual bool isAdmin() { return true;}
	};
}


//#endif // USERMODEL_HPP_INCLUDED