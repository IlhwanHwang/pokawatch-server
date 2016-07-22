#include <cmath>
#include <vector>
#include "game.h"
#include "protocol.h"
#include "network.h"
#include "draw.h"
#include "resource.h"
#include "effect.h"
#include "utility.h"

Unit Game::unitArray[UNIT_NUM_MAX] = {					// initialize of units
	Unit(0, MAP_HEIGHT / 2 - 1, TEAM_POSTECH),
	Unit(0, MAP_HEIGHT / 2, TEAM_POSTECH),
	Unit(0, MAP_HEIGHT / 2 + 1, TEAM_POSTECH),
	Unit(MAP_WIDTH - 1, MAP_HEIGHT / 2 - 1, TEAM_KAIST),
	Unit(MAP_WIDTH - 1, MAP_HEIGHT / 2, TEAM_KAIST),
	Unit(MAP_WIDTH - 1, MAP_HEIGHT / 2 + 1, TEAM_KAIST)

};
Poison Game::poisonArray[POISON_NUM_MAX];
Petal Game::petalArray[PETAL_NUM_MAX];
Mushroom Game::mushroomArray[MUSHROOM_NUM_MAX];
protocol_data Game::protocolToSend;
protocol_data * Game::protocolPointer;
protocol_team Game::owner;
int Game::own[2], Game::win[2], Game::extra, Game::elapsed;
int Game::death[2];
int Game::order[UNIT_NUM_MAX];
bool Game::end = false;

void Game::init() // game initialization
{
	owner = TEAM_NULL;
	own[0] = 0;
	own[1] = 0;
	win[0] = 0;
	win[1] = 0;
	extra = 0;
	elapsed = 0;
	death[0] = 0;
	death[1] = 0;
	end = false;

	glutDisplayFunc(draw);

	makeProtocol();
}

void Game::makeProtocol() // protocol data making routine
{
	for (int i = 0; i < UNIT_NUM_MAX; i++) protocolToSend.unit[i] = *(unitArray[i].getProtocol());
	for (int i = 0; i < POISON_NUM_MAX; i++) protocolToSend.poison[i] = *(poisonArray[i].getProtocol());
	for (int i = 0; i < PETAL_NUM_MAX; i++) protocolToSend.petal[i] = *(petalArray[i].getProtocol());
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) protocolToSend.mushroom[i] = *(mushroomArray[i].getProtocol());
	protocolToSend.elapsed= elapsed;
	protocolToSend.owner = owner;
	protocolToSend.own[0] = own[0];
	protocolToSend.own[1] = own[1];
	protocolToSend.win[0] = win[0];
	protocolToSend.win[1] = win[1];
	protocolToSend.extra = extra;

	protocolPointer = &protocolToSend;
}

void Game::update() {	// update routine
	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].update();
	for (int i = 0; i < POISON_NUM_MAX; i++) poisonArray[i].update();
	for (int i = 0; i < PETAL_NUM_MAX; i++) petalArray[i].update();
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) mushroomArray[i].update();

	Effect::update();
}

void Game::drawFaces() {
	const float xoffset = 64;
	const float x[6] = { 
		xoffset, 
		xoffset, 
		xoffset, 
		WINDOW_WIDTH - xoffset,
		WINDOW_WIDTH - xoffset,
		WINDOW_WIDTH - xoffset };
	const float y[6] = { GUI_MAP_Y - 128, GUI_MAP_Y, GUI_MAP_Y + 128, GUI_MAP_Y - 128, GUI_MAP_Y, GUI_MAP_Y + 128 };

	for (int i = 0; i < UNIT_NUM_MAX; i++) {
		Unit& u = unitArray[i];
		Sprite* face = &Rspr::error;
		
		if (u.getState() == STATE_NULL || u.getState() == STATE_DEAD) {
			face = &Rspr::faceDEAD;
		}
		else {
			face = DEP_SELECT(u.getDep(), &Rspr::faceCSE, &Rspr::facePHYS, &Rspr::faceLIFE, &Rspr::faceME, &Rspr::faceCHEM);
		}

		Draw::draw(Rspr::faceFrame, x[i], y[i]);
		Draw::draw(*face, x[i], y[i]);
		if (u.getState() == STATE_DEAD)
			Draw::number(u.getRespawn(), x[i], y[i]);
	}
}

