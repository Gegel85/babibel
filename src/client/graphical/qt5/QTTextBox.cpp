/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTTextBox.cpp
*/

#include "QTTextBox.hpp"

namespace Babel::Client
{
	QTTextBox::QTTextBox(QWidget &parent, const std::string &text, Vector2<int> position, Vector2<unsigned> size) :
		TextBox(text, position, size),
		QLabel(QString(this->_text.c_str()), &parent)
	{
		this->setGeometry(this->_position.x, this->_position.y, this->_size.x, this->_size.y);
	}

	void QTTextBox::draw(GUIScreen &screen)
	{
		(void)screen;
	}

	void QTTextBox::setText(const std::string &text)
	{
		this->_text = text;
		QLabel::setText(QString(this->_text.c_str()));
		this->setGeometry(this->_position.x, this->_position.y, (unsigned)(text.length()) * 10, this->_size.y);

	}

}