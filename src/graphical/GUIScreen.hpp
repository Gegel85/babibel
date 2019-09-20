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
#include "Vector2.hpp"
#include "Widget.hpp"

namespace Babel
{
class GUIScreen {
public:
	GUIScreen(Vector2<unsigned int> size);

	Vector2<unsigned int> getSize();
	std::vector<Widget> getWidgets();
	Widget &getWidget(unsigned int id);
	void setFullscreen(bool fullscreen = true);
	void setSize(Vector2<unsigned int> size);
	unsigned int addWidget(std::shared_ptr<Widget> widget);
};
}

#endif