void Game::drawPoint() {
	float x = (POINT_X1 + POINT_X2) / 2.0;
	float y = (POINT_Y1 + POINT_Y2) / 2.0;

	if (owner == TEAM_NULL) {
		Draw::onmap(Rspr::pointNorm, x, y, 0.0);

		if (own[team_to_index(TEAM_POSTECH)] > 0) {
			float alpha = (float)(own[team_to_index(TEAM_POSTECH)]) / POINT_TURN_OWN / 2.0;
			Draw::onmapB(Rspr::pointP, x, y, 0.0, Color::white, alpha);
		}
		if (own[team_to_index(TEAM_KAIST)] > 0) {
			float alpha = (float)(own[team_to_index(TEAM_KAIST)]) / POINT_TURN_OWN / 2.0;
			Draw::onmapB(Rspr::pointK, x, y, 0.0, Color::white, alpha);
		}
	}
	else {
		Sprite& spr = owner == TEAM_POSTECH ? Rspr::pointP : Rspr::pointK;
		Draw::onmap(spr, x, y, 0.0);
	}
}

void Game::drawOverlay() {
	const float flagx = WINDOW_WIDTH * 0.5;
	const float flagy = WINDOW_HEIGHT - 96.0;
	if (owner == TEAM_NULL) {
		Draw::draw(Rspr::flagNull, flagx, flagy);
	}
	if (owner == TEAM_POSTECH) {
		Draw::draw(Rspr::flagP[Gui::aniFullPhaseCombinate(4)], flagx, flagy);
	}
	if (owner == TEAM_KAIST) {
		Draw::draw(Rspr::flagK[Gui::aniFullPhaseCombinate(4)], flagx, flagy);
	}
	
	const float barw = 256.0;
	const float barpx = flagx - 96.0;
	const float barpy = flagy;
	Draw::draw(Rspr::pointBarFrame, barpx - barw / 2.0, barpy);
	Draw::drawS(Rspr::pointBarP, barpx, barpy, (float)win[team_to_index(TEAM_POSTECH)] / POINT_TURN_WIN, 1.0);
	const float barkx = flagx + 96.0;
	const float barky = flagy;
	Draw::draw(Rspr::pointBarFrame, barkx + barw / 2.0, barky);
	Draw::drawS(Rspr::pointBarK, barkx, barky, (float)win[team_to_index(TEAM_KAIST)] / POINT_TURN_WIN, 1.0);
}

void Game::draw() {
	Gui::drawPre();
	Draw::draw(Rspr::bg, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.5);

	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_HEIGHT; j++) {
			if (extra <= 0) {
				Draw::onmapB(Rspr::tileLight, i, j, 0.0, (i + j) % 2 == 0 ? Color::white : Color::lightgray, 1.0);
			}
			else {
				Draw::onmapB(Rspr::tileLight, i, j, 0.0, 
					(i + j) % 2 == 0 ?
						(elapsed % 2 == 0 ? Color::lightgray : Color::cyan) :
						(elapsed % 2 == 1 ? Color::lightgray : Color::magenta),
					1.0);
			}
		}
	}

	drawPoint();

	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].draw();
	for (int i = 0; i < POISON_NUM_MAX; i++) poisonArray[i].draw();
	for (int i = 0; i < PETAL_NUM_MAX; i++) petalArray[i].draw();
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) mushroomArray[i].draw();

	Effect::draw();
	Draw::flush();

	// Overlay informations
	drawFaces();
	drawOverlay();

	//Draw::drawB(Rspr::intengrad, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.91, Color::black, 1.0);
	//Draw::bignumber(elapsed, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.91);

	if (end) {
		if (owner == TEAM_POSTECH)
			Draw::draw(Rspr::winPostech, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.5);
		else if (owner == TEAM_KAIST)
			Draw::draw(Rspr::winKaist, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.5);
		else
			Draw::draw(Rspr::winDraw, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.5);
	}
}

void Game::release() {
	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].release();
	Resource::postinit();
	Draw::setsize(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGHT);
}

