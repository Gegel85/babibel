/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTTextBox.hpp
*/

#ifndef QTTEXTBOX_HPP
#define QTTEXTBOX_HPP

#include <QLabel>
#include "../Widget/TextBox.hpp"

namespace Babel::Client
{
	class QTTextBox : public TextBox, public QLabel
	{
	public:
		QTTextBox(QWidget &parent, const std::string &text = "", Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0});
		void setText(const std::string &text);
		void draw(GUIScreen &screen) override;
	};
}
#endif