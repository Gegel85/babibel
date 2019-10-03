/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TypingBox.cpp
*/

#include "TypingBox.hpp"

namespace Babel::Client {
	TypingBox::TypingBox(const std::string &text, Vector2<int> position, Vector2<unsigned> size) :
		Widget(position, size),
		_text(text)
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