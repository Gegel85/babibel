/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Window.hpp
*/

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QApplication>
#include <QWidget>
#include <QPushButton>

class Window : public QWidget
{
	public:
		Window();
		~Window() = {
git 			if (this->_button) delete (this->_button);
		};

	private:
	QPushButton *_button;
};

#endif