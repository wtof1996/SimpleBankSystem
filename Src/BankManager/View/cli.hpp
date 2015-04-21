#pragma once

//#ifndef CLI_HPP_INCLUDED
//#define CLI_HPP_INCLUDED

#include <iostream>
#include <string>
#include <boost\regex.hpp>
#include <boost\function.hpp>
#include <initializer_list>
#include <vector>
#include <cstdlib>
#include <exception>
#include <sstream>

namespace CLI
{
	using std::string;
	using std::cin;
	using std::cout;
	using std::endl;

	const string MenuInputDirection = "请输入选项,输入完后按回车";
	const string RetryDirection = "无效输入，请重新输入";

	template<typename T>
	class MenuItem
	{
	public:
		string Name;
		boost::function<void(T&)> CallBack;

		MenuItem<T>() = delete;

		MenuItem<T>(const string &name, const boost::function<void(T&)> &callback) noexcept
			: Name(name), CallBack(callback){}

		~MenuItem<T>() = default;
	};

	class ListItem
	{
	public:
		string Name;
		std::vector<string> Data;

		ListItem() = delete;

		ListItem(const string &name, const std::vector<string> &data)
			:Name(name), Data(data) {}

		~ListItem() = default;
	};

	void ShowMsg(const string &msg);
	void ShowBoxMsg(const string &msg, const char border = '*');
	string GetInput(const string &direction);
	string GetInput(const string &direction, const boost::regex &checker);

	inline int to_number(string &a)
	{
		std::stringstream s(a);
		int ret = 0;
		s >> ret;
		return ret;
	}

	inline int to_number(string &&a)
	{
		std::stringstream s(a);
		int ret = 0;
		s >> ret;
		return ret;
	}

	template<typename T>
	void ShowMenu(T& data, std::initializer_list<MenuItem<T>> menu, const string &headline)
	{
		ShowBoxMsg(headline);

		cout << endl;

		size_t index = 0;
		for (auto &i : menu) {
			cout << "[" << ++index << "] : " << i.Name << endl;
		}

		size_t input;
		do{
			input = to_number(GetInput(MenuInputDirection + "[1-" + std::to_string(menu.size()) + "] : ", 
							  boost::regex("^\\d+")));

		} while (( input <= 0 || input > menu.size() )&& (ShowMsg(RetryDirection), true));

		index = 0;
		for (auto &i : menu) {
			if (++index == input){
				i.CallBack(data);
				break;
			}
		}

	}
#ifdef _WIN32
	inline void CleanCLI() { system("cls"); }
#endif

#ifdef __linux__
	inline void CleanCLI() { system("clean"); }
#endif

}


//#endif // CLI_HPP_INCLUDED
