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
#include <iostream>
#include "../../../network/Socket.hpp"
#include "../../../network/Protocol.hpp"
#include "../../../network/SocketExceptions.hpp"
#include "../GUIScreen.hpp"
#include "QTButton.hpp"
#include "QTScrollBar.hpp"
#include "QTWindow.hpp"
#include "QTTypingBox.hpp"
#include "QTTextBox.hpp"
#include "QTCursors.hpp"
#include "../../TcpClient.hpp"

namespace Babel::Client
{
	class BabelQTClient : public QObject
	{
	Q_OBJECT
	public:
		BabelQTClient(TcpClient &client, Vector2<unsigned int> size, QWidget *parent = nullptr);
		~BabelQTClient();
		QTWindow window;

	public slots:
		void sendConnectionLogs();

	private:
		QTScrollBar _scrollBar;

		QTButton _logButton;
		QTTextBox _serverLogged;

		QTTextBox _usernameLog;
		QTTextBox _passwordLog;
		QTTypingBox _username;
		QTTypingBox _password;

		bool _end = false;
		TcpClient &_client;
		std::thread _thread;
	};
}

#endif