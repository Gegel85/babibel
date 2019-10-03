/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTTextBox.hpp
*/

#ifndef QTTEXTBOX_HPP
#define QTTEXTBOX_HPP

#include <QTextEdit>
#include "../Widget/TextBox.hpp"

namespace Babel::Client
{
	class QTTextBox : public TextBox, public QTextEdit {
	public:
		QTTextBox(QWidget &parent, const std::string &text = "", Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0});
		void draw(GUIScreen &screen) override;
		std::string getPlainText() const;
	};
}
#endif