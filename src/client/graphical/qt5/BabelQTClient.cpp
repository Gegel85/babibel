/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** myQTWindow.cpp
*/

#include "BabelQTClient.hpp"

namespace Babel::Client
{
	BabelQTClient::BabelQTClient(TcpClient &client, Vector2<unsigned int> size, QWidget *parent) :
		QObject(parent),
		window(size, parent),
		_scrollBar(VERTICAL, this->window, {(int)size.x - 15, 0}, {15, size.y}),
		_logButton(this->window, "Connect", {10, 125}, {70, 35}),
		_serverLogged(this->window, "Not Connected to server", {10, (int)(size.y - 45)}, {250, 35}),
		_usernameLog(this->window, "Username", {12, 5}, {80, 30}),
		_passwordLog(this->window, "Password", {12, 55}, {80, 30}),
		_username(this->window, "", {10, 30}, {200, 30}),
		_password(this->window, "", {10, 85}, {100, 30}),
		_client(client),
		_thread([this](){
			while (!this->_end) {
				this->_serverLogged.setText(this->_client.isConnected() ? "Connected to server" : this->_client.getLastError());
				this->_logButton.setEnabled(this->_client.isConnected());
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		})
	{
		this->window.setWindowTitle("Skipe");
		this->window.setWindowIcon("Assets/Images/skipe-logo.png");
		this->_logButton.setCursor(POINTINGHANDCURSOR);
		QObject::connect(&this->_logButton, SIGNAL(released()), this, SLOT(sendConnectionLogs()));
	}

	BabelQTClient::~BabelQTClient()
	{
		this->_end = true;
		if (this->_thread.joinable())
			this->_thread.join();
	}
	void BabelQTClient::sendConnectionLogs()
	{
		std::string username = this->_username.getPlainText();
		std::string password = this->_password.getPlainText();
	}
}