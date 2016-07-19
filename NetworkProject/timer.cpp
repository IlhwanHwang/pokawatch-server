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
//#include "server.h"
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
	// add any per turn action

	Gui::turn();
	Network::turn();

	if(Network::getGameStart()[0] == GAME_START_CHAR && Game::getTurnLeft() > 0) Game::turn();
}

void Timer::update(int count) {
	glutTimerFunc(frameInterval, update, count + 1);

	if (count % framePerTurn == 0) {
		turn();
	}

	Gui::update();
	Draw::refresh();

	// add any per frame actions
	// such as update() and draw() for all object

	Network::update();

	if (Network::getMode() == MODE_SERVER) {
		Game::update();
		Game::draw();
	}
	if (Network::getMode() == MODE_NOTHING) {
		Draw::naivefill(Rspr::infoMain);
	}
	if (Network::getMode() == MODE_CLIENT) {
		int cnt = 0;
		for (int i = 0; i < UNIT_NUM_MAX; i++)
			if(Network::getCharacterSelection(i) == 0) cnt++;

		if (cnt == UNIT_NUM_MAX)
			Draw::naivefill(Rspr::infoClient1);
		else
			Draw::naivefill(Rspr::infoClient3);
	}

	glutSwapBuffers(); // Automatic Vsync.

	Key::keyUpdate();
}