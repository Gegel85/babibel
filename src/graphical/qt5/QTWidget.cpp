/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTWidget.cpp
*/

#include "QTWidget.hpp"

namespace Babel {
	QTWidget::QTWidget(QTWidget &parent, Vector2<unsigned> position = {0, 0}, Vector2<unsigned> size = {0, 0}) :
		Widget(position, size),
		_parent(&parent)
	{
	}

}