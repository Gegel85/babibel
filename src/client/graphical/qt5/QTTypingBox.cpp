/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTTypingBox.cpp
*/

#include "QTTypingBox.hpp"

namespace Babel::Client
{
	QTTypingBox::QTTypingBox(QWidget &parent, const std::string &text, Vector2<int> position, Vector2<unsigned> size) :
		QTextEdit(QString(text.c_str()), &parent),
		TypingBox(text, position, size)
	{
		this->setGeometry(this->_position.x, this->_position.y, this->_size.x, this->_size.y);
	}

	void QTTypingBox::draw(GUIScreen &screen)
	{
	}

	std::string QTTypingBox::getPlainText() const
	{
		return (this->toPlainText().toStdString());
	}
}