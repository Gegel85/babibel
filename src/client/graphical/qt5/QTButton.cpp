/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTButton.cpp
*/

#include "QTButton.hpp"

namespace Babel::Client {
	QTButton::QTButton(QWidget &parent, const std::string &text, Vector2<int> position, Vector2<unsigned> size) :
		Button(text, position, size),
		QPushButton(QString(this->_text.c_str()), &parent),
		_clickable(true)
	{
		this->setGeometry(this->_position.x, this->_position.y, this->_size.x, this->_size.y);
	}

	void QTButton::setText(const std::string &text)
	{
		this->_text = text;
		QPushButton::setText(QString(this->_text.c_str()));
		this->setGeometry(this->_position.x, this->_position.y, (unsigned)(text.length()) * 10, this->_size.y);
	}

	bool QTButton::isClicked() const
	{
		return (this->isDown());
	}

	void QTButton::draw(GUIScreen &screen)
	{
		(void)screen;
	}

	void QTButton::setCursor(QTCursors cursor)
	{
		QPushButton::setCursor(Qt::CursorShape(cursor));
	}

	void QTButton::setClickable(bool clickable)
	{
		if (clickable)
			QPushButton::setFlat(false);
		else
			QPushButton::setFlat(true);
		this->_clickable = clickable;
	}

	void QTButton::setEnabled(bool enabled)
	{
		QPushButton::setEnabled(enabled);
	}
}