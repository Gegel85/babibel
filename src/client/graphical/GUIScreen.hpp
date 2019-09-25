/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** GUIScreen.hpp
*/

#ifndef GUISCREEN_HPP
#define GUISCREEN_HPP

#include <vector>
#include <memory>
#include "../data/Vector2.hpp"
#include "Widget/Widget.hpp"

namespace Babel
{
	class GUIScreen {
	protected:
		std::vector<std::shared_ptr<Widget>> _widgets;
	public:
		GUIScreen() = default;

		std::vector<std::shared_ptr<Widget>> getWidgets();
		Widget &getWidget(unsigned int id);
		unsigned int addWidget(std::shared_ptr<Widget> widget);
		virtual Vector2<unsigned int> getSize() const = 0;
		virtual void setFullscreen(bool fullscreen = true) = 0;
		virtual void setSize(Vector2<unsigned int> size) = 0;
		virtual bool refresh() = 0;
	};
}

#endif