/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTButton.cpp
*/

#include "QTButton.hpp"

namespace BabelClient {
	QTButton::QTButton(QWidget &parent, const std::string &text, Vector2<int> position, Vector2<unsigned> size) :
		Button(text, position, size),
		QPushButton(QString(this->_text.c_str()), &parent)
	{
		this->setGeometry(this->_position.x, this->_position.y, this->_size.x, this->_size.y);
	}

	void QTButton::setText(const std::string &text)
	{
		this->_text = text;
		QPushButton::setText(QString(this->_text.c_str()));
	}

	bool QTButton::isClicked() const
	{
		return (this->isDown());
	}

	void QTButton::draw(GUIScreen &screen)
	{
		
	}
}