/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTLayout.hpp
*/

#ifndef QTLAYOUT_HPP
#define QTLAYOUT_HPP

#include <QBoxLayout>
#include "../Widget/Widget.hpp"

namespace Babel::Client
{
	enum QTBoxLayoutDirection
	{
		LEFTTORIGHT = QBoxLayout::LeftToRight,
		RIGHTTOLEFT = QBoxLayout::RightToLeft,
		TOPTOBOTTOM = QBoxLayout::TopToBottom,
		BOTTOMTOTOP = QBoxLayout::BottomToTop
	};

	class QTLayout : public Widget, public QBoxLayout
	{
	public:
		QTLayout(QTBoxLayoutDirection dir, QWidget &parent, Vector2<int> position = {0, 0}, Vector2<unsigned> size = {0, 0});
		void draw(GUIScreen &screen) override;
		void setEnabled(bool enabled);
//		void addWidget(Widget &widget);
	};
}
#endif