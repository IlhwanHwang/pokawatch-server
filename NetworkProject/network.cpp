#pragma comment(lib, "ws2_32.lib")

#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <cstdio>
#include <string>
#include "network.h"
#include "protocol.h"
#include "game.h"
#include "key.h"
#include "ai.h"

#include "draw.h"
#include "resource.h"

#define SCENARIO_SINGLE
//#define SCENARIO_SERVER

using namespace std;

SOCKET Network::hServSock;														// Server socket variable of server/client side
SOCKET Network::hClntSock[CLIENT_NUM_MAX];										// Client socket variable of server side
SOCKET Network::hSocket;														// Client socket variable of client side
SOCKADDR_IN Network::servAddr;													// Server address variable of server/client side
SOCKADDR_IN Network::clntAddr[CLIENT_NUM_MAX];									// Client address variable of server side
int Network::szClntAddr[CLIENT_NUM_MAX];											// Client address size variable of server side
char Network::messageToClient[MESSAGE_T0_CLIENT_SIZE];							// Message buffer of server side
char Network::messageFromClient[CLIENT_NUM_MAX][MESSAGE_TO_SERVER_SIZE*3];			// Message buffer of server side
char Network::messageToServer[MESSAGE_TO_SERVER_SIZE*3];							// Message buffer of client side
int Network::mode;																// determine server/ client/ nothing
int Network::characterSelection[UNIT_NUM_MAX];												// Information of selection of charactor(dep)
char Network::gameStart[3];														// game started? not(N) start(G)
int Network::command[(UNIT_NUM_MAX)/2];															// selected command of client side
char * Network::serverIpArg;

void Network::ErrorHandling(char *message)  //Error handling routine
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void Network::makeServerSocket() // Server Socket making routine
{
	string portString = PORT_STRING;
	char * port = (char*)portString.c_str();

	WSADATA wsaData;

	// Load Winsock 2.2 DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error!");

	// Create Server socket
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET) ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(port));

	// Allocate address of server
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) ErrorHandling("bind() error");

	// Listeing for client
	if (listen(hServSock, CLIENT_NUM_MAX) == SOCKET_ERROR) ErrorHandling("listen() error");

	u_long iMode = 1;
	if (ioctlsocket(hServSock, FIONBIO, &iMode)) {
		cout << "IOCTLSOCKET error" << endl;
	}
	else {
		cout << "IOCTLSOCKET succeeded" << endl;
	}

	printf("Server Listen\n");
}

void Network::acceptClient()
{
	//accept client's request
	for (int i = 0; i < CLIENT_NUM_MAX; i++) szClntAddr[i] = sizeof(clntAddr[i]);
	for (int i = 0; i < CLIENT_NUM_MAX; i++)
	{
		while (1) {
			hClntSock[i] = accept(hServSock, (SOCKADDR*)&clntAddr[i], &szClntAddr[i]);
			if (hClntSock[i] != INVALID_SOCKET)
				break;
		}
	}
	// give their team
	for (int i = 0; i < CLIENT_NUM_MAX; i++)
	{
		char teamInfo[8];
		teamInfo[0] = i + '0';
		teamInfo[1] = '\0';

		int WhatDo = send(hClntSock[i], teamInfo, sizeof(teamInfo) - 1, 0);
	}
}

void Network::sendToClient(char *messageToClient, size_t length) // Message sending routine of client side
{
	for (int i = 0; i < CLIENT_NUM_MAX; i++)
	{
		int WhatDo = send(hClntSock[i], messageToClient, length, 0);
	}
}

void Network::recieveFromClient() // Message recieving routine of server side
{
	for (int i = 0; i < CLIENT_NUM_MAX; i++)
	{
		while (1) {
			int strLen = recv(hClntSock[i], messageFromClient[i], MESSAGE_TO_SERVER_SIZE - 1, 0);
			if (strLen != -1) {
				messageFromClient[i][strLen] = '\0';
				break;
			}
		}
	}
}

void Network::closeServerConnection() //server cosing routine
{
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		closesocket(hClntSock[i]);
	}
	WSACleanup();
}

void Network::makeClientSocket() // Client socket making routine
{
	string servIpString = serverIpArg;
	string portNumString = PORT_STRING;
	char * servIp = (char*)servIpString.c_str();
	char * portNum = (char*)portNumString.c_str();

	WSADATA wsaData;

	// Load WinSocket 2.2 DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup(), error");
	}

	// Create client socket 
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("hSocketet(), error");
	}

	printf("Socket made\n");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(servIp);
	servAddr.sin_port = htons(atoi(portNum));

	// Request for connetion
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("Connect() error");
	}
}


void Network::getProtocolDataFromServer() // Message receving from server
{
	int strLen;
	//recieving data
	strLen = recv(hSocket, messageToClient, sizeof(messageToClient) - 1, 0);
	if (strLen == -1)
	{
		Network::ErrorHandling("read() error");
	}
	messageToClient[strLen] = '\0';

	/* For Client the only information given is protocol data below
	* because this game is made for the purpose of AI game
	* every AI should make decision based on this protocol data */

	// for checking recieving data appropriately protocol data is printed

	protocol_data *newData = (protocol_data*)messageToClient;

}

