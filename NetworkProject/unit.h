//
// CNH 2016.05.19
// unit class
//

#pragma once

#include "protocol.h"
#include <iostream>

class Object {
protected:
	const char* name;
	void error(const char* e) { std::cerr << name << ": " << e << std::endl; }

public:
	Object() { name = "Anonmous object"; }
	virtual void turn() = 0; // called every turn
	virtual void update() = 0; // called every frame
	virtual void draw() const = 0; // called every frame (draw event)
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
	
	float moveOffX, moveOffY, moveOffPhase;
	protocol_direction moveOffDirection;

	int moveStun;
	int healthMax;
	
	void init();

public:
	Unit(protocol_team team);
	Unit(protocol_team team, const char* name);

	void spawn(int x, int y, protocol_dep dep); // spawn as specified dep
	void move(protocol_direction direction); // move 1 cell toward the direction
	void attack(protocol_direction direction); // attack toward the direction
	void skill(protocol_direction direction); // use hero skill toward the direction
	void damage(int h = 1); // exert given damage on unit (1 on default)
	void heal(int h = 1); // heal unit with given value (1 on default)
	void stun(int s = 3); // stun unit (3 turns on default)

	void release() { p.state = STATE_IDLE; init(); } // For the first time

	void moveOffDiscard() { moveOffDirection = DIRECTION_NULL; };

	void turn();
	void update();
	void draw() const;

	protocol_unit* getProtocol() { return &p; }

	protocol_team getTeam() { return p.team; }
	protocol_dep getDep() { return p.dep; }
	int getX() { return p.x; }
	int getY() { return p.y; }
	protocol_state getState() { return p.state; }
	int getHealth() { return p.health; }
	bool getHero() { return p.hero; }
	int getCooltime() { return p.cooltime; }
	int getRespawn() { return p.respawn; }

	// bypassing all constraints and animations.
	// just use it for very exceptional cases.
	void setPosition(int x, int y) { p.x = x; p.y = y; }
	void setState(protocol_state state) { p.state = state; }
	void setHealth(int health) { p.health = health; }
	void setHero(bool hero) { p.hero = hero; }
	void setCooltime(int cooltime) { p.cooltime = cooltime; }
	void setRespawn(int respawn) { p.respawn = respawn; }
	void setStun(int stun) { p.stun = stun; }
};

class Flag : public Object {
private:
	protocol_flag p;

public:
	Flag(int x, int y) { p.team = TEAM_NULL; p.x = x; p.y = y; };

	void own(protocol_team team) { p.team = team; } // change its owner

	void turn();
	void update();
	void draw() const;

	protocol_flag* getProtocol() { return &p; }

	protocol_team getTeam() { return p.team; }
	int getX() { return p.x; }
	int getY() { return p.y; }
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

	protocol_poison* getProtocol() { return &p; }

	protocol_team getTeam() { return p.team; }
	int getX() { return p.x; }
	int getY() { return p.y; }
	int getSpan() { return p.span; }
};

class Petal : public Object {
private:
	protocol_petal p;

public:
	Petal();

	void spawn(protocol_team team, int x, int y, protocol_direction direction);
	void turn();
	void update();
	void draw() const;

	protocol_petal* getProtocol() { return &p; }

	protocol_team getTeam() { return p.team; }
	int getX() { return p.x; }
	int getY() { return p.y; }
	int getDirection() { return p.direction; }
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

	protocol_team getTeam() { return p.team; }
	int getX() { return p.x; }
	int getY() { return p.y; }

	protocol_mushroom* getProtocol() { return &p; }
};