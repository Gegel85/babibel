/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTWindow.hpp
*/

#ifndef QTWINDOW_HPP
#define QTWINDOW_HPP

#include <QWidget>
#include <QtWidgets/QMainWindow>
#include "../GUIScreen.hpp"

namespace Babel {
	class QTWindow : public QMainWindow, public GUIScreen {
		public:
			QTWindow(Vector2<unsigned int> size, QWidget &parent);
			void setFullscreen(bool fullscren = true) override;
			void setSize(Vector2<unsigned> size) override;

		private:
	};
}
#endif