/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Window.cpp
*/

#include "QTWindow.hpp"

namespace Babel {
	QTWindow::QTWindow(Vector2<unsigned int> size, QWidget *parent) :
		QMainWindow(parent),
		GUIScreen(),
		_size(size)
	{
		this->setFixedSize(size.x, size.y);
	}

	void QTWindow::setFullscreen(bool fullscren)
	{

	}

	void QTWindow::setSize(Vector2<unsigned> size)
	{
		this->setFixedSize(size.x, size.y);
	}

	void QTWindow::refresh()
	{

	}

	Vector2<unsigned> QTWindow::getSize() const
	{
		return (this->_size);
	}
}