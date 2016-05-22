#include "game.h"
#include "protocol.h"
#include "network.h"

Unit Game::unitArray[UNIT_NUM_MAX] = { Unit(TEAM_POSTECH), Unit(TEAM_POSTECH), Unit(TEAM_POSTECH), Unit(TEAM_KAIST), Unit(TEAM_KAIST), Unit(TEAM_KAIST) };
Flag Game::flagArray[FLAG_NUM_MAX] = { Flag(FLAG1_X, FLAG1_Y), Flag(FLAG2_X, FLAG2_Y) , Flag(FLAG3_X, FLAG3_Y) , Flag(FLAG4_X, FLAG4_Y) , Flag(FLAG5_X, FLAG5_Y) };
Poison Game::poisonArray[POISON_NUM_MAX];
Petal Game::petalArray[PETAL_NUM_MAX];
Mushroom Game::mushroomArray[MUSHROOM_NUM_MAX];
protocol_data Game::protocolToSend;
protocol_data * Game::protocolPointer;
int Game::score[2];
int Game::turnleft;

void Game::init()
{
	score[0] = 0;
	score[1] = 0;
	turnleft = TURN_MAX;
	makeProtocol();

	unitArray[0].spawn(0, 0, DEP_CSE);
	unitArray[1].spawn(1, 0, DEP_CSE);
	unitArray[2].spawn(2, 0, DEP_CSE);
	unitArray[3].spawn(3, 0, DEP_CSE);
	unitArray[4].spawn(4, 0, DEP_CSE);
	unitArray[5].spawn(5, 0, DEP_CSE);
}

void Game::makeProtocol()
{
	for (int i = 0; i < UNIT_NUM_MAX; i++) protocolToSend.unit[i] = *(unitArray[i].getProtocol());
	for (int i = 0; i < FLAG_NUM_MAX; i++) protocolToSend.flag[i] = *(flagArray[i].getProtocol());
	for (int i = 0; i < POISON_NUM_MAX; i++) protocolToSend.poison[i] = *(poisonArray[i].getProtocol());
	for (int i = 0; i < PETAL_NUM_MAX; i++) protocolToSend.petal[i] = *(petalArray[i].getProtocol());
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) protocolToSend.mushroom[i] = *(mushroomArray[i].getProtocol());

	protocolPointer = &protocolToSend;
}

void Game::update() {
	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].update();
	for (int i = 0; i < FLAG_NUM_MAX; i++) flagArray[i].update();
	for (int i = 0; i < POISON_NUM_MAX; i++) poisonArray[i].update();
	for (int i = 0; i < PETAL_NUM_MAX; i++) petalArray[i].update();
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) mushroomArray[i].update();
}

void Game::draw() {
	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].draw();
	for (int i = 0; i < FLAG_NUM_MAX; i++) flagArray[i].draw();
	for (int i = 0; i < POISON_NUM_MAX; i++) poisonArray[i].draw();
	for (int i = 0; i < PETAL_NUM_MAX; i++) petalArray[i].draw();
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) mushroomArray[i].draw();
}

void Game::turn() {
	for (int i = 0; i < UNIT_NUM_MAX; i++) {
		Unit& u = unitArray[i];
		protocol_command c = Network::getCommand(i);

		switch (rand() % 4) {
		case 0:
			c = COMMAND_MOVE_RIGHT;
			break;
		case 1:
			c = COMMAND_MOVE_UP;
			break;
		case 2:
			c = COMMAND_MOVE_LEFT;
			break;
		case 3:
			c = COMMAND_MOVE_DOWN;
			break;
		}

		switch (c) {
		case COMMAND_MOVE_RIGHT:
			u.move(DIRECTION_RIGHT);
			break;
		case COMMAND_MOVE_UP:
			u.move(DIRECTION_UP);
			break;
		case COMMAND_MOVE_LEFT:
			u.move(DIRECTION_LEFT);
			break;
		case COMMAND_MOVE_DOWN:
			u.move(DIRECTION_DOWN);
			break;
		}
	}
}