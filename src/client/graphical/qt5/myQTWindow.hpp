/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** myQTWindow.hpp
*/

#ifndef MYQTWINDOW_HPP
#define MYQTWINDOW_HPP

#include <vector>
#include <QWidget>
#include "../GUIScreen.hpp"
#include "QTButton.hpp"
#include "QTScrollBar.hpp"
#include "QTWindow.hpp"

namespace Babel
{
	class myQTWindow
	{
	public:
		myQTWindow(Vector2<unsigned int> size, QWidget *parent = nullptr);
		QTWindow window;

	public slots:
		void renameButton();

	private:
		QTScrollBar _scrollBar;
		QTButton _myButton;
//		std::vector<Babel::QTButton &> _users;
	};
}

#endif