/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** Window.cpp
*/

#include "Window.hpp"

Window::Window() : QWidget()
{
	this->setFixedSize(300, 150);

	this->_button = new QPushButton("Pimp mon bouton !", this);

	this->_button->setFont(QFont("Comic Sans MS", 14));
	this->_button->setCursor(Qt::PointingHandCursor);
	this->_button->setIcon(QIcon("smile.png"));
	this->_button->move(60, 50);
}