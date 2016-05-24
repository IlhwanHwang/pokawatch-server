//
// CNH 2016.05.19
// unit class
//

#include "unit.h"
#include "resource.h"
#include "draw.h"
#include "spline.h"
#include "game.h"
#include "key.h"

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
	healthPrevious = p.health;
	p.stun = 0;
	p.cooltime = 0;
	p.x = orgx;
	p.y = orgy;
	p.invincible = INVINCIBLE_SPAN;
	p.state = STATE_IDLE;

	animationFlip = p.x > MAP_WIDTH / 2 ? true : false;
}

Unit::Unit(int x, int y, protocol_team team) : Unit(x, y, team, "Unnamed unit") {}

Unit::Unit(int x, int y, protocol_team team, const char* name) {
	p.team = team;
	p.state = STATE_NULL;
	p.respawn = 0;
	orgx = x;
	orgy = y;
	p.x = orgx;
	p.y = orgy;
	death = 0;

	this->name = name;
}

void Unit::spawn(protocol_dep dep) {
	p.dep = dep;
}

bool Unit::move(protocol_direction direction) {
	if (p.state == STATE_NULL)
		return false;

	if (moveStun > 0) {
		error("Cannot move in consecutive turns");
		return false;
	}
	if (p.state == STATE_STUN) {
		error("Tried to move when stunned");
		return false;
	}

	int dx = 0;
	int dy = 0;

	switch (direction) {
	case DIRECTION_RIGHT: dx = 1; animationFlip = false; break;
	case DIRECTION_UP:    dy = 1;  break;
	case DIRECTION_LEFT:  dx = -1; animationFlip = true; break;
	case DIRECTION_DOWN:  dy = -1; break;
	default: error("Invalid input"); break;
	}

	p.x += dx;
	p.y += dy;
	moveOffAction = true;

	if (p.x < 0 || p.x >= MAP_WIDTH || p.y < 0 || p.y >= MAP_HEIGHT) {
		p.x -= dx;
		p.y -= dy;
		moveOffDirection = DIRECTION_NULL;
		error("Tried to move to the outside of the map");
		return false;
	}
	else {
		moveOffDirection = direction;
		if (p.dep == DEP_ME) {
			moveStun = 2;
		}
	}

	return true;
}

void Unit::attack(protocol_direction direction) {
	if (p.state == STATE_NULL)
		return;

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
	case DIRECTION_RIGHT:	p.state = STATE_ATTACK_RIGHT;   animationFlip = false;	break;
	case DIRECTION_UP:		p.state = STATE_ATTACK_UP;		break;
	case DIRECTION_LEFT:	p.state = STATE_ATTACK_LEFT;   animationFlip = true;	break;
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
	if (p.state == STATE_NULL)
		return;

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
	case DIRECTION_RIGHT:	p.state = STATE_SKILL_RIGHT;   animationFlip = false;	break;
	case DIRECTION_UP:		p.state = STATE_SKILL_UP;		break;
	case DIRECTION_LEFT:	p.state = STATE_SKILL_LEFT;   animationFlip = true;	break;
	case DIRECTION_DOWN:	p.state = STATE_SKILL_DOWN;	break;
	default: error("Invalid input"); break;
	}

	p.hero = false;
}

void Unit::damage(int h) {
	if (p.state == STATE_NULL)
		return;

	if (p.state == STATE_DEAD) {
		error("Cannot damage dead unit");
		return;
	}

	if (p.invincible > 0) {
		error("Cannot damage invincible unit");
		return;
	}

	if (p.health == healthPrevious)
		healthPrevious = p.health;
	p.health -= h;

	if (p.health <= 0) {
		kill();
	}
}

void Unit::heal(int h) {
	if (p.state == STATE_NULL)
		return;
	
	if (p.state == STATE_DEAD) {
		error("Cannot heal dead unit");
		return;
	}

	p.health += h;

	if (p.health > healthMax) {
		p.health = healthMax;
	}
}

void Unit::stun(int s) {
	if (p.state == STATE_NULL)
		return;

	p.stun = s;
	if (s > 0) {
		p.state = STATE_STUN;
	}
}

