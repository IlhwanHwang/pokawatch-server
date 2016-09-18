//
// CNH 2016.05.19
// unit class
//

#pragma once

#include "protocol.h"
#include <iostream>

// Abstract object interface
class Object {
protected:
	const char* name;
	void error(const char* e) { std::cerr << name << ": " << e << std::endl; }

public:
	Object() { name = "Anonmous object"; }
	virtual void turn() = 0; // called every turn
	virtual void update() = 0; // called every frame
	virtual void draw() const = 0; // called every frame (draw event), no sideeffect is allowed.
};

// Recommended flow
// 1. turn() for all unit
// 2. parse and input commands
// 3. calculate damages and heals

// Provided features by Unit class
// 1. prevent unallowed actions
// - moving to the outside of the map
// - taking action when stunned
// - ME trying to move in consecutive turns
// - and so on
// 2. update trivias every turn
// - decrease respawn time
// - decrease cooltime
// - decrease stun
// 3. update state
// - DEAD where respawn > 0
// - STUN where stun > 0

class Unit : public Object {
private:
	protocol_unit p;
	int orgx, orgy;

	int death;
	bool animationFlip;
	bool aniInvincible;
	bool aniDamaged;
	bool aniHealed;
	
	bool flagRespawned;
	bool flagDead;
	bool flagUnstun;

	// Members for movement animation
	float moveOffX, moveOffY, moveOffZ;
	protocol_direction moveOffDirection;
	bool moveOffAction;

	int moveStun;
	int healthMax;
	int healthPrevious;
	int damaged;
	int healed;

	void init();
	void flip(protocol_direction direction) {
		if (direction == DIRECTION_RIGHT)
			animationFlip = false;
		if (direction == DIRECTION_LEFT)
			animationFlip = true;
	};

public:
	Unit(int x, int y, protocol_team team);
	Unit(int x, int y, protocol_team team, const char* name);

	void spawn(protocol_dep dep); // spawn as specified dep
	bool move(protocol_direction direction); // move 1 cell toward the direction
	void attack(protocol_direction direction); // attack toward the direction
	void skill(protocol_direction direction); // use hero skill toward the direction
	void damage(int h = 1); // exert given damage on unit (1 on default)
	void heal(int h = 1); // heal unit with given value (1 on default)
	void stun(int s = 3); // stun unit (3 turns on default)
	void destun();
	void kill(); // kill

	void release() { p.state = STATE_IDLE; init(); } // For the first time

	void moveResetMovestun() { moveStun = 0; } // Release movestun (just for me)
	void moveOffDiscard() { moveOffDirection = DIRECTION_NULL; }; // Calling it after move makes unit jump on its position

	void turninit();
	void turn();
	void turnpost();
	void flush();
	void update();
	void draw() const;

	bool isAlive() { return p.state != STATE_DEAD && p.state != STATE_NULL; }

	const protocol_unit* getProtocol() const { return &p; } // Export unit information as protocol.

	protocol_team getTeam() const { return p.team; }
	protocol_dep getDep() const { return p.dep; }
	int getX() const { return p.x; }
	int getY() const { return p.y; }
	protocol_state getState() const { return p.state; }
	int getHealth() const { return p.health; }
	bool getHero() const { return p.hero; }
	int getCooltime() const { return p.cooltime; }
	int getRespawn() const { return p.respawn; }
	int getDeath() const { return death; }
	int getOrgX() const { return orgx; }
	int getOrgY() const { return orgy; }
	bool getFliped() const { return animationFlip; }

	bool checkRespawned() { if (flagRespawned) { flagRespawned = false; return true; } return false; };
	bool checkDead() { if (flagDead) { flagDead = false; return true; } return false; };

	// bypassing all constraints and animations.
	// just use it for very exceptional cases.
	void setPosition(int x, int y) { p.x = x; p.y = y; }
	void setState(protocol_state state) { p.state = state; }
	void setHealth(int health) { p.health = health; }
	void setHero(bool hero) { p.hero = hero; }
	void setCooltime(int cooltime) { p.cooltime = cooltime + 1; }
	void setRespawn(int respawn) { p.respawn = respawn + 1; }
	void setStun(int stun) { p.stun = stun + 1; }
};

class Poison : public Object {
private:
	protocol_poison p;

public:
	Poison();

	void spawn(protocol_team team, int x, int y);
	void turn();
	void update();
	void draw() const;

	void invalidate() { p.valid = false; }

	const protocol_poison* getProtocol() const { return &p; }

	protocol_team getTeam() const { return p.team; }
	int getX() const { return p.x; }
	int getY() const { return p.y; }
	int getSpan() const { return p.span; }
	bool isValid() const { return p.valid; }
};

class Petal : public Object {
private:
	protocol_petal p;
	float moveOffX, moveOffY;

public:
	Petal();

	void spawn(protocol_team team, int x, int y, protocol_direction direction);
	void turn();
	void update();
	void draw() const;

	void invalidate() { p.valid = false; }
	void stepback();

	const protocol_petal* getProtocol() const { return &p; }

	protocol_team getTeam() const { return p.team; }
	int getX() const { return p.x; }
	int getY() const { return p.y; }
	int getDirection() const { return p.direction; }
	bool isValid() const { return p.valid; }
};

class Mushroom : public Object {
private:
	protocol_mushroom p;

public:
	Mushroom();

	void spawn(protocol_team team, int x, int y);
	void turn();
	void update();
	void draw() const;

	void invalidate() { p.valid = false; }

	protocol_team getTeam() const { return p.team; }
	int getX() const { return p.x; }
	int getY() const { return p.y; }
	bool isValid() const { return p.valid; }

	const protocol_mushroom* getProtocol() const { return &p; }
};