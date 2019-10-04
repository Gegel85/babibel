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
		TypingBox(text, position, size),
		QTextEdit(QString(text.c_str()), &parent)
	{
		this->setGeometry(this->_position.x, this->_position.y, this->_size.x, this->_size.y);
	}

	void QTTypingBox::draw(GUIScreen &screen)
	{
		(void)screen;
	}

	std::string QTTypingBox::getPlainText() const
	{
		return (this->toPlainText().toStdString());
	}
}