void Unit::kill() {
	p.state = STATE_DEAD;
	p.respawn = RESPAWN_COOLTIME;
	p.hero = false;
	death++;
	if (p.team == TEAM_POSTECH) Game::setDeath(TEAM_POSTECH - 1, Game::getDeath(TEAM_POSTECH - 1) + 1);
	if (p.team == TEAM_KAIST) Game::setDeath(TEAM_KAIST - 1, Game::getDeath(TEAM_KAIST - 1) + 1);
	p.dep = DEP_NULL;
}

void Unit::turn() {
	if (p.state == STATE_NULL)
		return;

	moveOffAction = false;
	healthPrevious = p.health;

	if (p.state == STATE_DEAD) {
		p.health = 0;
		if (p.respawn > 0) {
			p.respawn--;
		}
		else {
			if (p.dep != DEP_NULL) {
				init();
			}
		}
	}
	else {
		p.state = STATE_IDLE;

		if (p.invincible > 0) {
			p.invincible--;
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
}

void Unit::update() {
	if (p.state == STATE_NULL)
		return;

	// Animation by moving
	if (moveOffAction) {
		float dx = 0.0, dy = 0.0;
		switch (moveOffDirection) {
		case DIRECTION_RIGHT: dx = -1.0; break;
		case DIRECTION_UP: dy = -1.0; break;
		case DIRECTION_LEFT: dx = 1.0; break;
		case DIRECTION_DOWN: dy = 1.0; break;
		}

		float mag = 1.0 - Spline::accandfric(Gui::aniPhase());
		dx *= mag;
		dy *= mag;

		moveOffX = dx;
		moveOffY = dy;
		moveOffZ = Spline::accjump(Gui::aniPhase()) * 0.2;
	}
	else {
		moveOffX = 0.0;
		moveOffY = 0.0;
		moveOffZ = 0.0;
	}
	// End of animation by moving
}

void Unit::draw() const {
	Sprite* face = &Rspr::error;
	Sprite* body = &Rspr::error;;

	if (p.state == STATE_NULL || p.state == STATE_DEAD) {
		face = &Rspr::faceDEAD;
	}
	else {
		switch (p.dep) {
		case DEP_CSE: face = &Rspr::faceCSE; break;
		case DEP_CHEM: face = &Rspr::faceCHEM; break;
		case DEP_ME: face = &Rspr::faceME; break;
		case DEP_LIFE: face = &Rspr::faceLIFE; break;
		case DEP_PHYS: face = &Rspr::facePHYS; break;
		}
	}

	float x = (orgx - MAP_WIDTH / 2) * GUI_CELL_WIDTH * 1.5 + GUI_MAP_X;
	float y = (orgy - MAP_HEIGHT / 2) * GUI_CELL_HEIGHT * 3.0 + GUI_MAP_Y;

	Draw::draw(Rspr::faceFrame, x, y);
	Draw::draw(*face, x, y);
	if (p.state == STATE_DEAD)
		Draw::number(p.respawn, x, y);

	if (p.state == STATE_NULL || (p.state == STATE_DEAD && p.health == healthPrevious)) {
		return;
	}

	// Unit body
	switch (p.dep) {
	case DEP_CSE: body = &Rspr::unitCSE; break;
	case DEP_CHEM: body = &Rspr::unitCHEM; break;
	case DEP_ME: body = &Rspr::unitME; break;
	case DEP_LIFE: body = &Rspr::unitLIFE; break;
	case DEP_PHYS: body = &Rspr::unitPHYS; break;
	}

	const float drawx = (float)p.x + moveOffX;
	const float drawy = (float)p.y + moveOffY;

	Color c = Color::white;

	if (p.invincible > 0 || (p.health != healthPrevious && Gui::aniPhase() >= 1.0)) {
		c = Gui::aniIndpPhaseCombinate(2, 0.3) == 0 ? Color::gray : Color::white;
	}

	Draw::qonmapSB(*body, 0.0, drawx, drawy, moveOffZ, animationFlip ? -1.0 : 1.0, 1.0, c, 1.0);
	if (p.hero) {
		Draw::qonmap(Rspr::hero, -0.05, drawx, drawy, moveOffZ);
	}
	if (p.state == STATE_STUN) {
		Draw::qonmap(Rspr::stun[Gui::aniIndpPhaseCombinate(4, 0.1)], 0.05, drawx, drawy, moveOffZ + 1.2);
	}

	// Attack motion
	drawAttackMotion();

	// Health status
	int showHealth = Gui::aniPhase() < 1.0 ? healthPrevious : p.health;
	float ddx = 16 / GUI_CELL_WIDTH;
	float dx = -(float)(showHealth - 1) / 2.0 * ddx;
	for (int i = 0; i < showHealth; i++) {
		Draw::qonmap(Rspr::unitHeart, 0.1, drawx + dx, drawy, moveOffZ + 1.5);
		dx += ddx;
	}
}

void Unit::drawAttackMotion() const {
	if (STATE_KIND_ATTACK(p.state)) {
		if (p.dep == DEP_PHYS) {
			int ind = Gui::aniIndpPhaseCombinate(4, 0.1);

			switch (p.state) {
			case STATE_ATTACK_RIGHT:
				for (int x = p.x + 1; x < MAP_WIDTH; x++)
					Draw::qonmap(Rspr::beamH[ind], 0.0, x, p.y, 0.5);
				break;
			case STATE_ATTACK_LEFT:
				for (int x = p.x - 1; x >= 0; x--)
					Draw::qonmap(Rspr::beamH[3 - ind], 0.0, x, p.y, 0.5);
				break;
			case STATE_ATTACK_UP:
				for (int y = p.y + 1; y < MAP_HEIGHT; y++)
					Draw::qonmap(Rspr::beamV[ind], 0.0, p.x, y, 0.5);
				break;
			case STATE_ATTACK_DOWN:
				for (int y = p.y - 1; y >= 0; y--)
					Draw::qonmap(Rspr::beamV[3 - ind], 0.0, p.x, y, 0.5);
				break;
			}
		}

		if (p.dep == DEP_CSE) {
			if (Gui::aniPhase() < 1.0) {
				Draw::qonmap(Rspr::spark[Gui::aniPhaseCombinate(4)], -0.01, p.x, p.y, 0.0);
			}
		}
	}

	if (STATE_KIND_SKILL(p.state)) {
		if (p.dep == DEP_CSE) {
			if (Gui::aniPhase() < 1.0) {
				Sprite& spr = Rspr::sparkboom[Gui::aniPhaseCombinate(4)];
				int x1, y1, x2, y2;

				x1 = p.x - 3;
				y1 = p.y - 3;
				x2 = p.x + 3;
				y2 = p.y + 3;

				x1 = x1 < 0 ? 0 : x1;
				x2 = x2 >= MAP_WIDTH ? MAP_WIDTH - 1 : x2;
				y1 = y1 < 0 ? 0 : y1;
				y2 = y2 >= MAP_HEIGHT ? MAP_HEIGHT - 1 : y2;

				for (int i = x1; i <= x2; i++) {
					for (int j = y1; j <= y2; j++) {
						Draw::qonmap(spr, -0.01, i, j, 0.0);
					}
				}
			}
		}
	}
}

void Flag::turn() {

}

void Flag::update() {

}

void Flag::draw() const {
	Sprite* f;
	switch (p.team) {
	case TEAM_NULL:
		f = &Rspr::flagNull;
		break;
	case TEAM_POSTECH:
		f = &Rspr::flagPostech;
		break;
	case TEAM_KAIST:
		f = &Rspr::flagKaist;
		break;
	}
	Draw::onmap(*f, (float)p.x, (float)p.y, 0.0);
}

Poison::Poison() {
	p.valid = false;
	name = "Poison";
}

void Poison::spawn(protocol_team team, int x, int y) {
	if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT) {
		error("Spawned outside");
		return;
	}

	if (p.valid) {
		error("Already spawned instance");
		return;
	}

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

	Draw::qonmap(Rspr::poison, -1.0, p.x, p.y, 0.0);
}

Petal::Petal() {
	p.valid = false;
	name = "Petal";
}

void Petal::spawn(protocol_team team, int x, int y, protocol_direction direction) {
	/*
	if (p.x < 0 || p.y < 0 || p.x >= MAP_WIDTH || p.y >= MAP_HEIGHT) {
		error("Spawned outside");
		return;
	}
	*/
	if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT) {
		error("Spawned outside");
		return;
	}


	if (p.valid) {
		error("Already spawned instance");
		return;
	}

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

	Draw::qonmap(Rspr::petal, 0.0, p.x, p.y, 0.5);
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