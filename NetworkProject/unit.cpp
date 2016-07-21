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
	// Different health values for different units.
	healthMax = DEP_SELECT(p.dep, 3, 3, 3, 5, 3);
	moveStun = 0;
	p.health = healthMax;
	healthPrevious = p.health;
	p.stun = 0;
	p.cooltime = 0;
	p.x = orgx;
	p.y = orgy;
	p.invincible = INVINCIBLE_SPAN; // Invincible for a while after spawning
	p.state = STATE_IDLE;

	animationFlip = p.x > MAP_WIDTH / 2 ? true : false;
	aniInvincible = false;
	deadBlind = false;
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

	int dx = direction_to_dx(direction);
	int dy = direction_to_dy(direction);
	flip(direction);

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
		moveOffDirection = direction_flip(direction);
		if (p.dep == DEP_ME) {
			moveStun = 2; // ME unit moves 1 cell per 2 turns.
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

	p.state = direction_to_attackstate(direction);
	flip(direction);
	p.cooltime = DEP_SELECT(p.dep, 6, 0, 10, 0, 4);
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

	p.state = direction_to_skillstate(direction);
	flip(direction);

	// A hero can use its skill only once.
	p.hero = false;
}

void Unit::damage(int h) {
	if (h <= 0)
		return;

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

	damaged += h;
	aniDamaged = true;
}

void Unit::heal(int h) {
	if (h <= 0)
		return;

	if (p.state == STATE_NULL)
		return;
	
	if (p.state == STATE_DEAD) {
		error("Cannot heal dead unit");
		return;
	}

	healed += h;
	aniHealed = true;
}

void Unit::stun(int s) {
	if (s <= 0)
		return;

	if (p.state == STATE_NULL)
		return;	

	p.stun += s;
}

void Unit::kill() {
	if (p.state == STATE_DEAD)
		return;

	p.state = STATE_DEAD;
	p.respawn = RESPAWN_COOLTIME;
	p.hero = false;
	death++;
	Game::setDeath(team_to_index(p.team), Game::getDeath(team_to_index(p.team)) + 1);
}

void Unit::turninit() {
	healthPrevious = p.health;
	moveOffAction = false;
	aniDamaged = false;
	aniHealed = false;
	healed = 0;
	damaged = 0;
}

void Unit::turn() {
	if (p.state == STATE_NULL)
		return;

	turninit();

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

void Unit::flush() {
	if (p.state == STATE_DEAD)
		return;

	p.health += healed - damaged;

	if (p.health <= 0) {
		kill();
	}

	if (p.health > healthMax) {
		p.health = healthMax;
	}
}

void Unit::postturn() {
}

void Unit::update() {
	if (p.state == STATE_NULL)
		return;

	// Animation by moving
	if (moveOffAction) {
		float dx = (float)direction_to_dx(moveOffDirection);
		float dy = (float)direction_to_dy(moveOffDirection);

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

	if (p.state == STATE_DEAD && Gui::isPost())
		deadBlind = true;
}

void Unit::draw() const {
	Sprite* body = &Rspr::error;

	// Nothing is drawn on map unless alive
	if (p.state == STATE_NULL || deadBlind) {
		return;
	}

	// Unit body
	if (p.team == TEAM_POSTECH) {
		body = DEP_SELECT(
			p.dep, 
			&Rspr::unitCSEP, 
			&Rspr::unitPHYSP, 
			&Rspr::unitLIFEP, 
			&Rspr::unitMEP, 
			&Rspr::unitCHEMP);
	}
	if (p.team == TEAM_KAIST) {
		body = DEP_SELECT(
			p.dep,
			&Rspr::unitCSEK,
			&Rspr::unitPHYSK,
			&Rspr::unitLIFEK,
			&Rspr::unitMEK,
			&Rspr::unitCHEMK);
	}

	const float drawx = (float)p.x + moveOffX;
	const float drawy = (float)p.y + moveOffY;

	Color c = Color::white;

	// Blinking effect
	if (p.invincible > 0 || (aniDamaged && Gui::isPost())) {
		c = Gui::aniIndpPhaseCombinate(2, 0.3) == 0 ? Color::gray : Color::white;
	}

	Draw::qonmapSB(*body, 0.0, drawx, drawy, moveOffZ, animationFlip ? -1.0 : 1.0, 1.0, c, 1.0);
	if (p.hero) {
		Draw::qonmap(Rspr::hero, -0.05, drawx, drawy, moveOffZ);
	}
	if (p.state == STATE_STUN) {
		Draw::qonmap(Rspr::stun[Gui::aniIndpPhaseCombinate(4, 0.1)], 0.05, drawx, drawy, moveOffZ + 1.2);
	}

	// Health status
	int showHealth = Gui::isPost() ? p.health : healthPrevious;
	float ddx = 16 / GUI_CELL_WIDTH;
	float dx = -(float)(showHealth - 1) / 2.0 * ddx;
	for (int i = 0; i < showHealth; i++) {
		Draw::qonmap(Rspr::unitHeart, 0.1, drawx + dx, drawy, moveOffZ + 1.5);
		dx += ddx;
	}
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

	int dx = direction_to_dx(p.direction);
	int dy = direction_to_dy(p.direction);

	p.x += dx;
	p.y += dy;

	if (p.x < 0 || p.x >= MAP_WIDTH || p.y < 0 || p.y >= MAP_HEIGHT) {
		p.valid = false;
	}
}

void Petal::update() {
	if (!p.valid)
		return;

	float dx = (float)direction_to_dx(p.direction);
	float dy = (float)direction_to_dy(p.direction);

	float mag = 1.0 - Spline::accandfric(Gui::aniPhase());
	dx *= mag;
	dy *= mag;

	moveOffX = dx;
	moveOffY = dy;
}

void Petal::draw() const {
	if (!p.valid)
		return;

	Draw::qonmap(Rspr::petal, 0.0, p.x + moveOffX, p.y + moveOffY, 0.5);
}

Mushroom::Mushroom() {
	p.valid = false;
	name = "Mushroom";
}

void Mushroom::spawn(protocol_team team, int x, int y) {
	if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT) {
		error("Spawned outside");
		return;
	}

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

	Draw::qonmap(Rspr::mushroom, -0.01, p.x, p.y, 0.0);
}