/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTWidget.hpp
*/

#ifndef QTWIDGET_HPP
#define QTWIDGET_HPP

#include "../Widget/Widget.hpp"

namespace Babel {
	class QTWidget : public Widget {
		public:
			QTWidget(QTWidget &parent, Vector2<unsigned> position = {0, 0}, Vector2<unsigned> size = {0, 0});

		private:
			QWidget *_parent = nullptr;
	};
}

#endif