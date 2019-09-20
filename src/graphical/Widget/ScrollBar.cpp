/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** ScrollBar.cpp
*/

#include "ScrollBar.hpp"

namespace Babel {
	ScrollBar::ScrollBar(Vector2<unsigned> position = {0, 0}, Vector2<unsigned> size = {0, 0})
	:	_position(position),
		_size(size)
	{
	}

	void ScrollBar::setMaxValue(unsigned maxValue) {
		this->_maxValue = maxValue;
	}

	void ScrollBar::setFrameSize(unsigned frameSize) {
		this->_frameSize = frameSize;
	}

	double ScrollBar::getValue() const{
		return (this->_value);
	}
}