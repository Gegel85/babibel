/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Picture.hpp
*/

#ifndef PICTURE_HPP
#define PICTURE_HPP

#include "Widget.hpp"

namespace Babel {
class Button : public Widget
{
protected:
	std::string _text;

public:
	Button(const std::string &text = "", Vector2<unsigned> position = {0, 0}, Vector2<unsigned> size = {0, 0});
	void setText(const std::string &text);
	std::string getText() const;
	bool isClicked();
};
}

#endif