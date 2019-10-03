/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** GUIScreen.cpp
*/

#include <algorithm>
#include "GUIScreen.hpp"

namespace Babel::Client
{
	unsigned int GUIScreen::addWidget(std::shared_ptr<Widget> widget)
	{
		std::vector<std::shared_ptr<Widget>>::iterator it = std::find(this->_widgets.begin(), this->_widgets.end(), nullptr);

		if (it == this->_widgets.end()) {
			this->_widgets.push_back(widget);
			return this->_widgets.size() - 1;
		}
		it->swap(widget);
		return it - this->_widgets.begin();
	}

	std::vector<std::shared_ptr<Widget>> GUIScreen::getWidgets()
	{
		return this->_widgets;
	}

	Widget &GUIScreen::getWidget(unsigned int id)
	{
		return *this->_widgets.at(id);
	}
}