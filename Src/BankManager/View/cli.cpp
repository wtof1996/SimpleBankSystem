#include "cli.hpp"
#include <algorithm>

namespace CLI
{
	void ShowMsg(const string &msg, bool nl)
	{
		cout.clear();

		cout << convert(msg);
		if(nl) cout << endl;
	}

	void ShowBoxMsg(const string &msg, const char border)
	{
		ShowMsg(string(msg.size() + 8, border));
		ShowMsg(string(4, ' ') + msg);
		ShowMsg(string(msg.size() + 8, border));
	}


	string GetInput(const string &direction)
	{
		ShowMsg(direction);
		string ret;
		cin.clear();
		getline(cin, ret);

		return ret;
	}

	string GetInput(const string &direction, const boost::regex &checker)
	{
		boost::smatch match;
		string input;
		do{
			input = GetInput(direction);
		} while (!boost::regex_match(input, match, checker) && (ShowMsg(RetryDirection), true) );

		return input;
	}

	size_t ShowChooseList(const string &msg, std::initializer_list<string> list)
	{
		ShowMsg(msg);
		BOOST_LOG_TRIVIAL(info) << "ChooseList Showed";
		BOOST_LOG_TRIVIAL(info) << "Message is:" << msg;


		cout << endl;

		size_t index = 0;
		for (auto &i : list) {

			ShowMsg("[" + std::to_string(++index) + "] : " + i);

			BOOST_LOG_TRIVIAL(debug) << "Item Index:" << index - 1;
			BOOST_LOG_TRIVIAL(debug) << "Item Name:" << i;
		}

		size_t input;
		do{
			input = to_number(GetInput(MenuInputDirection + "[1-" + std::to_string(list.size()) + "] : ",
				boost::regex("^\\d+")));

		} while ((input <= 0 || input > list.size()) && (ShowMsg(RetryDirection), true));

		BOOST_LOG_TRIVIAL(info) << "User choosed No." << input;

		return input;
	}
}