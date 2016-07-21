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

	protocolPointer = &protocolToSend;
}

void Game::update() {	// update routine
	if (end)
		return;

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
		ACTUAL_WINDOW_WIDTH - xoffset, 
		ACTUAL_WINDOW_WIDTH - xoffset,
		ACTUAL_WINDOW_WIDTH - xoffset };
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

void Game::draw() {		// draw routine
	Draw::draw(Rspr::bg, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.5);

	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_HEIGHT; j++) {
			if (extra <= 50) {
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

	Draw::onmap(Rspr::pointNorm, (POINT_X1 + POINT_X2) / 2.0, (POINT_Y1 + POINT_Y2) / 2.0, 0.0);
	Draw::onmapB(Rspr::pointNorm, 0.0, 0.0, 0.0, Color::white, 0.0);

	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].draw();
	for (int i = 0; i < POISON_NUM_MAX; i++) poisonArray[i].draw();
	for (int i = 0; i < PETAL_NUM_MAX; i++) petalArray[i].draw();
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) mushroomArray[i].draw();

	Effect::draw();
	Draw::flush();

	// Overlay informations
	drawFaces();
	Draw::drawB(Rspr::intengrad, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.91, Color::black, 1.0);
	Draw::bignumber(elapsed, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.91);

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

void Game::ruleMove() // rules related to move command
{
	std::vector<int> movable;
	std::vector<int> alive;

	for (int i = 0; i < UNIT_NUM_MAX; i++) {
		// Priority for first team at the even turn, second team otherwise.
		int ind = (elapsed % 2 == 0) ? i : (i + UNIT_NUM_MAX / 2) % UNIT_NUM_MAX;
		Unit& u = unitArray[ind];
		protocol_command c = Network::getCommand(ind);
		protocol_state s = u.getState();

		if (s != STATE_NULL && s != STATE_DEAD) {
			alive.push_back(ind);

			if (command_kind_move(c)) {
				movable.push_back(ind);
			}
		}
	}

	bool changed = true; // is there any change in the map

	while (changed) {
		changed = false; // assuming no change

		for (int i = 0; i < movable.size(); i++) {
			int ind = movable[i];
			Unit& u = unitArray[ind];
			protocol_command c = Network::getCommand(ind);
			protocol_direction d = command_to_direction(c);
			bool moved = u.move(d);
			bool duplicated = false;

			if (moved) // control for move
			{
				for (int j = 0; j < alive.size(); j++) {
					if (alive[j] == ind)
						continue;

					Unit& other = unitArray[alive[j]];
					if (other.getX() == u.getX() && other.getY() == u.getY()) {
						duplicated = true;
						break;
					}
				}

				if (!duplicated) // if not duplicated
				{	
					for (int j = 0; j < UNIT_NUM_MAX; j++)
					{
						if (j == ind)
							continue;

						Unit& other = unitArray[j];
						if (other.getOrgX() == u.getX() && other.getOrgY() == u.getY()) {
							duplicated = true;
							break;
						}
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

	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		Unit& u = unitArray[i];
		protocol_command c = Network::getCommand(i);
		if (command_kind_attack(c)) {
			protocol_direction d = command_to_direction(c);
			u.attack(d);
		}
	}

	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		Unit& u = unitArray[i];

		if (!state_kind_attack(u.getState()) && u.getDep() != DEP_ME)
			continue;

		protocol_direction d = state_to_direction(u.getState());
		protocol_team t = u.getTeam();
		int x = u.getX();
		int y = u.getY();
		int dx = direction_to_dx(d);
		int dy = direction_to_dy(d);

		if (u.getDep() == DEP_CSE) {
			regionStun(team_invert(t), x - 1, y - 1, x + 1, y + 1, 3);
			Effect::push(new EffectCSEAttack(t, (float)x, (float)y));
		}
		else if (u.getDep() == DEP_PHYS) {
			regionDamage(team_invert(t), x, y, x + dx * MAP_WIDTH, y + dy * MAP_HEIGHT, 1);
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
			regionDamage(team_invert(t), x - 1, y - 1, x + 1, y + 1, 1);
		}
		else if (u.getDep() == DEP_CHEM) {
			for (int p = 1; p <= POISON_LENGTH; p++)
			{
				int indexForValidPoison = getValidPoisonIndex();
				poisonArray[indexForValidPoison].spawn(t, x + dx * p, y + dy * p);
			}
		}
	} // end of attack
}

void Game::ruleCollide() {
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		Unit& u = unitArray[i];

		if (!u.isAlive())
			continue;

		int x = u.getX();
		int y = u.getY();
		protocol_team t = u.getTeam();

		for (int j = 0; j < PETAL_NUM_MAX; j++) // Petals
		{
			Petal& p = petalArray[j];
			if (p.isValid() && x == p.getX() && y == p.getY())
			{
				if (p.getTeam() == t) {
					u.heal(1);
				}
				else {
					u.damage(1);
				}
				p.invalidate();
			}
		}

		for (int j = 0; j < POISON_NUM_MAX; j++) // Poisons
		{
			Poison& p = poisonArray[j];
			if (p.isValid() && t != p.getTeam() && x == p.getX() && y == p.getY())
			{
				u.damage(1);
			}
		}

		for (int j = 0; j < MUSHROOM_NUM_MAX; j++) // Mushrooms
		{
			Mushroom& m = mushroomArray[j];
			if (m.isValid() && t != m.getTeam() && x == m.getX() && y == m.getY())
			{
				u.damage(1);
				m.invalidate();
			}
		}
	}
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

	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		Unit& u = unitArray[i];
		protocol_command c = Network::getCommand(i);
		if (command_kind_skill(c)) {
			protocol_direction d = command_to_direction(c);
			u.skill(d);
		}
	}

	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
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
			regionStun(team_invert(t), x - 3, y - 3, x + 3, y + 3, 5);
		}
		else if (u.getDep() == DEP_PHYS) {
			// Not implemented
		}
		else if (u.getDep() == DEP_LIFE) {
			regionHealAll(t, 1);
		}
		else if (u.getDep() == DEP_ME) {
			// Not implemented
		}
		else if (u.getDep() == DEP_CHEM) {
			int indexForValidMushroom = getValidMushroomIndex();
			mushroomArray[indexForValidMushroom].spawn(t, x + dx, y + dy);
		}
	} // end of skill
}

void Game::ruleSpawn() // rules related to spawn
{
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		Unit& u = unitArray[i];
		protocol_command c = Network::getCommand(i);

		if (command_kind_spawn(c) && u.getState() == STATE_DEAD)
		{
			//int& tspawn = spawn[team_to_index(u.getTeam())];
			//tspawn++;
			u.spawn(command_to_dep(c));
			//if ((tspawn % HERO_DELAY == 0) && (tspawn != 0)) u.setHero(true);
		}

	} // end of spawn
}

void Game::rulePoint() {
	bool point[2] = { false, false };
	
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
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

void Game::turn() {
	if (end)
		return;
	elapsed++;

	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].turn();
	for (int i = 0; i < POISON_NUM_MAX; i++) poisonArray[i].turn();
	for (int i = 0; i < PETAL_NUM_MAX; i++) petalArray[i].turn();
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) mushroomArray[i].turn();

	// things are done by rules
	ruleMove();
	ruleSpawn();

	ruleSkill();
	ruleAttack();
	ruleCollide();
	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].flush();

	rulePoint();
	
	if(Network::getMode() == MODE_SERVER) makeProtocol();	// make protocol
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
