/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTScrollBar.hpp
*/

#ifndef QTSCROLLBAR_HPP
#define	QTSCROLLBAR_HPP

#include <QScrollBar>
#include "../Widget/ScrollBar.hpp"

namespace BabelClient
{
	enum QTOrientation {
		HORIZONTAL = Qt::Horizontal,
		VERTICAL =	Qt::Vertical
	};

	class QTScrollBar : public QScrollBar, public ScrollBar {
	public:
		QTScrollBar(QTOrientation orientation, QWidget &parent, Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0});
		void draw(GUIScreen &screen) override;
	};
}

#endif