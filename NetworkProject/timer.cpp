//
// CNH 2016.05.19
// timer module
//

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdio>
#include <WinSock2.h>
#include <string>

#include "timer.h"
#include "client.h"
#include "server.h"
#include "client.h"
#include "protocol.h"
#include "unit.h"
#include "key.h"

using namespace std;

int Timer::frameInterval;
int Timer::framePerTurn;

extern SOCKET hServSock;			// of server
extern SOCKET hClntSock[6];		// of server
extern SOCKET hSocket;				// of client
extern SOCKADDR_IN servAddr;		// of server/client
extern SOCKADDR_IN clntAddr[6];	// of server
extern int szClntAddr[6];			// of server
extern char messageToClient[3];		// of server
extern char messageFromClient[6][16]; // of server
extern char messageToServer[];		// of client
extern int mode;					//determine server(1) client(2) or nothing (0)
extern int characterSelection;		//character_selection_time;
extern char gameStart[2];				// start(1)
extern Unit unitArray[UNIT_NUM_MAX];

int command;


void Timer::init(int interval, int perturn) {
	frameInterval = interval;
	framePerTurn = perturn;

	glutTimerFunc(frameInterval, update, 0);
}

void Timer::turn() {
	//std::cout << "Turn!" << std::endl;
	// add any per turn action

	if (mode == MODE_CLIENT && gameStart[0] == GAME_START_CHAR)
	{
		sendToServer(&hSocket, (char *)(to_string(command)).c_str());
		printf("command : %d was sent\n", command);
	}
	if (mode == MODE_SERVER && gameStart[0] != GAME_START_CHAR)
	{
		printf("GAME START!!!!!!!!!!!!!!!!\n");
		sendToClient(messageToClient, hClntSock);
		recieveFromClient(hClntSock, messageFromClient);	// recieve data;
		printf("%s\n", messageFromClient);
	}
}

void Timer::update(int count) {
	glutTimerFunc(frameInterval, update, count + 1);

	if (count % framePerTurn == 0) {
		turn();
	}

	// add any per frame actions
	// such as update() and draw() for all object

	if (mode == MODE_NOTHING && Key::keyCheckPressed(MODE_SERVER_KEY))
	{
		char gameStartMessage[2];
		gameStartMessage[0] = GAME_START_CHAR;
		gameStartMessage[1] = '\0';

		mode = MODE_SERVER;
		printf("mode- server chosn\n");
		makeServerSocket(&hServSock, &servAddr);
		acceptClient(&hServSock, hClntSock, clntAddr, szClntAddr);
		recieveFromClient(hClntSock, messageFromClient);			// recieve spawn
		printf("SELECTED : \n");
		
		for (int i = 0; i < UNIT_NUM_MAX; i++)
		{
			if (unitArray[i].getTeam() == TEAM_POSTECH)
			{
				unitArray[i].spawn(TEAM_POSTECH_SPAWN_X, TEAM_POSTECH_SPAWN_Y, (protocol_dep)(atoi(messageFromClient[i])));
				printf("%d! ", atoi(messageFromClient[i]));
			}
			else if ((unitArray[i].getTeam() == TEAM_KAIST))
			{
				unitArray[i].spawn(TEAM_KAIST_SPAWN_X, TEAM_KAIST_SPAWN_Y, (protocol_dep)(atoi(messageFromClient[i])));
				printf("%d! ", atoi(messageFromClient[i]));
			}
		}

		sendToClient(gameStartMessage, hClntSock);					// accept clients
		gameStart[0] = GAME_START_CHAR;								// game started
	}

	if (mode == MODE_NOTHING && Key::keyCheckPressed(MODE_CLIENT_KEY))
	{
		mode = MODE_CLIENT;
		printf("mode- client chose\n");
		makeClientSocket(&hSocket, &servAddr);
		connectToServer(&servAddr, &hSocket);
	}// if invalid input, we should give error message
	
	if (mode == MODE_SERVER || mode == MODE_CLIENT)					// 
	{
		if (mode==MODE_CLIENT && characterSelection == 0 && Key::keyCheckPressed('1'))
		{
			characterSelection = DEP_CSE;
			sendToServer(&hSocket, (char *)(to_string(characterSelection)).c_str());
			printf("character chosen : %d", characterSelection);
			getProtocolDataFromServer(&hSocket, gameStart);
			printf("GameStart!");
		}
		if (mode == MODE_CLIENT && characterSelection == 0 && Key::keyCheckPressed('2'))
		{
			characterSelection = DEP_PHYS;
			sendToServer(&hSocket, (char *)(to_string(characterSelection)).c_str());
			printf("character chosen : %d", characterSelection);
			getProtocolDataFromServer(&hSocket, gameStart);
			printf("GameStart!");
		}
		if (mode == MODE_CLIENT && characterSelection == 0 && Key::keyCheckPressed('3'))
		{
			characterSelection = DEP_LIFE;
			sendToServer(&hSocket, (char *)(to_string(characterSelection)).c_str());
			printf("character chosen : %d", characterSelection);
			getProtocolDataFromServer(&hSocket, gameStart);
			printf("GameStart!");
		}

		if (mode == MODE_CLIENT && characterSelection == 0 && Key::keyCheckPressed('4'))
		{
			characterSelection = DEP_ME;
			sendToServer(&hSocket, (char *)(to_string(characterSelection)).c_str());
			printf("character chosen : %d", characterSelection);
			getProtocolDataFromServer(&hSocket, gameStart);
			printf("GameStart!");
		}

		if (mode == MODE_CLIENT && characterSelection == 0 && Key::keyCheckPressed('5'))
		{
			characterSelection = DEP_CHEM;
			sendToServer(&hSocket, (char *)(to_string(characterSelection)).c_str());
			printf("character chosen : %d\n", characterSelection);
			getProtocolDataFromServer(&hSocket, gameStart);
			printf("GameStart!\n");
		}

		if (mode == MODE_CLIENT && characterSelection > 0 && gameStart[0] == GAME_START_CHAR)
		{
			if (Key::keyCheckPressed('d')) command = COMMAND_MOVE_RIGHT;
			if (Key::keyCheckPressed('w')) command = COMMAND_MOVE_UP;
			if (Key::keyCheckPressed('a')) command = COMMAND_MOVE_LEFT;
			if (Key::keyCheckPressed('s')) command = COMMAND_MOVE_DOWN;

			if (Key::keyCheckPressed('d') && Key::keyCheckOn('j')) command = COMMAND_ATTACK_RIGHT;
			if (Key::keyCheckPressed('w') && Key::keyCheckOn('j')) command = COMMAND_ATTACK_UP;
			if (Key::keyCheckPressed('a') && Key::keyCheckOn('j')) command = COMMAND_ATTACK_LEFT;
			if (Key::keyCheckPressed('s') && Key::keyCheckOn('j')) command = COMMAND_ATTACK_DOWN;

			if (Key::keyCheckPressed('d') && Key::keyCheckOn('k')) command = COMMAND_SKILL_RIGHT;
			if (Key::keyCheckPressed('w') && Key::keyCheckOn('k')) command = COMMAND_SKILL_UP;
			if (Key::keyCheckPressed('a') && Key::keyCheckOn('k')) command = COMMAND_SKILL_LEFT;
			if (Key::keyCheckPressed('s') && Key::keyCheckOn('k')) command = COMMAND_SKILL_DOWN;

			if (Key::keyCheckPressed('l')) command = COMMAND_FLAG;
		}
	}

	Key::keyUpdate();
}