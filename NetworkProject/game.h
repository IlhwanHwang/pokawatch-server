#pragma once

#include "unit.h"
#include "protocol.h"

class Game {
private:
	static Unit unitArray[UNIT_NUM_MAX];
	static Flag flagArray[FLAG_NUM_MAX];
	static Poison poisonArray[POISON_NUM_MAX];
	static Petal petalArray[PETAL_NUM_MAX];
	static Mushroom mushroomArray[MUSHROOM_NUM_MAX];
	static int score[2];
	static int turnleft;

public:
	static void init();
};