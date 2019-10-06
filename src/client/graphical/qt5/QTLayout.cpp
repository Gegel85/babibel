/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTLayout.cpp
*/

#include <QRect>
#include "QTLayout.hpp"

namespace Babel::Client
{
	QTLayout::QTLayout(QTBoxLayoutDirection orientation, QWidget &parent, Vector2<int> position, Vector2<unsigned> size) :
		Widget(position, size),
		QBoxLayout((QBoxLayout::Direction)orientation, &parent)
	{
		this->setGeometry(QRect(position.x, position.y, size.x, size.y));
	}

	void QTLayout::draw(GUIScreen &screen)
	{
		(void)screen;
	}

	void QTLayout::setEnabled(bool enabled)
	{
		QBoxLayout::setEnabled(enabled);
	}
/*
	void QTLayout::addWidget(QWidget *wigdet)
	{
		QBoxLayout::addWidget(widget);
	}*/
}