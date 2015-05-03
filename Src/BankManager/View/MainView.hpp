#pragma once

#include "cli.hpp"
#include "BaseView.hpp"
#include "..\Controller\DataController.hpp"
#include "..\Controller\IOController.hpp"

//#ifndef MAINVIEW_HPP_INCLUDED
//#define MAINVIEW_HPP_INCLUDED

namespace view
{
	class MainView : public BaseView
	{
	public:
		controller::DataController Data;
		const std::string WelcomeInfo = "欢迎使用银行业务系统!";

		MainView()
		{

			controller::io::ReadXMLFiles(Data);

			CLI::CleanCLI();
			CLI::ShowMsg(WelcomeInfo);

			BOOST_LOG_TRIVIAL(info) << "Main View loaded";
		}

		~MainView()
		{
			controller::io::WriteXMLFiles(Data);
		}

		void Login();
		void Exit() { Loop = false;  }
		void Show() override;
	};
}


//#endif //MAINVIEW_HPP_INCLUDED
