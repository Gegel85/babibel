/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTWindow.hpp
*/

#ifndef QTWINDOW_HPP
#define QTWINDOW_HPP

#include <QMainWindow>
#include <QIcon> 
#include "../GUIScreen.hpp"

namespace Babel::Client {
	class QTWindow : public QMainWindow, public GUIScreen {
	public:
		QTWindow(Vector2<unsigned int> size, QWidget *parent = nullptr);
		void setFullscreen(bool fullscren = true) override;
		void setSize(Vector2<unsigned> size) override;
		bool refresh() override;
		Vector2<unsigned int> getSize() const override;
		void setWindowTitle(std::string title);
		void setWindowIcon(std::string path);

	private:
		Vector2<unsigned> _size;
	};
}
#endif