/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TextBox.cpp
*/

#include "TextBox.hpp"

namespace Babel {
	TextBox::TextBox(const std::string &text = "", Vector2<unsigned> position = {0, 0}, Vector2<unsigned> size = {0, 0})
	:	_text(text),
		_position(position),
		_size(size)
	{
	}

	void TextBox::setText(const std::string &text) {
		this->_text = text;
	}

	std::string TextBox::getText() const {
		return (this->_text);
	}
}