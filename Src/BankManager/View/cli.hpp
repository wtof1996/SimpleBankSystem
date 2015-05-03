/*
 * CLI 界面库
 * 封装了一个简单的基于行模式的CUI
 */
#pragma once

//#ifndef CLI_HPP_INCLUDED
//#define CLI_HPP_INCLUDED

#include <iostream>
#include <string>
#include <boost\regex.hpp>
#include <boost\function.hpp>
#include <boost\log\trivial.hpp>
#include <initializer_list>
#include <vector>
#include <cstdlib>
#include <exception>
#include <sstream>

#ifdef _WIN32
#define CONV_UTF8_GBK
#endif

namespace CLI
{
	using std::string;
	using std::cin;
	using std::cout;
	using std::endl;

	const string MenuInputDirection = "请输入选项,输入完后按回车";
	const string RetryDirection = "无效输入，请重新输入";

	//菜单项
	template<typename T>
	class MenuItem
	{
	public:
		string Name;
		boost::function<void(T&)> CallBack; //回调函数，建议使用Lambda构造

		MenuItem<T>() = delete;

		MenuItem<T>(const string &name, const boost::function<void(T&)> &callback) noexcept
			: Name(name), CallBack(callback){}

		~MenuItem<T>() = default;
	};


	void ShowMsg(const string &msg);

	void ShowBoxMsg(const string &msg, const char border = '*');
	//显示一个消息框

	size_t ShowChooseList(const string &msg, std::initializer_list<string> list);
	//显示一个选择列表

	string GetInput(const string &direction);
	//这个函数只是简单的读一行

	string GetInput(const string &direction, const boost::regex &checker);
	// 读一行的基础上，通过正则表达式checker保证输入数据有效性


	//辅助函数
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

	//根据给定的MenuList显示菜单，并确保调用对应的函数
	//菜单会绑定到data对象上，不同的选项会调用其中不同的成员函数
	template<typename T>
	void ShowMenu(T& data, std::initializer_list<MenuItem<T>> menu, const string &headline)
	{
		BOOST_LOG_TRIVIAL(info) << "Menu Showed";
		BOOST_LOG_TRIVIAL(info) << "HeadLine is:" << headline;

		ShowBoxMsg(headline);

		cout << endl;

		size_t index = 0;
		for (auto &i : menu) {
			cout << "[" << ++index << "] : " << i.Name << endl;

			BOOST_LOG_TRIVIAL(debug) << "Item Index:" << index - 1;
			BOOST_LOG_TRIVIAL(debug) << "Item Name:" << i.Name;
		}

		size_t input;
		do{
			input = to_number(GetInput(MenuInputDirection + "[1-" + std::to_string(menu.size()) + "] : ",
							  boost::regex("^\\d+")));

		} while (( input <= 0 || input > menu.size() )&& (ShowMsg(RetryDirection), true));

		BOOST_LOG_TRIVIAL(info) << "User choosed No." << input;

		index = 0;
		for (auto &i : menu) {
			if (++index == input){
				i.CallBack(data);
				break;
			}
		}

	}

//以下是不同平台下的清屏函数
#ifdef _WIN32
	inline void CleanCLI() { system("cls"); }
#endif

#ifdef __linux__
	inline void CleanCLI() { system("clean"); }
#endif

}


//#endif // CLI_HPP_INCLUDED