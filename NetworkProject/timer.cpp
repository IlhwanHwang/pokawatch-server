//
// CNH 2016.05.19
// timer module
//

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdio>

#include "timer.h"
#include "protocol.h"
#include "key.h"

int Timer::frameInterval;
int Timer::framePerTurn;
extern int mode;
extern int characterSelection;
int command;

void Timer::init(int interval, int perturn) {
	frameInterval = interval;
	framePerTurn = perturn;

	glutTimerFunc(frameInterval, update, 0);
}

void Timer::turn() {
	std::cout << "Turn!" << std::endl;
	if (mode > 0 && characterSelection > 0)
	{

	}
	// add any per turn action
	

}

void Timer::update(int count) {
	glutTimerFunc(frameInterval, update, count + 1);

	if (count % framePerTurn == 0) {
		turn();
	}

	if (mode > 0) printf("mode CHANGED!");

	// add any per frame actions
	// such as update() and draw() for all object

	if (mode == 0 && Key::keyCheckPressed('1'))
	{
		mode = 1;
	}
	if (mode == 0 && Key::keyCheckPressed('2'))
	{
		mode = 2;
	}// if not 1 or 2 give error message
	

	//if (characterSelection == 0 && Key::keyCheckPressed('a') && Key::keyCheckOn('s')) printf("hihi"); // sa
	
		
	if (mode > 0)
	{
		if (characterSelection == 0 && Key::keyCheckPressed('1'))
		{
			characterSelection = COMMAND_SPAWN_CSE;
		}
		if (characterSelection == 0 && Key::keyCheckPressed('2'))
		{
			characterSelection = COMMAND_SPAWN_PHYS;
		}
		if (characterSelection == 0 && Key::keyCheckPressed('3'))
		{
			characterSelection = COMMAND_SPAWN_LIFE;
		}
		if (characterSelection == 0 && Key::keyCheckPressed('4'))
		{
			characterSelection = COMMAND_SPAWN_ME;
		}
		if (characterSelection == 0 && Key::keyCheckPressed('5'))
		{
			characterSelection = COMMAND_SPAWN_CHEM;
		}
		if (characterSelection > 0)
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

	if (Key::keyCheckOn('w')) printf("w was clicked");


	/*
	
	COMMAND_MOVE_RIGHT,
	COMMAND_MOVE_UP,
	COMMAND_MOVE_LEFT,
	COMMAND_MOVE_DOWN,
	
	COMMAND_ATTACK_RIGHT,
	COMMAND_ATTACK_UP,
	COMMAND_ATTACK_LEFT,
	COMMAND_ATTACK_DOWN,
	
	COMMAND_SKILL_RIGHT,
	COMMAND_SKILL_UP,
	COMMAND_SKILL_LEFT,
	COMMAND_SKILL_DOWN,
	
	COMMAND_SPAWN_CSE,
	COMMAND_SPAWN_PHYS,
	COMMAND_SPAWN_LIFE,
	COMMAND_SPAWN_ME,
	COMMAND_SPAWN_CHEM,
	
	COMMAND_FLAG
	
	*/

	Key::keyUpdate();
}