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
#include "network.h"
#include "game.h"
#include "draw.h"
#include "shader.h"
#include "debug.h"

using namespace std;

int Timer::frameInterval;
int Timer::framePerTurn;

void Timer::init(int interval, int perturn) {
	frameInterval = interval;
	framePerTurn = perturn;

	glutTimerFunc(frameInterval, update, 0);
}

void Timer::turn() {
	//std::cout << "Turn!" << std::endl;
	// add any per turn action

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
		protocolToSend[MESSAGE_T0_CLIENT_SIZE-1] = '\0';
		
		Network::sendToClient(protocolToSend);

		for (int i = 0; i < MESSAGE_T0_CLIENT_SIZE; i=i+4)
		{
			//printf("%d ", ((int*)protocolToSend)[i]);
		}
		printf("\n");

	}
}

void Timer::update(int count) {
	glutTimerFunc(frameInterval, update, count + 1);

	if (count % framePerTurn == 0) {
		turn();
	}

	// add any per frame actions
	// such as update() and draw() for all object

	if (Network::getMode() == MODE_NOTHING && Key::keyCheckPressed(MODE_SERVER_KEY))
	{
		char gameStartMessage[2];
		gameStartMessage[0] = GAME_START_CHAR;
		gameStartMessage[1] = '\0';

		Network::setMode(MODE_SERVER);
		printf("mode- server chosn\n");
		Network::makeServerSocket();
		Network::acceptClient();
		Network::recieveFromClient();			// recieve spawn
		printf("SELECTED : \n");
		/*
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
		}*/

		Network::sendToClient(gameStartMessage);		// accept clients
		Network::setGameStart(0,GAME_START_CHAR);		// game started
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
		if (Network::getMode() ==MODE_CLIENT && Network::getCharacterSelection() == 0 && Key::keyCheckPressed('1'))
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

	Key::keyUpdate();
}