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

using namespace std;

SOCKET Network::hServSock;												// of server
SOCKET Network::hClntSock[UNIT_NUM_MAX];									// of server
SOCKET Network::hSocket;													// of client
SOCKADDR_IN Network::servAddr;											// of server/client
SOCKADDR_IN Network::clntAddr[UNIT_NUM_MAX];								// of server
int Network::szClntAddr[UNIT_NUM_MAX];									// of server
char Network::messageToClient[MESSAGE_T0_CLIENT_SIZE];					// of server
char Network::messageFromClient[UNIT_NUM_MAX][MESSAGE_TO_SERVER_SIZE];	// of server
char Network::messageToServer[MESSAGE_TO_SERVER_SIZE];					// of client
int Network::mode;														//determine server/ client/ nothing
int Network::characterSelection;
char Network::gameStart[2];												// not(N) start(G)
int Network::command;

void Network::ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void Network::communicate(int cnt)
{
	printf("와우 오졌따리");
}

void Network::makeServerSocket()
{
	string portString = PORT_STRING;
	char * port = (char*)portString.c_str();

	WSADATA wsaData;

	// Load Winsock 2.2 DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error!");

	// 서버 소켓 생성
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET) ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(port));

	// 소켓에 주소 할당
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) ErrorHandling("bind() error");

	// 연결 요청 대기 상태
	if (listen(hServSock, UNIT_NUM_MAX) == SOCKET_ERROR) ErrorHandling("listen() error");

	printf("listen");
}

void Network::acceptClient()
{
	// 연결 요청 수락
	for (int i = 0; i < UNIT_NUM_MAX; i++) szClntAddr[i] = sizeof(clntAddr[i]);
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		hClntSock[i] = accept(hServSock, (SOCKADDR*)&clntAddr[i], &szClntAddr[i]);
		if (hClntSock[i] == INVALID_SOCKET) ErrorHandling("accept() error");
	}
}

void Network::sendToClient(char *messageToClient)
{
	

	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		int WhatDo = send(hClntSock[i], messageToClient, MESSAGE_T0_CLIENT_SIZE-1, 0);
		printf("THIS : %d\n", WhatDo);
	}
}

void Network::recieveFromClient()
{
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		int strLen = recv(hClntSock[i], messageFromClient[i], MESSAGE_TO_SERVER_SIZE-1, 0);
		messageFromClient[i][strLen] = '\0';
		printf("%s WAS COMMAND %c%c\n", messageFromClient[i], messageFromClient[i][0], messageFromClient[i][1]);

	}
}

void Network::closeServerConnection()
{
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		closesocket(hClntSock[i]);
	}
	WSACleanup();
}

void Network::makeClientSocket()
{
	string servIpString = SERV_IP_STRING;
	string portNumString = PORT_STRING;
	char * servIp = (char*)servIpString.c_str();
	char * portNum = (char*)portNumString.c_str();

	WSADATA wsaData;

	// Load WinSocket 2.2 DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup(), error");
	}

	// 서버 접속을 위한 소켓 생성
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("hSocketet(), error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(servIp);
	servAddr.sin_port = htons(atoi(portNum));

	// 서버로 연결 요청
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("Connect() error");
	}

}


void Network::getProtocolDataFromServer()
{
	int strLen;
	// 데이터 수신
	strLen = recv(hSocket, messageToClient, sizeof(messageToClient) - 1, 0);
	if (strLen == -1)
	{
		Network::ErrorHandling("read() error");
	}
	messageToClient[strLen] = '\0';
	printf("strLen : %d MESS : %d", strLen, MESSAGE_T0_CLIENT_SIZE); // 32 // 33

	for (int i = 0; i < strLen; i = i + 4)
	{
		printf("%d ", ((int*)messageToClient)[i]);
	}
	printf("\n");
}

void Network::recieveGameStart()
{
	int strLen;
	// 데이터 수신
	strLen = recv(hSocket, gameStart, sizeof(gameStart) - 1, 0);
	if (strLen == -1)
	{
		Network::ErrorHandling("read() error");
	}
	gameStart[strLen] = '\0';
}

void Network::closeClientConnection()
{
	// 연결 종료
	closesocket(hSocket);
	WSACleanup();
}

void Network::sendToServer(char message[])
{
	send(hSocket, message, sizeof(message), 0);
}

void Network::turn()
{
	if (Network::getMode() == MODE_CLIENT && Network::getGameStart()[0] == GAME_START_CHAR)
	{
		Network::sendToServer((char *)(to_string(Network::getCommand())).c_str());
		printf("command : %d was sent\n", Network::getCommand());
		Network::getProtocolDataFromServer();
	}
	if (Network::getMode() == MODE_SERVER && Network::getGameStart()[0] == GAME_START_CHAR)
	{
		char *protocolToSend;
		Network::recieveFromClient();	// recieve data;
		protocolToSend = (char*)(Game::getProtocolPointer());
		protocolToSend[MESSAGE_T0_CLIENT_SIZE] = '\0';
		
		Network::sendToClient(protocolToSend);

		for (int i = 0; i < MESSAGE_T0_CLIENT_SIZE; i=i+4)
		{
			//printf("%d ", ((int*)protocolToSend)[i]);
		}
		printf("\n");
	}
}


