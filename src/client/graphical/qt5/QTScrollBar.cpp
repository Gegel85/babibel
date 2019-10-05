/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTScrollBar.cpp
*/

#include "QTScrollBar.hpp"

namespace Babel::Client
{
	QTScrollBar::QTScrollBar(QTOrientation orientation, QWidget &parent, Vector2<int> position, Vector2<unsigned> size) :
		QScrollBar((Qt::Orientation)orientation, &parent),
		ScrollBar(position, size)
	{
		this->setGeometry(position.x, position.y, size.x, size.y);
	}

	void QTScrollBar::draw(GUIScreen &screen)
	{
		(void)screen;
	}
}