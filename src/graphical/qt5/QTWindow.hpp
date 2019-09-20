/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTWindow.hpp
*/

#ifndef QTWINDOW_HPP
#define QTWINDOW_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>
#include "../GUIScreen.hpp"
#include "QTWidget.hpp"

namespace Babel {
	class QTWindow : public QMainWindow, public GUIScreen, public QTWidget {
		public:
			QTWindow(Vector2<unsigned int> size, QTWidget &parent);
			void setFullScreen(bool fullscren = true);
			void setSize(Vector2<unsigned> size);

		private:
	};
}
#endif