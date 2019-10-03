/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTTextBox.cpp
*/

#include "QTTextBox.hpp"

namespace BabelClient
{
	QTTextBox::QTTextBox(QWidget &parent, const std::string &text, Vector2<int> position, Vector2<unsigned> size) :
		QTextEdit(QString(text.c_str()), &parent),
		TextBox(text, position, size)
	{
		this->setGeometry(this->_position.x, this->_position.y, this->_size.x, this->_size.y);
	}

	void QTTextBox::draw(GUIScreen &screen)
	{
	}

	std::string QTTextBox::getPlainText() const
	{
		return (this->toPlainText().toStdString());
	}
}