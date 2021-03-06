/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Window.cpp
*/

#include "QTWindow.hpp"

namespace Babel::Client {
	QTWindow::QTWindow(Vector2<unsigned int> size, QWidget *parent) :
		QMainWindow(parent),
		GUIScreen(),
		_size(size)
	{
		this->setFixedSize(size.x, size.y);
	}

	void QTWindow::setFullscreen(bool fullscren)
	{
		(void)fullscren;
	}

	void QTWindow::setSize(Vector2<unsigned> size)
	{
		this->setFixedSize(size.x, size.y);
	}

	bool QTWindow::refresh()
	{
		return (true);
	}

	Vector2<unsigned> QTWindow::getSize() const
	{
		return (this->_size);
	}

	void QTWindow::setWindowTitle(std::string title)
	{
		QMainWindow::setWindowTitle(QString(title.c_str()));
	}

	void QTWindow::setWindowIcon(std::string path)
	{
		QIcon icon(QString(path.c_str()));
		QMainWindow::setWindowIcon(icon);
	}
}