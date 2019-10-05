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
#include "QTLayout.hpp"
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
		void logOut();
		void callButton();

	private:
		QTLayout _usersLayout;
//		QTScrollBar _scrollBar;

		QTTextBox _lastError;
		QTButton _logButton;
		QTButton _logOutButton;
		QTTextBox _serverLogged;

		QTTextBox _usernameLog;
		QTTextBox _passwordLog;
		QTTypingBox _username;
		QTTypingBox _password;

		int _myID = -1;
		QTTextBox _myIDTxtBx;
		QTTextBox _idToCallTxtBx;
		QTTypingBox _idToCallTyBx;
		QTButton _callButton;
		QTTextBox _stateOfCallTxtBx;

		bool _end = false;
		TcpClient &_client;
		std::thread _thread;
	};
}

#endif