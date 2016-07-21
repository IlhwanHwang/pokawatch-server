#pragma once

#include "unit.h"
#include "protocol.h"
#include "effect.h"

class Game {											//EVERY THING REALTED TO GAME IS HERE

private:
	static Unit unitArray[UNIT_NUM_MAX];						// unit array
	static Poison poisonArray[POISON_NUM_MAX];					// poison array
	static Petal petalArray[PETAL_NUM_MAX];						// petal array
	static Mushroom mushroomArray[MUSHROOM_NUM_MAX];			// mushroom array
	static protocol_team owner;
	static int own[2];
	static int win[2];
	static int extra;
	static int elapsed;
	static protocol_data protocolToSend;						// protocol data
	static protocol_data *protocolPointer;
	static int death[2];										// record how many death occured in each team
	static int order[UNIT_NUM_MAX];

	static bool end;

	static void rulePriority();
	static void ruleMove();										// rules related to move command
	static void ruleAttack();									// rules related to attack command
	static void ruleCollide();
	static void ruleSkill();									// rules related to skill command
	static void ruleSpawn();									// rules related to spawn command
	static void rulePoint();
	static void drawFaces();
	static void drawPoint();
	static void drawOverlay();

public:
	static void init();											// initialize game
	static void makeProtocol();									// protocol data making routine									
	static void update();										// frame interval routine
	static void turn();											// turn routine
	static void draw();											// draw routine
	static void release();

	static void regionApply(protocol_team t, int x1, int y1, int x2, int y2, int damage, int heal, int stun);
	static void regionDamage(protocol_team t, int x1, int y1, int x2, int y2, int h) {
		regionApply(t, x1, y1, x2, y2, h, 0, 0);
	};
	static void regionHeal(protocol_team t, int x1, int y1, int x2, int y2, int h) {
		regionApply(t, x1, y1, x2, y2, 0, h, 0);
	};
	static void regionStun(protocol_team t, int x1, int y1, int x2, int y2, int stun) {
		regionApply(t, x1, y1, x2, y2, 0, 0, stun);
	};
	static void regionDamageAll(protocol_team t, int h) {
		regionDamage(t, 0, 0, MAP_WIDTH - 1, MAP_HEIGHT - 1, h);
	}
	static void regionHealAll(protocol_team t, int h) {
		regionHeal(t, 0, 0, MAP_WIDTH - 1, MAP_HEIGHT - 1, h);
	}

	//getters and setters
	static protocol_data getProtocol() { return protocolToSend; }
	static protocol_data* getProtocolPointer() { return protocolPointer; }
	static Unit &getUnit(int index) { return unitArray[index]; }
	static int getValidPoisonIndex();
	static int getValidPetalIndex();
	static int getValidMushroomIndex();
	static int getDeath(int ind) { return death[ind]; }
	static int getElapsed() { return elapsed; }
	static void setDeath(int ind, int x) { death[ind] = x; }
	static bool isEnded() { return end; }
};