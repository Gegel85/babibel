/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Widget.cpp
*/

#include "Widget.hpp"

namespace Babel {
	Widget::Widget(Vector2<unsigned> position = {0, 0}, Vector2<unsigned> size = {0, 0})
	:	_position(position),
		_size(size),
		_enabled(true),
		_visible(true)
	{
	}

	void Widget::setVisible(bool visible) {
		this->_visible = visible;
	}

	void Widget::setEnabled(bool enabled) {
		this->_enabled = enabled;
	}

	void Widget::setSize(Vector2<unsigned> size) {
		this->_size = size;
	}

	void Widget::setPosition(Vector2<unsigned> position)
	{
		this->_position = position;
	}

	bool Widget::getVisible () const {
		return (this->_visible);
	}

	bool Widget::getEnabled() const {
		return (this->_enabled);
	}

	Vector2<unsigned> Widget::getSize() const {
		return (this->_size);
	}

	Vector2<unsigned> Widget::getPosition() const {
		return (this->_position);
	}
}