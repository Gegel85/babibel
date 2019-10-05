/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTTypingBox.hpp
*/

#ifndef QTTYPINGBOX_HPP
#define QTTYPINGBOX_HPP

#include <QTextEdit>
#include "../Widget/TypingBox.hpp"

namespace Babel::Client
{
	class QTTypingBox : public TypingBox, public QTextEdit {
	public:
		QTTypingBox(QWidget &parent, const std::string &text = "", Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0});
		void draw(GUIScreen &screen) override;
		std::string getPlainText() const;
		void setEnabled(bool enabled);
	};
}
#endif