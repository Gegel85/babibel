/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** ScrollBar.hpp
*/

#ifndef SCROLLBAR_HPP
#define SCORLLBAR_HPP

#include "Widget.hpp"

namespace Babel {
class ScrollBar : public Widget
{
protected:
	unsigned _maxValue;
	unsigned _frameSize;

public:
	ScrollBar(Vector2<unsigned> position = {0, 0}, Vector2<unsigned> size = {0, 0});
	void setMaxValue(unsigned maxValue);
	void setFrameSize(unsigned frameSize);
	double getValue();
};
}

#endif