void Game::rulePriority() {
	for (int i = 0; i < UNIT_NUM_MAX; i++) {
		// Priority for first team at the even turn, second team otherwise.
		order[i] = (elapsed % 2 == 0) ? i : (i + UNIT_NUM_MAX / 2) % UNIT_NUM_MAX;
	}
}

void Game::ruleCommand() {
	for (int i = 0; i < UNIT_NUM_MAX; i++) {
		Unit& u = unitArray[i];
		protocol_command c = Network::getCommandEnum(i);
		if (command_kind_skill(c)) {
			protocol_direction d = command_to_direction(c);
			u.skill(d);
		}
		if (command_kind_attack(c)) {
			protocol_direction d = command_to_direction(c);
			u.attack(d);
		}
		if (command_kind_spawn(c) && u.getState() == STATE_DEAD)
		{
			u.spawn(command_to_dep(c));
		}
	}
}

void Game::ruleMove() {
	std::vector<int> movable;

	for (int k = 0; k < UNIT_NUM_MAX; k++) {
		const int i = order[k];
		Unit& u = unitArray[i];
		
		if (!u.isAlive())
			continue;

		protocol_command c = Network::getCommandEnum(i);
		if (command_kind_move(c)) {
			movable.push_back(i);
		}
	}

	bool changed = true; // is there any change in the map

	while (changed) {
		changed = false; // assuming no change

		for (int i = 0; i < movable.size(); i++) {
			int ind = movable[i];
			Unit& u = unitArray[ind];
			protocol_command c = Network::getCommandEnum(ind);
			protocol_direction d = command_to_direction(c);
			bool moved = u.move(d);
			bool duplicated = false;

			if (moved) // control for move
			{
				for (int j = 0; j < UNIT_NUM_MAX; j++) {
					if (j == ind)
						continue;
					Unit& other = unitArray[j];
					if (!other.isAlive())
						continue;
					if (other.getX() == u.getX() && other.getY() == u.getY()) {
						duplicated = true;
						break;
					}
				}
			}

			if (duplicated) {
				// if duplicated, then go back
				u.moveResetMovestun();
				u.move(direction_flip(d));
				u.moveOffDiscard();
			}
			else {
				// if succeeded to move, then erase itself from list
				movable.erase(movable.begin() + i);
				i--;
				changed = true; // keep looping
			}
		}
	}

	// Case of CSE's blink
	{
		for (int k = 0; k < UNIT_NUM_MAX; k++) {
			const int i = order[k];
			Unit& u = unitArray[i];

			if (u.getDep() == DEP_CSE && state_kind_attack(u.getState())) {
				const protocol_direction d = state_to_direction(u.getState());
				const protocol_team t = u.getTeam();
				const int x = u.getX();
				const int y = u.getY();
				const int dx = direction_to_dx(d);
				const int dy = direction_to_dy(d);

				bool empty = true;
				const int jump = CSE_BLINK_LENGTH;
				const int jx = clamp(x + dx * jump, 0, MAP_WIDTH - 1);
				const int jy = clamp(y + dy * jump, 0, MAP_HEIGHT - 1);

				for (int j = 0; j < UNIT_NUM_MAX; j++) {
					if (i == j)
						continue;

					Unit& other = unitArray[j];
					if (!other.isAlive())
						continue;

					if (other.getX() == x + dx * jump && other.getY() == y + dy * jump) {
						empty = false;
						break;
					}
				}

				if (empty) {
					Effect::push(new EffectCSEBlink((float)x, (float)y, u.getFliped()));
					u.setPosition(jx, jy);
				}
			}
		}
	}

	// Case of ME's accident
	{
		std::vector<Unit*> cand;

		for (int k = 0; k < UNIT_NUM_MAX; k++) {
			const int i = order[k];
			Unit& u = unitArray[i];
			if (!state_kind_skill(u.getState()) || u.getDep() != DEP_ME)
				continue;

			cand.push_back(&u);
		}

		if (cand.size() == 1) {
			ruleAccident(*cand[0]);
		}
		else if (cand.size() == 2) {
			bool between = false;
			bool aligned = false;

			Unit& u1 = *cand[0];
			Unit& u2 = *cand[1];
			protocol_direction d1 = state_to_direction(u1.getState());
			protocol_direction d2 = state_to_direction(u2.getState());
			const int x1 = u1.getX();
			const int y1 = u1.getY();
			const int x2 = u2.getX();
			const int y2 = u2.getY();

			if (y1 == y2) { // Same Y
				aligned = true;
				for (int i = 0; i < UNIT_NUM_MAX; i++) {
					Unit& other = unitArray[i];
					
					if (!other.isAlive())
						continue;

					const int bx1 = x1 > x2 ? x2 : x1;
					const int bx2 = x1 < x2 ? x2 : x1;
					if (other.getY() == y1 && other.getX() > bx1 && other.getX() < bx2) { // Is in between two
						between = true;
						break;
					}
				}
			}
			if (x1 == x2) { // Same X
				aligned = true;
				for (int i = 0; i < UNIT_NUM_MAX; i++) {
					Unit& other = unitArray[i];

					if (!other.isAlive())
						continue;

					const int by1 = y1 > y2 ? y1 : y2;
					const int by2 = y1 < y2 ? y1 : y2;
					if (other.getX() == x1 && other.getY() > by1 && other.getY() < by2) { // Is in between two
						between = true;
						break;
					}
				}
			}

			if (aligned && !between) {
				const int dx1 = direction_to_dx(d1);
				const int dy1 = direction_to_dy(d1);
				if ((x2 - x1) * dx1 > 0 || (y2 - y1) * dy1 > 0) {
					if (d1 == direction_flip(d2)) {
						// Coliide each other
						const int ax1 = (x1 + x2) / 2;
						const int ay1 = (y1 + y2) / 2;
						const int ax2 = (x1 + x2 + 1) / 2;
						const int ay2 = (y1 + y2 + 1) / 2;
						u1.setPosition(ax1, ay1);
						u2.setPosition(ax2, ay2);
						u1.damage(ME_ACCIDENT_DAMAGE);
						u2.damage(ME_ACCIDENT_DAMAGE);

						switch (d1) {
						case DIRECTION_RIGHT:
							Effect::push(new EffectMEAccidentH(u1.getTeam(), ax1, ay1, ax1 - x1, false));
							Effect::push(new EffectMEAccidentH(u2.getTeam(), ax2, ay2, x2 - ax2, true));
							break;
						case DIRECTION_LEFT:
							Effect::push(new EffectMEAccidentH(u1.getTeam(), ax1, ay1, x1 - ax1, true));
							Effect::push(new EffectMEAccidentH(u2.getTeam(), ax2, ay2, ax2 - x2, false));
							break;
						case DIRECTION_UP:
							Effect::push(new EffectMEAccidentV(u1.getTeam(), ax1, ay1, ay1 - y1, false));
							Effect::push(new EffectMEAccidentV(u2.getTeam(), ax2, ay2, y2 - ay2, true));
							break;
						case DIRECTION_DOWN:
							Effect::push(new EffectMEAccidentV(u1.getTeam(), ax1, ay1, y1 - ay1, false));
							Effect::push(new EffectMEAccidentV(u2.getTeam(), ax2, ay2, ay2 - y2, true));
							break;
						}
					}
					else {
						// Reversed priority. The former is heading the latter.
						ruleAccident(u2);
						ruleAccident(u1);
					}
				}
				else {
					// Normal priority
					ruleAccident(u1);
					ruleAccident(u2);
				}
			}
		}

		cand.clear();
	}

	ruleFlush();
}

