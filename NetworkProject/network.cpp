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

using namespace std;

SOCKET Network::hServSock;														// Server socket variable of server/client side
SOCKET Network::hClntSock[CLIENT_NUM_MAX];										// Client socket variable of server side
SOCKET Network::hSocket;														// Client socket variable of client side
SOCKADDR_IN Network::servAddr;													// Server address variable of server/client side
SOCKADDR_IN Network::clntAddr[CLIENT_NUM_MAX];									// Client address variable of server side
int Network::szClntAddr[CLIENT_NUM_MAX];											// Client address size variable of server side
char Network::messageToClient[MESSAGE_T0_CLIENT_SIZE];							// Message buffer of server side
char Network::messageFromClient[CLIENT_NUM_MAX][MESSAGE_TO_SERVER_SIZE];			// Message buffer of server side
char Network::messageToServer[MESSAGE_TO_SERVER_SIZE];							// Message buffer of client side
int Network::mode;																// determine server/ client/ nothing
int Network::characterSelection;												// Information of selection of charactor(dep)
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

	printf("Server Listen\n");
}

void Network::acceptClient()
{
	//accept client's request
	for (int i = 0; i < CLIENT_NUM_MAX; i++) szClntAddr[i] = sizeof(clntAddr[i]);
	for (int i = 0; i < CLIENT_NUM_MAX; i++)
	{
		hClntSock[i] = accept(hServSock, (SOCKADDR*)&clntAddr[i], &szClntAddr[i]);
		if (hClntSock[i] == INVALID_SOCKET) ErrorHandling("accept() error");
	}
}

void Network::sendToClient(char *messageToClient) // Message sending routine of client side
{
	for (int i = 0; i < CLIENT_NUM_MAX; i++)
	{
		int WhatDo = send(hClntSock[i], messageToClient, MESSAGE_T0_CLIENT_SIZE - 1, 0);
	}
}

void Network::recieveFromClient() // Message recieving routine of server side
{
	for (int i = 0; i < CLIENT_NUM_MAX; i++)
	{
		int strLen = recv(hClntSock[i], messageFromClient[i], MESSAGE_TO_SERVER_SIZE - 1, 0);
		messageFromClient[i][strLen] = '\0';
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
	printf("UNIT INFO\n");
	for (int i = 0; i < UNIT_NUM_MAX; i++) printf("team %d dep %d x : %d y : %d state : %d health : %d\nhero : %d cooltime : %d respawn : %d stun : %d\n", newData->unit[i].team, newData->unit[i].dep, newData->unit[i].x, newData->unit[i].y, newData->unit[i].state, newData->unit[i].health, newData->unit[i].hero, newData->unit[i].cooltime, newData->unit[i].respawn, newData->unit[i].stun);
	printf("FLAG INFO\n");
	for (int i = 0; i < FLAG_NUM_MAX; i++) printf("team %d x : %d y : %d\n", newData->flag[i].team, newData->flag[i].x, newData->flag[i].y);
	printf("PETAL INFO\n");
	for (int i = 0; i < PETAL_NUM_MAX; i++) printf("team %d direction %d valid %d x %d y %d\n", newData->petal[i].team, newData->petal[i].direction, newData->petal[i].valid, newData->petal[i].x, newData->petal[i].y);
	printf("POISON INFO\n");
	for (int i = 0; i < POISON_NUM_MAX; i++) printf("team %d valid %d span %d x %d y %d\n", newData->poison[i].team, newData->poison[i].valid, newData->poison[i].span, newData->poison[i].x, newData->poison[i].y);
	printf("MUSHROOM INFO\n");
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) printf("team %d valid %d x %d y %d\n", newData->mushroom[i].team, newData->mushroom[i].valid, newData->mushroom[i].x, newData->mushroom[i].y);
	printf("SCORE\n");
	printf("[POSTECH] %d [KAIST] %d\n", newData->score[TEAM_POSTECH - 1], newData->score[TEAM_KAIST - 1]);
	printf("TURN LEFT\n");
	printf("%d\n", newData->turnleft);
	printf("\n");
}

void Network::recieveGameStart()
{
	Draw::naivefill(Rspr::infoClient2);
	glutSwapBuffers();

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
	// in timer each turn this function is called

	if (Network::getMode() == MODE_CLIENT && Network::getGameStart()[0] == GAME_START_CHAR) // for client when game started 
	{
		Network::sendToServer((char *)(to_string(Network::getCommand())).c_str());			// send command
		Network::getProtocolDataFromServer();												// get protocol data of that time
	}
	if (Network::getMode() == MODE_SERVER && Network::getGameStart()[0] == GAME_START_CHAR) // for server when game started
	{
		char *protocolToSend;
		Network::recieveFromClient();														// recieve command from clients;
		protocolToSend = (char*)(Game::getProtocolPointer());
		protocolToSend[MESSAGE_T0_CLIENT_SIZE] = '\0';
		Network::sendToClient(protocolToSend);												// send protocol data to client
	}
}

void Network::update() // frame turn routine
{
	if (Network::getMode() == MODE_NOTHING)		// mode selection (CLIENT)
	{
		Network::setMode(MODE_CLIENT);														// set as client 
		printf("mode- client chose\n");
		Network::makeClientSocket();														// client socket made
		Network::connectToServer();															// connect to server
	}// if invalid input, we should give error message

	if (Network::getMode() == MODE_SERVER || Network::getMode() == MODE_CLIENT)				// after mode selected client should decide character
	{
		if (Network::getMode() == MODE_CLIENT && Network::getCharacterSelection() == 0 && Key::keyCheckPressed('1'))			// mapping for key and characters
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

		if (Network::getMode() == MODE_CLIENT && Network::getCharacterSelection() > 0 && Network::getGameStart()[0] == GAME_START_CHAR) // after game started by keyboard control send appropriate command
		{
			Ai::ai();
			
		}
	}

}
