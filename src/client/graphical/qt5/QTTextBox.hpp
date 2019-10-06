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
	enum QTAlignment {
		LEFT = Qt::AlignLeft,
		RIGHT = Qt::AlignRight,
		HCENTER = Qt::AlignHCenter,
		JUSTIFY = Qt::AlignJustify,
		TOP = Qt::AlignTop,
		BOTTOM = Qt::AlignBottom,
		VCENTER = Qt::AlignVCenter,
		BASELINE = Qt::AlignBaseline,
	};

	class QTTextBox : public TextBox, public QLabel
	{
	public:
		QTTextBox(QWidget &parent, const std::string &text = "", Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0});
		void setText(const std::string &text);
		void draw(GUIScreen &screen) override;
		void setEnabled(bool enabled);
		void setAlignment(QTAlignment alignement);
	};
}
#endif