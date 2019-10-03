/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** QTApplication.cpp
*/

#include "QTApplication.hpp"

namespace BabelClient
{
	QTApplication::QTApplication(int &ac, char **av) :
		QApplication(ac, av)
		{
		}

	int QTApplication::launch()
	{
		return (this->exec());
	}
}