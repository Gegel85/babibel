/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Button.cpp
*/

#include "Button.hpp"

namespace BabelClient
{
	Button::Button(const std::string &text, Vector2<int> position, Vector2<unsigned> size) :
		Widget(position, size),
		_text(text),
		_clicked(false)
	{
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