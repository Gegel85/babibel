/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** myQTWindow.cpp
*/

#include "myQTWindow.hpp"

namespace Babel
{
	myQTWindow::myQTWindow(Vector2<unsigned int> size, QWidget *parent) :
		window(size, parent),
		_scrollBar(Babel::VERTICAL, this->window, {(int)size.x - 15, 0}, {15, size.y}),
		_myButton(this->window, "Useless button", {100, 100}, {100, 50})
	{
		QObject::connect(&this->_myButton, SIGNAL(released()), &this->window, SLOT(renameButton())); //&Bable::QTWindow::renameButton(Babel::QTButton));
	}

	void myQTWindow::renameButton()
	{
		this->_myButton.setText("Renamed button");
	}

}