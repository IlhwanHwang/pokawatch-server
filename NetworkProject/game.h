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
	static protocol_data protocolToSend;
	static protocol_data *protocolPointer;
	static int heroLeft[2];
	static int death[2];								// record how many death occured in each team
	static int spawn[2];								// record how many spawn occured in each team

	static void ruleMove();
	static void ruleAttack();
	static void ruleSkill();
	static void ruleSpawn();
	static void ruleFlag();


public:
	static void init();
	static void makeProtocol();
	static void update();
	static void draw();
	static void turn();
	static void release();
	static protocol_data getProtocol() { return protocolToSend; }
	static protocol_data* getProtocolPointer() { return protocolPointer; }
	static Unit &getUnit(int index) { return unitArray[index]; }
	static int getValidPoisonIndex();
	static int getValidPetalIndex();
	static int getValidMushroomIndex();
	static int getDeath(int ind) { return death[ind]; }
	static void setDeath(int ind, int x) { death[ind] = x; }
	static int getTurnLeft() { return turnleft; }
	static int getScore(int ind) { return score[ind]; }
};