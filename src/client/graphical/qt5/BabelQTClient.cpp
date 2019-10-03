/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** myQTWindow.cpp
*/

#include "BabelQTClient.hpp"

namespace Babel::Client
{
	BabelQTClient::BabelQTClient(Babel::Network::Socket &socket, Vector2<unsigned int> size, QWidget *parent) :
		window(size, parent),
		QObject(parent),
		_scrollBar(VERTICAL, this->window, {(int)size.x - 15, 0}, {15, size.y}),
		_logButton(this->window, "Connect", {10, 80}, {(unsigned)(std::string("Useless button").length()) * 10, 35}),
		_serverLoggedButton(this->window, "Not Connected to server", {10, (int)(size.y - 45)}, {(unsigned)(std::string("Not Connected to server").length()) * 10, 35}),
		_refreshConnectServer(this->window, "Refresh", {10, (int)(size.y - 95)}, {(unsigned)(std::string("Refresh").length()) * 10, 35}),
		_username(this->window, "Username", {10, 5}, {200, 30}),
		_password(this->window, "Password", {10, 40}, {100, 30}),
		_socket(socket)
	{
		this->_logButton.setCursor(POINTINGHANDCURSOR);
		this->_serverLoggedButton.setEnabled(false);
		QObject::connect(&this->_logButton, SIGNAL(released()), this, SLOT(sendConnectionLogs()));
		QObject::connect(&this->_refreshConnectServer, SIGNAL(released()), this, SLOT(refreshToKnowIfConnectedToServer()));

		if (this->_socket.isOpen())
			this->_serverLoggedButton.setText("Connected to server");
		else
			this->_serverLoggedButton.setText("Not Connected to server");
	}
/*
	void BabelQTClient::renameButton()
	{
		std::string new_text = this->_username.getPlainText() + ":" + this->_password.getPlainText();
		int lenght_new_text = new_text.length();

		if (lenght_new_text > 20) {
			lenght_new_text = 20;
			new_text = new_text.substr(0, 20) + "...";
		}
		if (lenght_new_text != this->_logButton.getText().length())
			this->_logButton.setGeometry(this->_logButton.getPosition().x, this->_logButton.getPosition().y, lenght_new_text * 10, this->_logButton.getSize().y);
		this->_logButton.setText(new_text);
	}
*/
	void BabelQTClient::sendConnectionLogs()
	{
		std::string username = this->_username.getPlainText();
		std::string password = this->_password.getPlainText();

	}

	void BabelQTClient::refreshToKnowIfConnectedToServer()
	{
		std::cout << this->_socket.isOpen() << std::endl;
		if (this->_socket.isOpen())
			this->_serverLoggedButton.setText("Connected to server");
		else
			this->_serverLoggedButton.setText("Not Connected to server");
	}

}