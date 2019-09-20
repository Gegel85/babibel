/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTButton.cpp
*/

#include "QTButton.hpp"

namespace Babel {
	QTButton::QTButton(std::string text = "", Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0} , QTWidget &parent) :
		Button(text, position, size),
		_parent(parent)
	{
		this->_button = new QPushButton(this->_text, this->_parent);
		this->_button.setGeometry(this->_position.x, this->_position.y, this->_size.x, this->_size.y);
	}

	QTButton::~QTButton() {
		if (this->_button)
			delete (this->_button);
	}

	QTButton::setText(std::string text) {
		this->_text = text;
		this->_button.setText(QString(this->_text));
	}

	bool QTButton::isClicked() const
	{
		return (this->_button.isDown());
	}
}