void Game::ruleAccident(Unit& u) {
	const int x = u.getX();
	const int y = u.getY();
	const protocol_direction d = state_to_direction(u.getState());
	const int dx = direction_to_dx(d);
	const int dy = direction_to_dy(d);
	int mindist = ARBITRARY_BIG_NUM;
	Unit* target = nullptr;

	for (int i = 0; i < UNIT_NUM_MAX; i++) {
		Unit& other = unitArray[i];

		if (!other.isAlive())
			continue;

		if (other.getX() == x && other.getY() == y)
			continue;

		if (other.getY() == y) {
			if ((other.getX() - x) * dx > 0) {
				int dist = (other.getX() - x) * dx;
				if (mindist > dist) {
					mindist = dist;
					target = &other;
				}
			}
		}
		if (other.getX() == x) {
			if ((other.getY() - y) * dy > 0) {
				int dist = (other.getY() - y) * dy;
				if (mindist > dist) {
					mindist = dist;
					target = &other;
				}
			}
		}
	}

	int ax, ay;

	if (target == nullptr) {
		ax = clamp(x + dx * MAP_WIDTH, 0, MAP_WIDTH - 1);
		ay = clamp(y + dy * MAP_HEIGHT, 0, MAP_HEIGHT - 1);
		u.setPosition(ax, ay);
	}
	else {
		ax = target->getX() - dx;
		ay = target->getY() - dy;
		u.setPosition(ax, ay);
		if (u.getTeam() != target->getTeam())
			target->damage(ME_ACCIDENT_DAMAGE);
	}

	switch (d) {
	case DIRECTION_RIGHT:
		Effect::push(new EffectMEAccidentH(u.getTeam(), ax, ay, ax - x, false));
		break;
	case DIRECTION_LEFT:
		Effect::push(new EffectMEAccidentH(u.getTeam(), ax, ay, x - ax, true));
		break;
	case DIRECTION_UP:
		Effect::push(new EffectMEAccidentV(u.getTeam(), ax, ay, ay - y, false));
		break;
	case DIRECTION_DOWN:
		Effect::push(new EffectMEAccidentV(u.getTeam(), ax, ay, y - ay, true));
		break;
	}
}

