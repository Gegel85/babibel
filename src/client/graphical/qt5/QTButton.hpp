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
#include "QTCursors.hpp"


//#include "Cursors.hpp"

namespace Babel::Client
{
	class QTButton : public Button, public QPushButton {
	public:
		QTButton(QWidget &parent, const std::string &text = "", Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0});

		void setText(const std::string &text) override;
		void draw(GUIScreen &screen) override;
		bool isClicked() const override;
		void setCursor(cursors cursor);
		void setClickable(bool clickable);
		void setEnabled(bool enabled);

	private:
		bool _clickable;
	};
}

#endif