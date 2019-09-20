/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TextBox.cpp
*/

#include "TextBox.hpp"

namespace Babel
{
	TextBox::TextBox(const std::string &text = "", Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0}) :
		Widget(position, size),
		_text(text)
	{
	}

	void TextBox::setText(const std::string &text)
	{
		this->_text = text;
	}

	std::string TextBox::getText() const
	{
		return (this->_text);
	}
}