void Game::ruleAttack() // rules related to attack
{
	//attack command
	
	/*
	 * CSE	: Stun enemies in the region of 3 X 3 square within 3 turns
	 * PHYS	: Cast a straight wave dealing 1 damage
	 * LIFE	: Spawn a petal dealing 1 damage to enemies and 1 heal to friends
	 * ME	: Damage 3 X 3 neighbors
	 * CHEM	: Spread chemicals
	 */

	for (int k = 0; k < UNIT_NUM_MAX; k++) {
		const int i = order[k];
		Unit& u = unitArray[i];

		if (!state_kind_attack(u.getState()) && u.getDep() != DEP_ME && u.getDep() != DEP_CSE)
			continue;

		const protocol_direction d = state_to_direction(u.getState());
		const protocol_team t = u.getTeam();
		const int x = u.getX();
		const int y = u.getY();
		const int dx = direction_to_dx(d);
		const int dy = direction_to_dy(d);

		if (u.getDep() == DEP_CSE) {
			if (elapsed % CSE_SPARK_COOLTIME == 0) {
				regionStun(team_invert(t), x - 1, y - 1, x + 1, y + 1, CSE_SPARK_STUN);
				Effect::push(new EffectCSEAttack(t, (float)x, (float)y));
			}
		}
		else if (u.getDep() == DEP_PHYS) {
			regionDamage(team_invert(t), x, y, x + dx * MAP_WIDTH, y + dy * MAP_HEIGHT, PHYS_WAVE_DAMAGE);
			Effect::push(
				new EffectPHYSAttack(
					t, 
					(float)(x + dx), 
					(float)(y + dy), 
					clamp((float)(x + dx * MAP_WIDTH), 0.0, (float)MAP_WIDTH - 1.0), 
					clamp((float)(y + dy * MAP_HEIGHT), 0.0, (float)MAP_HEIGHT - 1.0))
			);
		}
		else if (u.getDep() == DEP_LIFE) {
			int indexForValidPetal = getValidPetalIndex();
			petalArray[indexForValidPetal].spawn(t, x + dx, y + dy, d);
		}
		else if (u.getDep() == DEP_ME) {
			regionDamage(team_invert(t), x - 1, y - 1, x + 1, y + 1, ME_THORN_DAMAGE);
		}
		else if (u.getDep() == DEP_CHEM) {
			for (int p = 1; p <= CHEM_POISON_LENGTH; p++)
			{
				int indexForValidPoison = getValidPoisonIndex();
				poisonArray[indexForValidPoison].spawn(t, x + dx * p, y + dy * p);
			}
		}
	} // end of attack

	ruleFlush();
}

