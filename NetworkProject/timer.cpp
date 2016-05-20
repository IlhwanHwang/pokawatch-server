//
// CNH 2016.05.19
// timer module
//

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "timer.h"
#include "key.h"

int Timer::frameInterval;
int Timer::framePerTurn;

void Timer::init(int interval, int perturn) {
	frameInterval = interval;
	framePerTurn = perturn;

	glutTimerFunc(frameInterval, update, 0);
}

void Timer::turn() {
	std::cout << "Turn!" << std::endl;
	// add any per turn action
}

void Timer::update(int count) {

	glutTimerFunc(frameInterval, update, count + 1);
	
	if (count % framePerTurn == 0) {
		turn();
	}

	std::cout << "Frame number " << count << std::endl;
	// add any per frame actions
	// such as update() and draw() for all object

	Key::keyUpdate();
}