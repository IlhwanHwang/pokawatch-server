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
#include <ctime>

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
#include "resource.h"

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
	Network::turn();
	if(Network::getGameStart()[0]==GAME_START_CHAR && Game::getTurnLeft()>0) Game::turn();
	if (Game::getTurnLeft <= 0)
	{
		if (Game::getScore(TEAM_POSTECH - 1) > Game::getScore(TEAM_KAIST - 1)) printf("POSTECH WIN !\n");
		if (Game::getScore(TEAM_POSTECH - 1) < Game::getScore(TEAM_KAIST - 1)) printf("KAIST WIN !\n");
		if (Game::getScore(TEAM_POSTECH - 1) == Game::getScore(TEAM_KAIST - 1)) printf("DRAW !\n");
	}
}

void Timer::update(int count) {
	glutTimerFunc(frameInterval, update, count + 1);

	if (count % framePerTurn == 0) {
		turn();
	}

	if (Key::keyCheckPressed('1') && Network::getMode() == MODE_NOTHING) {
		Game::getUnit(0).spawn(DEP_CSE);
		Game::getUnit(1).spawn(DEP_CSE);
		//Game::getUnit(2).spawn(DEP_ME);
		//Game::getUnit(3).spawn(DEP_LIFE);
		//Game::getUnit(4).spawn(DEP_PHYS);
		//Game::getUnit(5).spawn(DEP_CSE);
		Game::release();
		Network::setGameStart(0, GAME_START_CHAR);
	}

	// add any per frame actions
	// such as update() and draw() for all object

	Network::update();
	Game::update();

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	Game::draw();

	glutSwapBuffers();

	Key::keyUpdate();
}