void Game::ruleCollide() {
	for (int k = 0; k < UNIT_NUM_MAX; k++) {
		const int i = order[k];
		Unit& u = unitArray[i];

		if (!u.isAlive())
			continue;

		int x = u.getX();
		int y = u.getY();
		protocol_team t = u.getTeam();

		for (int j = 0; j < PETAL_NUM_MAX; j++) {
			Petal& p = petalArray[j];
			if (p.isValid() && x == p.getX() && y == p.getY())
			{
				if (p.getTeam() == t) {
					u.heal(LIFE_PETAL_HEAL);
				}
				else {
					u.damage(LIFE_PETAL_DAMAGE);
				}
				p.invalidate();
			}
		}

		for (int j = 0; j < POISON_NUM_MAX; j++) {
			Poison& p = poisonArray[j];
			if (p.isValid() && t != p.getTeam() && x == p.getX() && y == p.getY())
			{
				u.damage(CHEM_POISON_DAMAGE);
			}
		}

		for (int j = 0; j < MUSHROOM_NUM_MAX; j++) {
			Mushroom& m = mushroomArray[j];
			if (m.isValid() && t != m.getTeam() && x == m.getX() && y == m.getY())
			{
				u.damage(CHEM_MUSHROOM_DAMAGE);
				m.invalidate();
			}
		}
	}

	ruleFlush();
}

void Game::ruleSkill() // rules related to skill
{
	//skill command

	/*
	* CSE	: Stun enemies in the region of 7 X 7 square within 5 turns 
	* PHYS	: Spawn a black hole
	* LIFE	: Heal all friends
	* ME	: Assault to the direction and kill the very first enemy on the way
	* CHEM	: Spawn a mushroom
	*/

	for (int k = 0; k < UNIT_NUM_MAX; k++) {
		const int i = order[k];
		Unit& u = unitArray[i];
		
		if (!state_kind_skill(u.getState()))
			continue;

		protocol_direction d = state_to_direction(u.getState());
		protocol_team t = u.getTeam();
		int x = u.getX();
		int y = u.getY();
		int dx = direction_to_dx(d);
		int dy = direction_to_dy(d);

		if (u.getDep() == DEP_CSE) {
			regionStun(
				team_invert(t), 
				x - CSE_STORM_RANGE, 
				y - CSE_STORM_RANGE, 
				x + CSE_STORM_RANGE, 
				y + CSE_STORM_RANGE, 
				CSE_STORM_STUN
			);
		}
		else if (u.getDep() == DEP_PHYS) {
			regionDamage(
				team_invert(t), 
				x + dx * 2 - 1, 
				y + dy * 2 - 1, 
				x + dx * 2 + 1, 
				y + dy * 2 + 1, 
				PHYS_BLACKHOLE_DAMAGE
			);
			Effect::push(new EffectBlackhole(t, x + dx * 2, y + dy * 2));
		}
		else if (u.getDep() == DEP_LIFE) {
			regionHealAll(t, LIFE_BLOSSOM_HEAL);
		}
		else if (u.getDep() == DEP_ME) {
			// ME's skill is implemented in move session
		}
		else if (u.getDep() == DEP_CHEM) {
			int indexForValidMushroom = getValidMushroomIndex();
			mushroomArray[indexForValidMushroom].spawn(t, x + dx, y + dy);
		}
	} // end of skill

	ruleFlush();
}

void Game::ruleSpawn() // rules related to spawn
{
	for (int k = 0; k < UNIT_NUM_MAX; k++) {
		const int i = order[k];
		Unit& u = unitArray[i];

		if (!u.checkRespawned())
			continue;
		
		for (int j = 0; j < UNIT_NUM_MAX; j++) {
			if (j == i)
				continue;

			Unit& other = unitArray[j];
			if (other.getX() == u.getX() && other.getY() == u.getY()) {
				other.damage(ARBITRARY_BIG_NUM);
			}
		}
	}

	ruleFlush();
}

