#pragma once

//#ifndef BASEVIEW_HPP_INCLUDED
//#define BASEVIEW_HPP_INCLUDED

namespace view
{
	class  BaseView
	{
	public:
		BaseView() = default;
		virtual ~BaseView() = default;

		virtual void Show() = 0;
		bool Loop = true;

	};
}





//#endif // BASEVIEW_HPP_INCLUDED