void Network::update()
{

	if (Network::getMode() == MODE_NOTHING && Key::keyCheckPressed(MODE_SERVER_KEY))
	{
		char gameStartMessage[2];
		gameStartMessage[0] = GAME_START_CHAR;
		gameStartMessage[1] = '\0';

		Network::setMode(MODE_SERVER);
		printf("mode- server chosn\n");
		Network::makeServerSocket();
		printf("소켓 만들어 짐\n");
		Network::acceptClient();
		printf("accepted\n");

		Network::recieveFromClient();			// recieve spawn
		printf("SELECTED : \n");

	
		for (int i = 0; i < UNIT_NUM_MAX; i++)
		{
			if (Game::getUnit(i).getTeam() == TEAM_POSTECH)
			{
				Game::getUnit(i).spawn((protocol_dep)(atoi(messageFromClient[i])));
				printf("%d! ", atoi(messageFromClient[i]));
			}
			else if ((Game::getUnit(i).getTeam() == TEAM_KAIST))
			{
				Game::getUnit(i).spawn((protocol_dep)(atoi(messageFromClient[i])));
				printf("%d! ", atoi(messageFromClient[i]));
			}
		}
	
		Game::release();
		Network::sendToClient(gameStartMessage);		// start to clients
		Network::setGameStart(0, GAME_START_CHAR);		// game started
	}

	if (Network::getMode() == MODE_NOTHING && Key::keyCheckPressed(MODE_CLIENT_KEY))
	{
		Network::setMode(MODE_CLIENT);
		printf("mode- client chose\n");
		Network::makeClientSocket();
		Network::connectToServer();
	}// if invalid input, we should give error message

	if (Network::getMode() == MODE_SERVER || Network::getMode() == MODE_CLIENT)					// 
	{
		if (Network::getMode() == MODE_CLIENT && Network::getCharacterSelection() == 0 && Key::keyCheckPressed('1'))
		{
			Network::setCharacterSelection(DEP_CSE);
			Network::sendToServer((char *)(to_string(Network::getCharacterSelection())).c_str());
			printf("character chosen : %d", Network::getCharacterSelection());
			Network::recieveGameStart();
			printf("GameStart!");
		}
		if (Network::getMode() == MODE_CLIENT && Network::getCharacterSelection() == 0 && Key::keyCheckPressed('2'))
		{
			Network::setCharacterSelection(DEP_PHYS);
			Network::sendToServer((char *)(to_string(Network::getCharacterSelection())).c_str());
			printf("character chosen : %d", Network::getCharacterSelection());
			Network::recieveGameStart();
			printf("GameStart!");
		}
		if (Network::getMode() == MODE_CLIENT && Network::getCharacterSelection() == 0 && Key::keyCheckPressed('3'))
		{
			Network::setCharacterSelection(DEP_LIFE);
			Network::sendToServer((char *)(to_string(Network::getCharacterSelection())).c_str());
			printf("character chosen : %d", Network::getCharacterSelection());
			Network::recieveGameStart();
			printf("GameStart!");
		}

		if (Network::getMode() == MODE_CLIENT && Network::getCharacterSelection() == 0 && Key::keyCheckPressed('4'))
		{
			Network::setCharacterSelection(DEP_ME);
			Network::sendToServer((char *)(to_string(Network::getCharacterSelection())).c_str());
			printf("character chosen : %d", Network::getCharacterSelection());
			Network::recieveGameStart();
			printf("GameStart!");
		}

		if (Network::getMode() == MODE_CLIENT && Network::getCharacterSelection() == 0 && Key::keyCheckPressed('5'))
		{
			Network::setCharacterSelection(DEP_CHEM);
			Network::sendToServer((char *)(to_string(Network::getCharacterSelection())).c_str());
			printf("character chosen : %d\n", Network::getCharacterSelection());
			Network::recieveGameStart();
			printf("GameStart!\n");
		}

		if (Network::getMode() == MODE_CLIENT && Network::getCharacterSelection() > 0 && Network::getGameStart()[0] == GAME_START_CHAR)
		{
			if (Key::keyCheckPressed('d')) Network::setCommand(COMMAND_MOVE_RIGHT);
			if (Key::keyCheckPressed('w')) Network::setCommand(COMMAND_MOVE_UP);
			if (Key::keyCheckPressed('a')) Network::setCommand(COMMAND_MOVE_LEFT);
			if (Key::keyCheckPressed('s')) Network::setCommand(COMMAND_MOVE_DOWN);

			if (Key::keyCheckPressed('d') && Key::keyCheckOn('j')) Network::setCommand(COMMAND_ATTACK_RIGHT);
			if (Key::keyCheckPressed('w') && Key::keyCheckOn('j')) Network::setCommand(COMMAND_ATTACK_UP);
			if (Key::keyCheckPressed('a') && Key::keyCheckOn('j')) Network::setCommand(COMMAND_ATTACK_LEFT);
			if (Key::keyCheckPressed('s') && Key::keyCheckOn('j')) Network::setCommand(COMMAND_ATTACK_DOWN);

			if (Key::keyCheckPressed('d') && Key::keyCheckOn('k')) Network::setCommand(COMMAND_SKILL_RIGHT);
			if (Key::keyCheckPressed('w') && Key::keyCheckOn('k')) Network::setCommand(COMMAND_SKILL_UP);
			if (Key::keyCheckPressed('a') && Key::keyCheckOn('k')) Network::setCommand(COMMAND_SKILL_LEFT);
			if (Key::keyCheckPressed('s') && Key::keyCheckOn('k')) Network::setCommand(COMMAND_SKILL_DOWN);

			if (Key::keyCheckPressed('l')) Network::setCommand(COMMAND_FLAG);
		}
	}

}
