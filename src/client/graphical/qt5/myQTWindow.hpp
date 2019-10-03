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
#include <QObject>
#include "../GUIScreen.hpp"
#include "QTButton.hpp"
#include "QTScrollBar.hpp"
#include "QTWindow.hpp"
#include "QTTextBox.hpp"

namespace Babel::Client
{
	class myQTWindow : public QObject
	{
	Q_OBJECT
	public:
		myQTWindow(Vector2<unsigned int> size, QWidget *parent = nullptr);
		QTWindow window;

	public slots:
		void renameButton();

	private:
		QTScrollBar _scrollBar;
		QTButton _myButton;
		QTTextBox _IPTextBox;
		QTTextBox _portTextBox;
//		std::vector<Babel::QTButton &> _users;
	};
}

#endif