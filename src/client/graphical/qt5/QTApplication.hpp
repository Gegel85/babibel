/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTApplication.hpp
*/

#ifndef QTAPPLICATION_HPP
#define QTAPPLICATION_HPP

#include <QtWidgets/QApplication>

namespace Babel::Client
{
	class QTApplication : public QApplication {
	public:
		QTApplication(int &ac, char **av);
		int launch();
	};
}

#endif