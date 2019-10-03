/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Button.hpp
*/

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Widget.hpp"

namespace Babel::Client
{
	class Button : public Widget {
	protected:
		std::string _text;
		bool _clicked;

	public:
		Button(const std::string &text = "", Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0});
		virtual void setText(const std::string &text) = 0;
		std::string getText() const;
		virtual bool isClicked() const;
	};
}
#endif