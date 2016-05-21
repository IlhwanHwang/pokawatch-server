//
// CNH 2016.05.19
// timer module
//

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdio>

#include "timer.h"
#include "key.h"
#include "draw.h"
#include "shader.h"
#include "debug.h"

int Timer::frameInterval;
int Timer::framePerTurn;

Sprite red;
Sprite blue;

void Timer::init(int interval, int perturn) {
	frameInterval = interval;
	framePerTurn = perturn;

	red.load("red.png");
	blue.load("blue.png");

	glutTimerFunc(frameInterval, update, 0);
}

void Timer::turn() {
	std::cout << "Turn!" << std::endl;
}

void Timer::update(int count) {

	glutTimerFunc(frameInterval, update, count + 1);

	if (count % framePerTurn == 0) {
		turn();
	}
	
	glClearColor(0.25, 0.75, 1.0, 1.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Shader::projection(Matrix::Ortho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT));

	mat4 m;
	Shader::useDefault();
	Shader::projection(m);
	Shader::apply();
	Shader::draw4thPlane();
	Shader::drawCanonical();
	//Draw::draw(red, 0.0, 32.0, 32.0);
	//Draw::draw(blue, 0.0, 48.0, 48.0);
	//Draw::flush();

	glutSwapBuffers();

	Key::keyUpdate();
}