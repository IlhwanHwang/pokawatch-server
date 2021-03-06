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
#include "audio.h"

using namespace std;

int Timer::frameInterval;
int Timer::framePerTurn;
bool Timer::turnWait = false;
float Timer::turnWaitPhase = 0.0;

void Timer::init(int interval, int perturn) {
	frameInterval = interval;
	framePerTurn = perturn;

	glutTimerFunc(frameInterval, update, 0);
}

void Timer::turn() {
	// add any per turn action

	Gui::turn();
	Network::turn();
}

void Timer::update(int count) {
	glutTimerFunc(frameInterval, update, count + 1);

	Audio::update();

	if (Key::keyCheckPressed('1')) {
		turnWait = !turnWait;
	}

	if (!Game::isStarted()) {
		if (Key::keyCheckPressed(32)) {
			Game::start();
		}
	}
	else {
		if (turnWait) {
			if (Key::keyCheckPressed(32))
				turn();
			Audio::discardTurn();
		}
		else {
			if (Audio::isTurn() && !Game::isEnded())
				turn();
		}
	}

	Gui::update();
	Draw::refresh();

	// add any per frame actions
	// such as update() and draw() for all object

	Network::update();

	if (Network::getMode() == MODE_SERVER || Network::getMode() == MODE_SINGLE) {
		Game::update();
		Game::draw();
	}

	glutSwapBuffers(); // Automatic Vsync.

	Key::keyUpdate();
}