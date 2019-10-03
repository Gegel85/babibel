/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Widget.hpp
*/

#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <string>
#include "../../data/Vector2.hpp"

namespace Babel::Client
{
	class GUIScreen;
	class Widget {
	protected:
		Vector2<int> _position;
		Vector2<unsigned> _size;
		bool _enabled;
		bool _visible;

	public:
		Widget(Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0});
		void setVisible(bool visible);
		void setEnabled(bool enabled);
		void setSize(Vector2<unsigned> size);
		void setPosition(Vector2<int> position);
		bool getVisible() const;
		bool getEnabled() const;
		Vector2<unsigned> getSize() const;
		Vector2<int> getPosition() const;
		virtual void draw(GUIScreen &screen) = 0;
	};
}

#endif