void Game::rulePoint() {
	bool point[2] = { false, false };
	
	for (int k = 0; k < UNIT_NUM_MAX; k++) {
		const int i = order[k];
		Unit& u = unitArray[i];
		if (u.getX() >= POINT_X1 && u.getY() >= POINT_Y1 &&
			u.getX() <= POINT_X2 && u.getY() <= POINT_Y2) {
			point[team_to_index(u.getTeam())] = true;
		}
	}

	for (int i = 0; i < 2; i++) {
		int j = i == 0 ? 1 : 0;

		if (point[i] && !point[j]) {
			if (owner == TEAM_NULL) {
				own[j] = 0;
				if (own[i] < POINT_TURN_OWN) {
					// Owning the first point
					own[i]++;
				}
				else {
					// Owned the first point
					owner = index_to_team(i);
					Gui::shake(2.0);
					Effect::push(new EffectOwn(GUI_MAP_X, GUI_MAP_Y));
					Effect::push(new EffectOwnFlag(WINDOW_WIDTH * 0.5, WINDOW_HEIGHT - 96.0));
				}
			}
			if (owner == index_to_team(j)) {
				own[j] = 0;
				if (own[i] < POINT_TURN_OWN) {
					// Owning the first point
					own[i]++;
				}
				else {
					// Owned the first point
					owner = index_to_team(i);
				}
			}
		}

		if (!point[i]) {
			if (own[i] > 0)
				own[i]--;
		}

		if (owner == index_to_team(i)) {
			if (win[i] < POINT_TURN_WIN) {
				// Winning the point
				win[i]++;
			}
			else {
				if (point[j]) {
					// Extra time
					extra = POINT_TURN_EXTRA;
				}
				else {
					if (extra > 0) {
						// Extra time expiring
						extra--;
					}
					else {
						// Extra time expired
						end = true;
					}
				}
			}
		}
	}
}

void Game::ruleFlush() {
	for (int i = 0; i < UNIT_NUM_MAX; i++) {
		Unit& u = unitArray[i];

		u.flush();
		if (u.checkDead()) {
			death[team_to_index(u.getTeam())]++;
			Effect::push(new EffectDeath(u.getTeam(), u.getX(), u.getY()));
			Gui::shake(1.0);
			if (death[team_to_index(u.getTeam())] % HERO_PERIOD == 0) {
				u.setHero(true);
			}
		}
	}
}

void Game::turn() {
	if (end) {
		for (int i = 0; i < UNIT_NUM_MAX; i++) {
			unitArray[0].setState(STATE_IDLE);
		}
		return;
	}
	elapsed++;

	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].turn();
	for (int i = 0; i < POISON_NUM_MAX; i++) poisonArray[i].turn();
	for (int i = 0; i < PETAL_NUM_MAX; i++) petalArray[i].turn();
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) mushroomArray[i].turn();

	// things are done by rules
	rulePriority();
	ruleCommand();
	ruleMove();
	ruleSpawn();
	ruleSkill();
	ruleAttack();
	ruleCollide();
	rulePoint();
	
	if (Network::getMode() == MODE_SERVER) {
		makeProtocol();
	}
}

void Game::regionApply(protocol_team t, int x1, int y1, int x2, int y2, int damage, int heal, int stun) {
	if (x1 > x2) {
		int tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	
	if (y1 > y2) {
		int tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		Unit& u = unitArray[i];
		if (u.getTeam() == t) {
			if (u.getX() >= x1 && u.getX() <= x2 && u.getY() >= y1 && u.getY() <= y2) {
				u.damage(damage);
				u.heal(heal);
				u.stun(stun);
			}
		}
	}
}

int Game::getValidPoisonIndex()
{
	int b;
	for ( b = 0; b < POISON_NUM_MAX; b++)
	{
		if (poisonArray[b].getProtocol()->valid == false)
			break;
	}
	if (b < POISON_NUM_MAX) return b;
	else return INVALID_POISON_INDEX;
}

int Game::getValidMushroomIndex()
{
	int b;
	for (b = 0; b < MUSHROOM_NUM_MAX; b++)
	{
		if (mushroomArray[b].getProtocol()->valid == false)
			break;
	}
	if (b < MUSHROOM_NUM_MAX) return b;
	else return INVALID_MUSHROOM_INDEX;
}

int Game::getValidPetalIndex()
{
	int b;
	for (b = 0; b < PETAL_NUM_MAX; b++)
	{
		if (petalArray[b].getProtocol()->valid == false)
			break;
	}
	if (b < PETAL_NUM_MAX) return b;
	else return INVALID_PETAL_INDEX;
}
