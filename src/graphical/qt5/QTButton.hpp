/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTButton.hpp
*/

#ifndef QTBUTTON_HPP
#define QTBUTOON_HPP

#include <QtWidgets/QPushButton>
#include "../Widget/Button.hpp"
#include "Cursors.hpp"

namespace Babel {
class QTButton : Button
{
	public:
		QTButton(std::string text = "", Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0} , QWidget *parent = nullptr);

		// void setText(const std::string &text);
		// const std::string getText();

		// void setToolTip(const std::string &toolTip);
		// const std::string getToolTip();

		// void setIcon(const std::string &path);
		// void setIcon(const QIcon &otherIcon);
		// const QIcon getIcon();

		// void setFont(std::string fontName, unsigned size);

		// void setCursor(enum cursors);
		// const enum cursors getCursor();

	private:
		QWidget *_parent;
};
}

#endif