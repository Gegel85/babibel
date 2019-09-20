/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TypingBox.cpp
*/

#include "TypingBox.hpp"

namespace Babel {
	TypingBox::TypingBox(const std::string &text = "", Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0}) :
		Widget(position, size),
		_size(size)
	{
	}

	void TypingBox::setText(const std::string &text)
	{
		this->_text = text;
	}

	std::string TypingBox::getText() const
	{
		return (this->_text);
	}
}