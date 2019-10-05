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
		_lastError(this->window, "Last Error: ", {10, (int)(size.y - 35)}, {120, 35}),
		_logButton(this->window, "Log in", {10, 125}, {70, 35}),
		_logOutButton(this->window, "Log out", {105, 125}, {70, 35}),
		_serverLogged(this->window, "Not Connected to server", {10, (int)(size.y - 60)}, {250, 35}),
		_usernameLog(this->window, "Username:", {12, 5}, {90, 30}),
		_passwordLog(this->window, "Password:", {12, 60}, {90, 30}),
		_username(this->window, "", {10, 30}, {200, 30}),
		_password(this->window, "", {10, 85}, {100, 30}),
		_myIDTxtBx(this->window, "My ID: ", {10, 170}, {150, 35}),
		_idToCallTxtBx(this->window, "ID to call:", {10, 190}, {110, 35}),
		_idToCallTyBx(this->window, "", {10, 220}, {35, 35}),
		_callButton(this->window, "Call", {50, 220}, {50, 35}),
		_stateOfCallTxtBx(this->window, "Not calling", {10, 255}, {110, 35}),
		_client(client),
		_thread([this](){
			while (!this->_end) {
				this->_serverLogged.setText(this->_client.isConnected() ? "Connected to server" : this->_client.getLastError());
				this->_logButton.setEnabled(this->_client.isConnected() && this->_myID == -1 ? true : false);
				this->_logOutButton.setEnabled(this->_myID == -1 ? false : true);
				if (this->_myID == -1) {
					this->_myIDTxtBx.setEnabled(false);
					this->_idToCallTxtBx.setEnabled(false);
					this->_idToCallTyBx.setEnabled(false);
					this->_callButton.setEnabled(false);
					this->_stateOfCallTxtBx.setEnabled(false);
				} else {
					this->_myIDTxtBx.setEnabled(true);
					this->_idToCallTxtBx.setEnabled(true);
					this->_idToCallTyBx.setEnabled(true);
					this->_callButton.setEnabled(true);
					this->_stateOfCallTxtBx.setEnabled(true);
					this->_stateOfCallTxtBx.setText(this->_client.isVoiceConnected() ? "Calling" : "Not calling");
					this->_myIDTxtBx.setText("My ID: " + std::to_string(this->_myID));
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			}
		})
	{
		this->window.setWindowTitle("Skipe");
		this->window.setWindowIcon("Assets/Images/skipe-logo.png");
		this->_logButton.setCursor(POINTINGHANDCURSOR);
		this->_callButton.setCursor(POINTINGHANDCURSOR);
		QObject::connect(&this->_logButton, SIGNAL(released()), this, SLOT(sendConnectionLogs()));
		QObject::connect(&this->_logOutButton, SIGNAL(released()), this, SLOT(logOut()));
		QObject::connect(&this->_callButton, SIGNAL(released()), this, SLOT(callButton()));
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
		if (username.empty() || password.empty()) {
			this->_lastError.setText("Last Error: Need valid username and password");
			return;
		}
		if (username.length() > 32 || password.length() > 32) {
			this->_lastError.setText("Last Error: Username and password need to be 32 long each");
			return;
		}
		username.resize(16, '\0');
		password.resize(16, '\0');
		this->_client.sendPacketToServer(Network::Protocol::Opcode::LOGIN, username + password);
		std::pair<unsigned char, std::string> servResponse = this->_client.waitServerResponse();
		if (servResponse.first == Network::Protocol::Opcode::KO) {
			this->_lastError.setText("Last Error: Server refuse connection: " + Network::Protocol::ErrorReason::errorReasonToString(servResponse.second));
			return;
		}
		this->_myID = std::atoi(servResponse.second.c_str());
		// this->_logOutButton.setEnabled(true);
		// this->_logButton.setEnabled(false);
	}

	void BabelQTClient::callButton()
	{
		std::string idToCall = this->_idToCallTyBx.getPlainText();

		if (idToCall.empty()) {
			this->_lastError.setText("Last Error: Cannot call, need valid ID");
			return;
		}
	}

	void BabelQTClient::logOut()
	{
		this->_client.sendPacketToServer(Network::Protocol::Opcode::LOGOUT, "");
		std::pair<unsigned char, std::string> servResponse = this->_client.waitServerResponse();
		if (servResponse.first == Network::Protocol::Opcode::KO) {
			this->_lastError.setText("Last Error: Server refuse deconnection: " + Network::Protocol::ErrorReason::errorReasonToString(servResponse.second));
			return;
		}
		this->_myID = -1;
		// this->_logOutButton.setEnabled(false);
		// this->_logButton.setEnabled(true);
	}
}