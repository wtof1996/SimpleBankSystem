#include "View\cli.hpp"

class Test
{
public:
	void Hello()
	{
		CLI::ShowMsg("Hello, World!");
	}

	void GoodBye()
	{
		CLI::ShowMsg("GoodBye");
	}

	void Hi()
	{
		CLI::ShowMsg("Hi");
	}
};

int main()
{
	Test a;
	CLI::ShowMenu(a, {  CLI::MenuItem<decltype(a)>("Hello", [](Test &o){o.Hello();} ),
                        CLI::MenuItem<decltype(a)>("GoodBye", [](Test &o){o.GoodBye(); }),
                        CLI::MenuItem<decltype(a)>("Hi", [](Test &o){o.Hi(); })


                    }, "Test");

	return 0;
}
