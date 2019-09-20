/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Widget.hpp
*/

#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <string>
#include "Vector2.hpp"

namespace Babel {
class GUIScreen;
class Widget
{
protected:
	Vector2<unsigned> _position;
	Vector2<unsigned> _size;
	bool _enabled;
	bool _visible;

public:
	Widget(Vector2<unsigned> position = {0, 0}, Vector2<unsigned> size = {0, 0});
	void setVisible(bool visible);
	void setEnabled(bool enabled);
	void setSize(Vector2<unsigned> size);
	void setPosition(Vector2<unsigned> position);
	bool getVisible() const;
	bool getEnabled() const;
	Vector2<unsigned> getSize() const;
	Vector2<unsigned> getPosition() const;
	virtual void draw(GUIScreen &screen) = 0;
	};
}

#endif