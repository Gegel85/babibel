/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** myQTWindow.cpp
*/

#include <winsock.h>
#include "../../../network/Socket.hpp"
#include "BabelQTClient.hpp"

namespace Babel::Client
{
	BabelQTClient::BabelQTClient(TcpClient &client, Vector2<unsigned int> size, QWidget *parent) :
		QObject(parent),
		window(size, parent),
		_lastError(this->window, "Last Error: ", {10, (int)(size.y - 35)}, {120, 35}),
		_logButton(this->window, "Log in", {10, 125}, {70, 35}),
		_logOutButton(this->window, "Log out", {105, 125}, {70, 35}),
		_serverLogged(this->window, "Not Connected to server", {10, (int)(size.y - 60)}, {250, 35}),
		_usernameLog(this->window, "Username:", {12, 5}, {90, 30}),
		_passwordLog(this->window, "Password:", {12, 60}, {90, 30}),
		_username(this->window, "", {10, 30}, {200, 30}),
		_password(this->window, "", {10, 85}, {100, 30}),
		_myIDTxtBx(this->window, "My ID: ", {10, 170}, {150, 35}),
		_idToCallTxtBx(this->window, "User ID:", {10, 190}, {70, 35}),
		_idToCallTyBx(this->window, "", {10, 220}, {50, 35}),
		_callButton(this->window, "Call", {65, 220}, {50, 35}),
		_stateOfCallTxtBx(this->window, "Not calling", {10, 255}, {110, 35}),
		_hangUpButton(this->window, "Hang up", {120, 220}, {70, 35}),
		_address(this->window, "", {300, 10}, {200, 35}),
		_port(this->window, "", {300, 50}, {200, 35}),
		_voiceConnectButton(this->window, "Voice connection", {300, 100}, {160, 35}),
		_client(client),
		_thread([this](){
			while (!this->_end) {
				this->_serverLogged.setText(this->_client.isConnected() ? "Connected to server" : this->_client.getLastError());
				this->_logButton.setEnabled(this->_client.isConnected() && this->_myID == -1 ? true : false);
				this->_logOutButton.setEnabled(this->_myID == -1 ? false : true);
				this->_stateOfCallTxtBx.setText(this->_client.isVoiceConnected() ? "Calling" : "Not calling");
				this->_hangUpButton.setEnabled(this->_client.isVoiceConnected()? true : false);
				if (this->_myID == -1) {
					this->_myIDTxtBx.setEnabled(false);
					this->_idToCallTxtBx.setEnabled(false);
					this->_idToCallTyBx.setEnabled(false);
					this->_callButton.setEnabled(false);
					this->_stateOfCallTxtBx.setEnabled(false);
					this->_hangUpButton.setEnabled(false);
					this->_myIDTxtBx.setText("My ID: ");
				} else {
					this->_myIDTxtBx.setEnabled(true);
					this->_idToCallTxtBx.setEnabled(true);
					this->_idToCallTyBx.setEnabled(true);
					this->_callButton.setEnabled(true);
					this->_stateOfCallTxtBx.setEnabled(true);
					this->_hangUpButton.setEnabled(true);
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
		this->_password.setEchoMode(PASSWORD);
		QObject::connect(&this->_logButton, SIGNAL(released()), this, SLOT(sendConnectionLogs()));
		QObject::connect(&this->_logOutButton, SIGNAL(released()), this, SLOT(logOut()));
		QObject::connect(&this->_callButton, SIGNAL(released()), this, SLOT(callButton()));
		QObject::connect(&this->_hangUpButton, SIGNAL(released()), this, SLOT(hangUp()));

		this->_address.setPlaceholderText("Adress");
		this->_port.setPlaceholderText("Port");
		QObject::connect(&this->_voiceConnectButton, SIGNAL(released()), this, SLOT(voiceConnect()));
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
		username.resize(32, '\0');
		password.resize(32, '\0');
		this->_client.sendPacketToServer(Network::Protocol::Opcode::LOGIN, username + password);
		std::pair<unsigned char, std::string> servResponse = this->_client.waitServerResponse();
		if (servResponse.first == Network::Protocol::Opcode::KO) {
			this->_lastError.setText("Last Error: Server refuse connection: " + Network::Protocol::ErrorReason::errorReasonToString(servResponse.second));
			return;
		}
		this->_myID = Network::Protocol::Packet::uint32FromByteString(servResponse.second);
	}

	void BabelQTClient::callButton()
	{
		std::string idToCall = this->_idToCallTyBx.getPlainText();

		if (idToCall.empty()) {
			this->_lastError.setText("Last Error: Cannot call, need valid ID");
			return;
		}
		this->_client.sendPacketToServer(Network::Protocol::Opcode::CALL, std::atoi(idToCall.c_str()));
		std::pair<unsigned char, std::string> servResponse = this->_client.waitServerResponse();
		if (servResponse.first == Network::Protocol::Opcode::KO || servResponse.first == Network::Protocol::Opcode::CALL_REFUSED) {
			this->_lastError.setText("Last Error: Server refuse call: " + Network::Protocol::ErrorReason::errorReasonToString(servResponse.second));
			return;
		}
		if (servResponse.first == Network::Protocol::Opcode::CALL_ACCEPTED)
			this->_stateOfCallTxtBx.setText("Calling");
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
	}

	void BabelQTClient::hangUp()
	{
		this->_client.disconnectFromVoice();
	}

	void BabelQTClient::voiceConnect()
	{
		std::string address = this->_address.getPlainText();
		std::string port = this->_port.getPlainText();
		int portNb = std::atoi(port.c_str());
		unsigned addr = inet_addr(address.c_str());

		if (addr == -1) {
			this->_lastError.setText("Last Error: cannot transform address with inet_addr");
			return;
		}

		this->_client.connectToVoice(addr, portNb);
	}
}