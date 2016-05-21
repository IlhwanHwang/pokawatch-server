//
// CNH 2016.05.19
// unit class
//

#include "unit.h"

void Unit::init() {
	switch (p.dep) {
	case DEP_CSE:  healthMax = 3; break;
	case DEP_PHYS: healthMax = 3; break;
	case DEP_LIFE: healthMax = 3; break;
	case DEP_ME:   healthMax = 5; break;
	case DEP_CHEM: healthMax = 3; break;
	default: error("Invalid department"); break;
	}

	moveStun = 0;
	p.health = healthMax;
	p.stun = 0;
	p.cooltime = 0;
}

Unit::Unit(protocol_team team) : Unit(team, "Unnamed unit") {}

Unit::Unit(protocol_team team, const char* name) {
	p.team = team;
	p.state = STATE_NULL;
	p.respawn = 1;
	this->name = name;
}

void Unit::spawn(int x, int y, protocol_dep dep) {
	p.x = x;
	p.y = y;
	p.dep = dep;
}

void Unit::move(protocol_direction direction) {
	if (moveStun > 0) {
		error("Cannot move in consecutive turns");
		return;
	}
	if (p.state == STATE_STUN) {
		error("Tried to move when stunned");
		return;
	}

	int dx = 0;
	int dy = 0;

	switch (direction) {
	case DIRECTION_RIGHT: dx = 1;  break;
	case DIRECTION_UP:    dy = 1;  break;
	case DIRECTION_LEFT:  dx = -1; break;
	case DIRECTION_DOWN:  dy = -1; break;
	default: error("Invalid input"); break;
	}

	p.x += dx;
	p.y += dy;

	if (p.x < 0 || p.x >= MAP_WIDTH || p.y < 0 || p.y >= MAP_HEIGHT) {
		p.x -= dx;
		p.y -= dy;
		error("Tried to move to the outside of the map");
	}
	else {
		if (p.dep == DEP_ME) {
			moveStun = 1;
		}
	}
}

void Unit::attack(protocol_direction direction) {
	if (p.state == STATE_DEAD) {
		error("Tried to attack when dead");
		return;
	}
	if (p.state == STATE_STUN) {
		error("Tried to attack when stunned");
		return;
	}
	if (p.cooltime > 0) {
		error("Tried to attack in cooltime");
		return;
	}
	if (p.dep == DEP_ME) {
		error("ME cannot attack");
		return;
	}

	switch (direction) {
	case DIRECTION_RIGHT:	p.state = STATE_ATTACK_RIGHT;	break;
	case DIRECTION_UP:		p.state = STATE_ATTACK_UP;		break;
	case DIRECTION_LEFT:	p.state = STATE_ATTACK_LEFT;	break;
	case DIRECTION_DOWN:	p.state = STATE_ATTACK_DOWN;	break;
	default: std::cerr << name << ": Invalid input" << std::endl; break;
	}

	switch (p.dep) {
	case DEP_CSE:  p.cooltime = 6;  break;
	case DEP_PHYS: p.cooltime = 0;  break;
	case DEP_LIFE: p.cooltime = 10; break;
	case DEP_ME:   p.cooltime = 0;  break;
	case DEP_CHEM: p.cooltime = 4;  break;
	default: std::cerr << name << ": Invalid department" << std::endl; break;
	}
}

void Unit::skill(protocol_direction direction) {
	if (p.state == STATE_DEAD) {
		error("Tried to use hero skill when dead");
		return;
	}
	if (p.state == STATE_STUN) {
		error("Tried to use hero skill when stunned");
		return;
	}
	if (!p.hero) {
		error("Non-hero tried to use hero skill");
		return;
	}

	switch (direction) {
	case DIRECTION_RIGHT:	p.state = STATE_SKILL_RIGHT;	break;
	case DIRECTION_UP:		p.state = STATE_SKILL_UP;		break;
	case DIRECTION_LEFT:	p.state = STATE_SKILL_LEFT;	break;
	case DIRECTION_DOWN:	p.state = STATE_SKILL_DOWN;	break;
	default: error("Invalid input"); break;
	}

	p.hero = false;
}

void Unit::damage(int h) {
	p.health -= h;

	if (p.health <= 0) {
		p.state = STATE_DEAD;
		p.respawn = RESPAWN_COOLTIME;
	}
}

void Unit::heal(int h) {
	p.health += h;

	if (p.health > healthMax) {
		p.health = healthMax;
	}
}

void Unit::stun(int s) {
	p.stun = s;
	if (s > 0) {
		p.state = STATE_STUN;
	}
}

void Unit::turn() {
	p.state = STATE_IDLE;

	if (p.respawn > 0) {
		p.respawn--;
		if (p.respawn > 0) {
			p.state = STATE_DEAD;
			p.health = 0;
			return;
		}
		else {
			init();
		}
	}

	if (p.cooltime > 0) {
		p.cooltime--;
	}

	if (moveStun > 0) {
		moveStun--;
	}

	if (p.stun > 0) {
		p.stun--;
		if (p.stun > 0) {
			p.state = STATE_STUN;
			return;
		}
	}
}

void Unit::update() {

}

void Unit::draw() const {

}

void Flag::turn() {

}

void Flag::update() {

}

void Flag::draw() const {

}

Poison::Poison() {
	p.valid = false;
	name = "Poison";
}

void Poison::spawn(protocol_team team, int x, int y) {
	p.team = team;
	p.x = x;
	p.y = y;
	p.span = 4;
	p.valid = true;
}

void Poison::turn() {
	if (!p.valid)
		return;

	if (p.span > 0) {
		p.span--;
	}
	else {
		p.valid = false;
	}
}

void Poison::update() {
	if (!p.valid)
		return;

}

void Poison::draw() const {
	if (!p.valid)
		return;

}

Petal::Petal() {
	p.valid = false;
	name = "Petal";
}

void Petal::spawn(protocol_team team, int x, int y, protocol_direction direction) {
	p.team = team;
	p.x = x;
	p.y = y;
	p.direction = direction;
	p.valid = true;
}

void Petal::turn() {
	if (!p.valid)
		return;

	int dx = 0;
	int dy = 0;

	switch (p.direction) {
	case DIRECTION_RIGHT: dx = 1;  break;
	case DIRECTION_UP:    dy = 1;  break;
	case DIRECTION_LEFT:  dx = -1; break;
	case DIRECTION_DOWN:  dy = -1; break;
	default: error("Invalid direction"); break;
	}

	p.x += dx;
	p.y += dy;

	if (p.x < 0 || p.x >= MAP_WIDTH || p.y < 0 || p.y >= MAP_HEIGHT) {
		p.valid = false;
	}
}

void Petal::update() {
	if (!p.valid)
		return;

}

void Petal::draw() const {
	if (!p.valid)
		return;

}

Mushroom::Mushroom() {
	p.valid = false;
	name = "Mushroom";
}

void Mushroom::spawn(protocol_team team, int x, int y) {
	p.team = team;
	p.x = x;
	p.y = y;
	p.valid = true;
}

void Mushroom::turn() {
	if (!p.valid)
		return;

}

void Mushroom::update() {
	if (!p.valid)
		return;

}

void Mushroom::draw() const {
	if (!p.valid)
		return;

}