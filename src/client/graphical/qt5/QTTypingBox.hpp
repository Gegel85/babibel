/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTTypingBox.hpp
*/

#ifndef QTTYPINGBOX_HPP
#define QTTYPINGBOX_HPP

//#include <QTextEdit>
#include <QLineEdit> 
#include "../Widget/TypingBox.hpp"

namespace Babel::Client
{
	enum QTEchoModes {
		NORMAL = QLineEdit::Normal,
		NOECHO = QLineEdit::NoEcho,
		PASSWORD = QLineEdit::Password,
		PASSWORDECHOONEDIT = QLineEdit::PasswordEchoOnEdit
	};

	class QTTypingBox : public TypingBox, public QLineEdit {
	public:
		QTTypingBox(QWidget &parent, const std::string &text = "", Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0});
		void draw(GUIScreen &screen) override;
		std::string getPlainText() const;
		void setEnabled(bool enabled);
		void setPlaceholderText(std::string text);
		void setEchoMode(QTEchoModes echomode);
	};
}
#endif