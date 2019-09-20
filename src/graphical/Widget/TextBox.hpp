/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TextBox.hpp
*/

#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "Widget.hpp"

namespace Babel {
class TextBox : public Widget
{
protected:
	std::string _text;
	Vector2<unsigned> _position;
	Vector2<unsigned> _size;

public:
	TextBox(const std::string &text = "", Vector2<unsigned> position = {0, 0}, Vector2<unsigned> size = {0, 0});
	void setText(const std::string &text);
	std::string getText() const;
};
}
#endif