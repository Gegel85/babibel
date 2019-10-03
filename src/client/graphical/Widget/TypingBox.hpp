/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** TypingBox.hpp
*/

#ifndef TYPINGBOX_HPP
#define TYPINGBOX_HPP

#include "Widget.hpp"

namespace Babel::Client
{
	class TypingBox : public Widget {
	protected:
		std::string _text;

	public:
		TypingBox(const std::string &text = "", Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0});
		void setText(const std::string &text);
		std::string getText() const;
	};
}

#endif