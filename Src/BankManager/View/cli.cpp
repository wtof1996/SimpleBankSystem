#include "cli.hpp"
#include <algorithm>

namespace CLI
{
	void ShowMsg(const string &msg)
	{
		cout.clear();
		cout << msg << endl;
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
}
