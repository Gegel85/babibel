/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** myQTWindow.cpp
*/

#include "myQTWindow.hpp"

namespace Babel::Client
{
	myQTWindow::myQTWindow(Vector2<unsigned int> size, QWidget *parent) :
		window(size, parent),
		QObject(parent),
		_scrollBar(VERTICAL, this->window, {(int)size.x - 15, 0}, {15, size.y}),
		_myButton(this->window, "Connect", {10, 80}, {(unsigned)(std::string("Useless button").length()) * 5, 35}),
		_IPTextBox(this->window, "IP Adress", {10, 5}, {200, 30}),
		_portTextBox(this->window, "Port", {10, 40}, {100, 30})
	{
		QObject::connect(&this->_myButton, SIGNAL(released()), this, SLOT(renameButton())); //&Bable::QTWindow::renameButton(Babel::QTButton));
	}

	void myQTWindow::renameButton()
	{
		std::string new_text = this->_IPTextBox.getPlainText() + ":" + this->_portTextBox.getPlainText();
		int lenght_new_text = new_text.length();

		if (lenght_new_text > 20) {
			lenght_new_text = 20;
			new_text = new_text.substr(0, 20) + "...";
		}
		if (lenght_new_text != this->_myButton.getText().length())
			this->_myButton.setGeometry(this->_myButton.getPosition().x, this->_myButton.getPosition().y, lenght_new_text * 10, this->_myButton.getSize().y);
		this->_myButton.setText(new_text);
	}

}