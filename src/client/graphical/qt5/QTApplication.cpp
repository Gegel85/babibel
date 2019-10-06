/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTApplication.cpp
*/

#include <iostream>
#include "QTApplication.hpp"

namespace Babel::Client
{
	QTApplication::QTApplication(int &ac, char **av) :
		QApplication(ac, av)
		{
		}

	int QTApplication::launch()
	{
		try {
			this->exec();
		} catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
			return (EXIT_FAILURE);
		}
		return (EXIT_SUCCESS);
	}
}