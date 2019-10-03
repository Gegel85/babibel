/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** myQTWindow.cpp
*/

#include "BabelQTClient.hpp"

namespace Babel::Client
{
	BabelQTClient::BabelQTClient(Babel::Network::Socket &socket, const std::string &lastError, Vector2<unsigned int> size, QWidget *parent) :
		window(size, parent),
		QObject(parent),
		_scrollBar(VERTICAL, this->window, {(int)size.x - 15, 0}, {15, size.y}),
		_logButton(this->window, "Connect", {10, 80}, {70, 35}),
		_serverLogged(this->window, "Not Connected to server", {10, (int)(size.y - 45)}, {250, 35}),
		_username(this->window, "Username", {10, 5}, {200, 30}),
		_password(this->window, "Password", {10, 40}, {100, 30}),
		_socket(socket),
		_thread([this](){
			while (!this->_end) {
				this->_serverLogged.setText(this->_socket.isOpen() ? "Connected to server" : this->_lastError);
				this->_logButton.setEnabled(this->_socket.isOpen() ? true : false);
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}),
		_lastError(lastError)
	{
		this->_logButton.setCursor(POINTINGHANDCURSOR);
//		this->_serverLoggedButton.setEnabled(false);
		QObject::connect(&this->_logButton, SIGNAL(released()), this, SLOT(sendConnectionLogs()));
	}

	BabelQTClient::~BabelQTClient()
	{
		if (this->_thread.joinable())
			this->_thread.join();
		this->_end = true;
	}
	void BabelQTClient::sendConnectionLogs()
	{
		std::string username = this->_username.getPlainText();
		std::string password = this->_password.getPlainText();

	}
}