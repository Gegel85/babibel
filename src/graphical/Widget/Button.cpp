/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Button.cpp
*/

#include "Button.hpp"

namespace Babel
{
	Button::Button(const std::string &text = "", Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0}) :
		Widget(position, size),
		_text(text),
		_clicked(false)
	{
	}

	void Button::setText(const std::string &text)
	{
		this->_text = text;
	}

	std::string Button::getText() const
	{
		return (this->_text);
	}

	bool Button::isClicked() const
	{
		return (this->_clicked);
	}
}