void Network::recieveGameStart()
{
	int strLen;
	strLen = recv(hSocket, gameStart, sizeof(gameStart) - 1, 0); // data recieving
	if (strLen == -1)
	{
		Network::ErrorHandling("read() error");
	}
	gameStart[strLen] = '\0';
}

void Network::closeClientConnection()
{
	//connection closing
	closesocket(hSocket);
	WSACleanup();
}

void Network::sendToServer(char message[]) // message sending routine
{
	send(hSocket, message, sizeof(message), 0);
}

void Network::turn() // turn routine
{
	if (Network::getGameStart()[0] == GAME_START_CHAR && !Game::isEnded()) {
		cout << "Turn reached" << endl;
#ifdef SCENARIO_SERVER
		for (int i = 0; i < CLIENT_NUM_MAX; i++)
		{
			int strLen = recv(hClntSock[i], messageFromClient[i], MESSAGE_TO_SERVER_SIZE - 1, 0);
			if (strLen == -1) {
				cout << "Client " << i << " didn't respond" << endl;
			}
			else {
				messageFromClient[i][strLen] = '\0';
				cout << "Client " << i << " had a message: " << messageFromClient[i] << endl;
			}
		}
#endif
#ifdef SCENARIO_SINGLE
		for (int i = 0; i < CLIENT_NUM_MAX; i++) {
			for (int j = 0; j < UNIT_PER_TEAM; j++) {
				Unit& u = Game::getUnit(UNIT_PER_TEAM * i + j);
				protocol_command c;

				if (u.isAlive()) {
					bool own = u.getX() > POINT_X1 && u.getX() < POINT_X2;
					if (!own)
						c = ((u.getX() - (POINT_X1 + POINT_X2) / 2) < 0 ? COMMAND_MOVE_RIGHT : COMMAND_MOVE_LEFT);
					else {
						if (rand() % 4)
							c = direction_to_movecommand(random_direction());
						else
							c = direction_to_attackcommand(random_direction());
					}
				}
				else {
					c = dep_to_spawncommand(random_dep());
				}
				messageFromClient[i][j] = (char)c + '0';
			}
		}
		
		char c_command;
		protocol_direction d = DIRECTION_NULL;
		protocol_command c = COMMAND_NULL;
		Unit& u = Game::getUnit(0);

		if (Key::keyCheckOn('w')) d = DIRECTION_UP;
		if (Key::keyCheckOn('a')) d = DIRECTION_LEFT;
		if (Key::keyCheckOn('s')) d = DIRECTION_DOWN;
		if (Key::keyCheckOn('d')) d = DIRECTION_RIGHT;

		if (Key::keyCheckOn('j')) c = direction_to_attackcommand(d);
		else if (Key::keyCheckOn('k')) c = direction_to_skillcommand(d);
		else c = direction_to_movecommand(d);

		if (Key::keyCheckOn('1')) c = COMMAND_SPAWN_CSE;
		if (Key::keyCheckOn('2')) c = COMMAND_SPAWN_PHYS;
		if (Key::keyCheckOn('3')) c = COMMAND_SPAWN_ME;
		if (Key::keyCheckOn('4')) c = COMMAND_SPAWN_LIFE;
		if (Key::keyCheckOn('5')) c = COMMAND_SPAWN_CHEM;

		messageFromClient[0][0] = (char)c + '0';
		
#endif
		Game::turn();
		sendToClient((char*)Game::getProtocolPointer(), sizeof(protocol_data));
	}
}

void Network::update() // frame turn routine
{
}

void Network::init(char * argv) {
	for (int i = 0; i < UNIT_NUM_MAX; i++) characterSelection[i] = 0;
	gameStart[0] = 'N';
	gameStart[1] = '0' + TEAM_NULL;
	gameStart[2] = '\0';

	char gameStartMessage[2];
	gameStartMessage[0] = GAME_START_CHAR;
	gameStartMessage[1] = '\0';

	setMode(MODE_SERVER);														// set as server

#ifdef SCENARIO_SERVER
	makeServerSocket();														// server socket made
	printf("Socket made \n");
	acceptClient();															// accepting client
	printf("Client accepted\n");
	recieveFromClient();														// recieve spawn information

	for (int i = 0; i < UNIT_NUM_MAX / 2; i++)												// spawn units
	{
		for (int j = 0; j < CLIENT_NUM_MAX; j++)
		{
			Game::getUnit(((UNIT_NUM_MAX) / 2) * j + i).spawn((protocol_dep)(messageFromClient[j][i] - '0'));
		}
	}

	Game::release();
	sendToClient(gameStartMessage, 2);
#endif 

#ifdef SCENARIO_SINGLE
	for (int i = 0; i < UNIT_PER_TEAM; i++)												// spawn units
	{
		for (int j = 0; j < CLIENT_NUM_MAX; j++)
		{
			Game::getUnit(UNIT_PER_TEAM * j + i).spawn((protocol_dep)(rand() % DEP_CHEM + 1));
		}
	}
	Game::getUnit(0).spawn(DEP_CHEM);
	Game::getUnit(0).setHero(true);
	Game::release();
#endif

	setGameStart(0, GAME_START_CHAR);
}