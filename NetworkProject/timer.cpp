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
	Game::turn();
}

void Timer::update(int count) {
	glutTimerFunc(frameInterval, update, count + 1);

	if (count % framePerTurn == 0) {
		turn();
	}

	// add any per frame actions
	// such as update() and draw() for all object

	Network::update();
	Game::update();

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	Shader::projection(Matrix::Ortho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT));
	Game::draw();
	Draw::flush();

	glutSwapBuffers();

	Key::keyUpdate();
}