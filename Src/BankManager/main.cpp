#include <stdexcept>

#include <boost\log\trivial.hpp>
#include <boost\scope_exit.hpp>

#include "Helper.hpp"
#include "Model\Config.hpp"

#include "View\cli.hpp"
#include "View\MainView.hpp"

using std::string;

int main(int argc, char* argv[])
{

try
{
	helper::InitLog();
	Config::get();

	BOOST_LOG_TRIVIAL(info) << "Program Start";

	Config::get().ParseOption(argc, argv);

	view::MainView main;
	while (main.Loop) main.Show();

}
catch(std::exception &e)
{
    BOOST_LOG_TRIVIAL(fatal) << e.what();

	CLI::ShowBoxMsg("程序发生异常，即将退出");
}

BOOST_SCOPE_EXIT(void){
	BOOST_LOG_TRIVIAL(info) << "Program stopped";
}BOOST_SCOPE_EXIT_END
	return 0;

}
