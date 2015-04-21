#include "View\cli.hpp"
#include <string>
#include "Controller\Helper.hpp"

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
		std::string cipher = helper::AES_128_EncryptHex("呵呵");
		CLI::ShowMsg(cipher);
		std::string plain = helper::AES_128_DecryptHex(cipher);


		CLI::ShowMsg(plain);

		std::cout << plain.size() << std::endl;
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
