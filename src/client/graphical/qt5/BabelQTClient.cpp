/*
** EPITECH PROJECT, 2019
** CPP_babel_2019
** File description:
** myQTWindow.cpp
*/

#ifdef _WIN32
    #include <winsock.h>
#endif
#include "../../../network/Socket.hpp"
#include <sstream>
#include "BabelQTClient.hpp"

namespace Babel::Client
{
	BabelQTClient::BabelQTClient(TcpClient &client, Vector2<unsigned int> size, QWidget *parent) :
		QObject(parent),
		window(size, parent),
		_lastError(this->window, "", {10, (int)(size.y - 5)}, {120, 35}),
		_logButton(this->window, "Log in", {10, 125}, {70, 35}),
		_logOutButton(this->window, "Log out", {90, 125}, {70, 35}),
		_registerButton(this->window, "Register", {170, 125}, {80, 35}),
		_serverLogged(this->window, "Not Connected to server", {10, (int)(size.y - 60)}, {250, 35}),
		_usernameLog(this->window, "Username:", {12, 5}, {90, 30}),
		_passwordLog(this->window, "Password:", {12, 60}, {90, 30}),
		_username(this->window, "", {10, 30}, {240, 30}),
		_password(this->window, "", {10, 85}, {240, 30}),
		_myIDTxtBx(this->window, "My ID: ", {10, 170}, {150, 35}),
		_idToCallTxtBx(this->window, "User ID:", {10, 190}, {70, 35}),
		_idToCallTyBx(this->window, "", {10, 220}, {50, 35}),
		_callButton(this->window, "Call", {65, 220}, {50, 35}),
		_stateOfCallTxtBx(this->window, "Not calling", {10, 300}, {110, 35}),
		_hangUpButton(this->window, "Hang up", {120, 220}, {70, 35}),
		_addFriendButton(this->window, "Add friend", {10, 260}, {100, 35}),
		_removeFriendButton(this->window,"Remove friend", {115, 260}, {130, 35}),
		_address(this->window, "", {300, 10}, {200, 35}),
		_port(this->window, "", {300, 50}, {200, 35}),
		_voiceConnectButton(this->window, "Voice connection", {300, 100}, {160, 35}),
		_listOfUsers(this->window, "List of users:", {550, 10}, {250, size.y}),
		_client(client),
		_thread([this](){
			while (!this->_end) {
				try {
					this->_serverLogged.setText(this->_client.isConnected() ? "Connected to server" : this->_client.getLastError());
					this->_logButton.setEnabled(this->_client.isConnected() && this->_myID == -1 ? true : false);
					this->_logOutButton.setEnabled(this->_myID == -1 ? false : true);
					this->_registerButton.setEnabled(this->_client.isConnected() && this->_myID == -1 ? true : false);
					this->_voiceConnectButton.setEnabled(this->_client.isVoiceConnected() ? false : true);
					this->_stateOfCallTxtBx.setText(this->_client.isVoiceConnected() ? "Calling" : "Not calling");

					if (this->_myID == -1)
						this->desactivateWhenDisconnected();
					else 
						this->activateWhenConnected();
					if (this->_count == 0) {
						getUsers();
						this->_count = 15;
					}
					this->_count--;
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));

				} catch (std::exception &e) {
					std::cerr << e.what() << std::endl;
					std::string cpyEWhat = e.what();
					this->_lastError.setText("Last Error: " + cpyEWhat);
					this->logOut();
				}
			}
		})
	{
		this->window.setWindowTitle("Skipe");
		this->window.setWindowIcon("assets/images/skipe-logo.png");
		this->_logButton.setCursor(POINTINGHANDCURSOR);
		this->_logOutButton.setCursor(POINTINGHANDCURSOR);
		this->_callButton.setCursor(POINTINGHANDCURSOR);
		this->_hangUpButton.setCursor(POINTINGHANDCURSOR);
		this->_voiceConnectButton.setCursor(POINTINGHANDCURSOR);
		this->_password.setEchoMode(PASSWORD);

		QObject::connect(&this->_logButton, SIGNAL(released()), this, SLOT(sendConnectionLogs()));
		QObject::connect(&this->_logOutButton, SIGNAL(released()), this, SLOT(logOut()));
		QObject::connect(&this->_registerButton, SIGNAL(released()), this, SLOT(registerMe()));
		QObject::connect(&this->_callButton, SIGNAL(released()), this, SLOT(callButton()));
		QObject::connect(&this->_hangUpButton, SIGNAL(released()), this, SLOT(hangUp()));
		QObject::connect(&this->_addFriendButton, SIGNAL(released()), this, SLOT(addFriend()));
		QObject::connect(&this->_removeFriendButton, SIGNAL(released()), this, SLOT(removeFriend()));

		this->_address.setPlaceholderText("Adress");
		this->_port.setPlaceholderText("Port");

		this->_listOfUsers.setAlignment(TOP);
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
		this->_logButton.setEnabled(false);
		this->_registerButton.setEnabled(false);
		std::pair<unsigned char, std::string> servResponse = this->_client.waitServerResponse();
		if (servResponse.first == Network::Protocol::Opcode::KO) {
			this->_lastError.setText("Last Error: Server refuse connection: " + Network::Protocol::ErrorReason::errorReasonToString(servResponse.second));
			this->_logButton.setEnabled(true);
			this->_registerButton.setEnabled(false);
			return;
		}
		if (servResponse.first != Network::Protocol::Opcode::OK)
			return;
		this->_myID = Network::Protocol::Packet::uint32FromByteString(servResponse.second);
	}

	void BabelQTClient::callButton()
	{
		std::string idToCall = this->_idToCallTyBx.getPlainText();

		if (idToCall.empty()) {
			this->_lastError.setText("Last Error: Cannot call, need valid ID");
			return;
		}
		this->_client.sendPacketToServer(Network::Protocol::Opcode::CALL, std::stoi(idToCall));
		this->_callButton.setEnabled(false);
		std::pair<unsigned char, std::string> servResponse = this->_client.waitServerResponse();
		if (servResponse.first == Network::Protocol::Opcode::KO || servResponse.first == Network::Protocol::Opcode::CALL_REFUSED) {
			this->_lastError.setText("Last Error: Server refuse call: " + Network::Protocol::ErrorReason::errorReasonToString(servResponse.second));
			this->_callButton.setEnabled(true);
			return;
		}
		if (servResponse.first == Network::Protocol::Opcode::CALL_ACCEPTED)
			this->_stateOfCallTxtBx.setText("Calling");
	}

	void BabelQTClient::logOut()
	{
		if (!this->_client.isConnected())
			return;
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

		if (addr == 0) {
			this->_lastError.setText("Last Error: cannot transform address with inet_addr");
			return;
		}

		this->_client.connectToVoice(addr, portNb);
	}

	void BabelQTClient::addFriend()
	{
		std::string idToAdd = this->_idToCallTyBx.getPlainText();

		if (idToAdd.empty()) {
			this->_lastError.setText("Last Error: Cannot add friend, need valid ID");
			return;
		}
		this->_client.sendPacketToServer(Network::Protocol::Opcode::ADD_FRIEND, std::stoi(idToAdd));
		this->_addFriendButton.setEnabled(false);
		this->_removeFriendButton.setEnabled(false);
		std::pair<unsigned char, std::string> servResponse = this->_client.waitServerResponse();
		if (servResponse.first == Network::Protocol::Opcode::KO) {
			this->_lastError.setText("Last Error: Server add friend: " + Network::Protocol::ErrorReason::errorReasonToString(servResponse.second));
			this->_addFriendButton.setEnabled(true);
			this->_removeFriendButton.setEnabled(true);
			return;
		}
		this->_addFriendButton.setEnabled(true);
		this->_removeFriendButton.setEnabled(true);
	}

	void BabelQTClient::removeFriend()
	{
		std::string idToRemove = this->_idToCallTyBx.getPlainText();

		if (idToRemove.empty()) {
			this->_lastError.setText("Last Error: Cannot remove friend, need valid ID");
			return;
		}
		this->_client.sendPacketToServer(Network::Protocol::Opcode::REMOVE_FRIEND, std::stoi(idToRemove));
		this->_addFriendButton.setEnabled(false);
		this->_removeFriendButton.setEnabled(false);
		std::pair<unsigned char, std::string> servResponse = this->_client.waitServerResponse();
		if (servResponse.first == Network::Protocol::Opcode::KO) {
			this->_lastError.setText("Last Error: Server add friend: " + Network::Protocol::ErrorReason::errorReasonToString(servResponse.second));
			this->_addFriendButton.setEnabled(true);
			this->_removeFriendButton.setEnabled(true);
			return;
		}
		this->_addFriendButton.setEnabled(true);
		this->_removeFriendButton.setEnabled(true);
	}

	void BabelQTClient::registerMe()
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
		this->_client.sendPacketToServer(Network::Protocol::Opcode::REGISTER, username + password);
		this->_logButton.setEnabled(false);
		this->_registerButton.setEnabled(false);
		std::pair<unsigned char, std::string> servResponse = this->_client.waitServerResponse();
		if (servResponse.first == Network::Protocol::Opcode::KO) {
			this->_lastError.setText("Last Error: Server refuse connection: " + Network::Protocol::ErrorReason::errorReasonToString(servResponse.second));
			this->_logButton.setEnabled(true);
			this->_registerButton.setEnabled(false);
			return;
		}
		if (!servResponse.first == Network::Protocol::Opcode::OK)
			return;
		this->sendConnectionLogs();
	}

	void BabelQTClient::getUsers()
	{
		if (!this->_client.isConnected())
			return;
		this->_client.sendPacketToServer(Network::Protocol::Opcode::GET_FRIENDS, "");

		auto servResponse = this->_client.waitServerResponse();

		if (servResponse.first == Network::Protocol::Opcode::KO) {
			this->_lastError.setText("Last Error: Can't get users: " + Network::Protocol::ErrorReason::errorReasonToString(servResponse.second));
			return;
		}
		if (!servResponse.first == Network::Protocol::Opcode::OK)
			return;
		if (servResponse.second.length() % 4 != 0) {
			this->_lastError.setText("Last Error: Bad response from server");
			return;
		}

		std::vector<unsigned> usersID;
		for (long unsigned int i = 0; i < servResponse.second.length(); i += 4) {
			if (servResponse.second.empty())
				usersID.push_back(0);
			else
				usersID.push_back(Network::Protocol::Packet::uint32FromByteString(servResponse.second.substr(i,4)));
		}

		std::vector<std::string> usersName;
		for (long unsigned int i = 0; i < usersID.size(); i++) {
			this->_client.sendPacketToServer(Network::Protocol::Opcode::GET_USER_INFOS, usersID.at(i));
			servResponse = this->_client.waitServerResponse();
			if (servResponse.first == Network::Protocol::Opcode::KO) {
				usersName.push_back("Error: " + Network::Protocol::ErrorReason::errorReasonToString(servResponse.second));
				continue;
			}
			if (servResponse.first != Network::Protocol::Opcode::OK)
				usersName.push_back("");
			else
				usersName.push_back(servResponse.second.substr(4, 32));
		}

		std::stringstream users;
		users << "List of users:";
		for (long unsigned int i = 0; i < usersID.size(); i++)
			users << std::endl << usersID[i] << ": " << usersName[i];
		this->_listOfUsers.setText(users.str());
	}

	void BabelQTClient::activateWhenConnected()
	{
		this->_myIDTxtBx.setEnabled(true);
		this->_idToCallTxtBx.setEnabled(true);
		this->_idToCallTyBx.setEnabled(true);
		this->_callButton.setEnabled(true);
		this->_myIDTxtBx.setText("My ID: " + std::to_string(this->_myID));
		this->_addFriendButton.setEnabled(true);
		this->_removeFriendButton.setEnabled(true);
	}

	void BabelQTClient::desactivateWhenDisconnected()
	{
		this->_myIDTxtBx.setEnabled(false);
		this->_idToCallTxtBx.setEnabled(false);
		this->_idToCallTyBx.setEnabled(false);
		this->_callButton.setEnabled(false);
		this->_myIDTxtBx.setText("My ID: ");
		this->_addFriendButton.setEnabled(false);
		this->_removeFriendButton.setEnabled(false);
	}
}