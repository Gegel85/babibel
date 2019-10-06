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
	class QTApplication : public QApplication
	{
	Q_OBJECT
	public:
		QTApplication(int &ac, char **av);
		int launch();

		bool notify(QObject * receiver, QEvent * event) override {
			try {
				return QCoreApplication::notify(receiver, event);
			} catch (std::exception &e) {
				std::cerr << e.what() << std::endl;
			}
				return false;
			};
		};
}

#endif