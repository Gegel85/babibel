/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** ScrollBar.cpp
*/

#include "ScrollBar.hpp"

namespace Babel {
	ScrollBar::ScrollBar(Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0}) :
		Widget(position, size),
		_maxValue(0),
		_frameSize(0),
		_barPos(0)
	{
	}

	void ScrollBar::setMaximum(unsigned maxValue)
	{
		this->_maximum = maxValue;
	}

	void ScrollBar::setFrameSize(unsigned frameSize)
	{
		this->_frameSize = frameSize;
	}

	double ScrollBar::getValue() const
	{
		return (this->_barPos / this->_position.y) ;
	}
}