#include <cmath>
#include <vector>
#include "game.h"
#include "protocol.h"
#include "network.h"
#include "draw.h"
#include "resource.h"

Unit Game::unitArray[UNIT_NUM_MAX] = { 
	Unit(0, MAP_HEIGHT / 2 - 1, TEAM_POSTECH),
//	Unit(0, MAP_HEIGHT / 2, TEAM_POSTECH),
//	Unit(0, MAP_HEIGHT / 2 + 1, TEAM_POSTECH),
//	Unit(MAP_WIDTH - 1, MAP_HEIGHT / 2 - 1, TEAM_KAIST),
//	Unit(MAP_WIDTH - 1, MAP_HEIGHT / 2, TEAM_KAIST),
	Unit(MAP_WIDTH - 1, MAP_HEIGHT / 2 + 1, TEAM_KAIST)
};
Flag Game::flagArray[FLAG_NUM_MAX] = { 
	Flag(FLAG1_X, FLAG1_Y), 
	Flag(FLAG2_X, FLAG2_Y) , 
	Flag(FLAG3_X, FLAG3_Y) , 
	Flag(FLAG4_X, FLAG4_Y) , 
	Flag(FLAG5_X, FLAG5_Y) 
};
Poison Game::poisonArray[POISON_NUM_MAX];
Petal Game::petalArray[PETAL_NUM_MAX];
Mushroom Game::mushroomArray[MUSHROOM_NUM_MAX];
protocol_data Game::protocolToSend;
protocol_data * Game::protocolPointer;
int Game::score[2];
int Game::turnleft;
int Game::heroLeft[2];
int Game::death[2];								// record how many death occured in each team
int Game::spawn[2];								// record how many spawn occured in each team


void Game::init()
{
	score[0] = 0;
	score[1] = 0;
	turnleft = TURN_MAX;
	death[0] = 0;
	death[1] = 0;
	spawn[0] = 0;
	spawn[1] = 0;

	makeProtocol();
	/*
	unitArray[0].spawn(0, 0, DEP_CSE);
	unitArray[1].spawn(1, 0, DEP_CSE);
	unitArray[2].spawn(2, 0, DEP_CSE);
	unitArray[3].spawn(3, 0, DEP_CSE);
	unitArray[4].spawn(4, 0, DEP_CSE);
	unitArray[5].spawn(5, 0, DEP_CSE);
	*/
}

void Game::makeProtocol()
{
	for (int i = 0; i < UNIT_NUM_MAX; i++) protocolToSend.unit[i] = *(unitArray[i].getProtocol());
	for (int i = 0; i < FLAG_NUM_MAX; i++) protocolToSend.flag[i] = *(flagArray[i].getProtocol());
	for (int i = 0; i < POISON_NUM_MAX; i++) protocolToSend.poison[i] = *(poisonArray[i].getProtocol());
	for (int i = 0; i < PETAL_NUM_MAX; i++) protocolToSend.petal[i] = *(petalArray[i].getProtocol());
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) protocolToSend.mushroom[i] = *(mushroomArray[i].getProtocol());
	protocolToSend.score[0] = score[0];
	protocolToSend.score[1] = score[1];
	protocolToSend.turnleft = turnleft;

	printf("\nUNIT_INFO\n");
	for (int i = 0; i < UNIT_NUM_MAX; i++) printf("team %d dep %d x : %d y : %d state : %d health : %d hero : %d cooltime : %d respawn : %d stun : %d\n" ,unitArray[i].getProtocol()->team, unitArray[i].getProtocol()->dep, unitArray[i].getProtocol()->x, unitArray[i].getProtocol()->y, unitArray[i].getProtocol()->state, unitArray[i].getProtocol()->health, unitArray[i].getProtocol()->hero, unitArray[i].getProtocol()->cooltime, unitArray[i].getProtocol()->respawn, unitArray[i].getProtocol()->stun);
	printf("FLAG_INFO\n");
	for (int i = 0; i < FLAG_NUM_MAX; i++) printf("team %d x : %d y : %d\n", flagArray[i].getTeam(), flagArray[i].getX(), flagArray[i].getY());
	printf("SCORE[POSTECH] : %d SCORE[KAIST] : %d\n", score[TEAM_POSTECH - 1], score[TEAM_KAIST - 1]);
	printf("TURN LEFT : %d\n", turnleft);
	protocolPointer = &protocolToSend;
}

void Game::update() {
	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].update();
	for (int i = 0; i < FLAG_NUM_MAX; i++) flagArray[i].update();
	for (int i = 0; i < POISON_NUM_MAX; i++) poisonArray[i].update();
	for (int i = 0; i < PETAL_NUM_MAX; i++) petalArray[i].update();
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) mushroomArray[i].update();
}

void Game::draw() {
	for (int i = 0; i < MAP_WIDTH; i++) {
		for (int j = 0; j < MAP_HEIGHT; j++) {
			Sprite& light = Rspr::tileLight;
			Sprite& dark = Rspr::tileDark;
			Draw::onmap((i + j) % 2 == 0 ? light : dark, i, j, 0.0);
		}
	}

	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].draw();
	for (int i = 0; i < FLAG_NUM_MAX; i++) flagArray[i].draw();
	for (int i = 0; i < POISON_NUM_MAX; i++) poisonArray[i].draw();
	for (int i = 0; i < PETAL_NUM_MAX; i++) petalArray[i].draw();
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) mushroomArray[i].draw();

	Draw::flush();

	// Overlay informations
	Draw::number(score[0], WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.91);
	Draw::number(score[1], WINDOW_WIDTH * 0.9, WINDOW_HEIGHT * 0.91);
	Draw::bignumber(turnleft, WINDOW_WIDTH * 0.5, WINDOW_HEIGHT * 0.91);
}

void Game::release() {
	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].release();
}

void Game::ruleMove() {
	std::vector<int> movable;
	std::vector<int> alive;

	for (int i = 0; i < UNIT_NUM_MAX; i++) {
		// Priority for first team at the even turn, second team otherwise.
		int ind = (turnleft % 2 == 0) ? i : (i + UNIT_NUM_MAX / 2) % UNIT_NUM_MAX;
		Unit& u = unitArray[ind];
		protocol_command c = Network::getCommand(ind);
		protocol_state s = u.getState();

		if (s != STATE_NULL && s != STATE_DEAD) {
			alive.push_back(ind);

			if (c == COMMAND_MOVE_RIGHT ||
				c == COMMAND_MOVE_UP ||
				c == COMMAND_MOVE_LEFT ||
				c == COMMAND_MOVE_DOWN) {
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
			bool moved = false;

			switch (c) {
			case COMMAND_MOVE_RIGHT: moved = u.move(DIRECTION_RIGHT); break;
			case COMMAND_MOVE_UP: moved = u.move(DIRECTION_UP); break;
			case COMMAND_MOVE_LEFT: moved = u.move(DIRECTION_LEFT); break;
			case COMMAND_MOVE_DOWN: moved = u.move(DIRECTION_DOWN); break;
			}

			bool duplicated = false;

			if (moved) {
				for (int j = 0; j < alive.size(); j++) {
					if (alive[j] == ind)
						continue;

					Unit& other = unitArray[alive[j]];
					if (other.getX() == u.getX() && other.getY() == u.getY()) {
						duplicated = true;
						break;
					}
				}

				if (!duplicated) {
					for (int j = 0; j < UNIT_NUM_MAX; j++) {
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
				switch (c) {
				case COMMAND_MOVE_RIGHT: u.move(DIRECTION_LEFT); break;
				case COMMAND_MOVE_UP: u.move(DIRECTION_DOWN); break;
				case COMMAND_MOVE_LEFT: u.move(DIRECTION_RIGHT); break;
				case COMMAND_MOVE_DOWN: u.move(DIRECTION_UP); break;
				}
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

void Game::ruleAttack()
{
	//attack command
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{

		Unit& u = unitArray[i];

		if (!u.isAlive())
			continue;

		protocol_command c = Network::getCommand(i);


		if ((c == COMMAND_ATTACK_RIGHT ||
			c == COMMAND_ATTACK_UP ||
			c == COMMAND_ATTACK_LEFT ||
			c == COMMAND_ATTACK_DOWN ) && u.getCooltime() == 0)
		{
			int b;
			int indexForValidPetal = getValidPetalIndex();
			int indexForValidPoison = getValidPoisonIndex();
			switch (u.getDep())
			{
			case DEP_CSE:
				switch (c)
				{
				case COMMAND_ATTACK_RIGHT:
					u.attack(DIRECTION_RIGHT);
					break;
				case COMMAND_ATTACK_UP:
					u.attack(DIRECTION_UP);
					break;
				case COMMAND_ATTACK_LEFT:
					u.attack(DIRECTION_LEFT);
					break;
				case COMMAND_ATTACK_DOWN:
					u.attack(DIRECTION_DOWN);
					break;
				}
				for (int a = 0; a < UNIT_NUM_MAX; a++)
				{
					if (abs((unitArray[a].getX() - u.getX())) <= 1 && (abs(unitArray[a].getY() - u.getY()) <= 1))
						if (unitArray[a].getTeam() != u.getTeam()) unitArray[a].setStun(5);
				}
				break;
			case DEP_PHYS:
				switch (c)
				{
				case COMMAND_ATTACK_RIGHT:
					u.attack(DIRECTION_RIGHT);
					for (int a = 0; a < UNIT_NUM_MAX; a++)
					{
						if (a != i && unitArray[a].getY() == u.getY() && unitArray[a].getX() > u.getX())
							if (unitArray[a].getTeam() != u.getTeam()) unitArray[a].damage(1);
					}
					break;
				case COMMAND_ATTACK_UP:
					u.attack(DIRECTION_UP);
					for (int a = 0; a < UNIT_NUM_MAX; a++)
					{
						if (a != i && unitArray[a].getX() == u.getX() && unitArray[a].getY() > u.getY())
							if (unitArray[a].getTeam() != u.getTeam()) unitArray[a].damage(1);
					}
					break;
				case COMMAND_ATTACK_LEFT:
					u.attack(DIRECTION_LEFT);
					for (int a = 0; a < UNIT_NUM_MAX; a++)
					{
						if (a != i && unitArray[a].getY() == u.getY() && unitArray[a].getX() < u.getX())
							if (unitArray[a].getTeam() != u.getTeam()) unitArray[a].damage(1);
					}
					break;
				case COMMAND_ATTACK_DOWN:
					u.attack(DIRECTION_DOWN);
					for (int a = 0; a < UNIT_NUM_MAX; a++)
					{
						if (a != i && unitArray[a].getX() == u.getX() && unitArray[a].getY() < u.getY())
							if (unitArray[a].getTeam() != u.getTeam()) unitArray[a].damage(1);
					}
					break;
				}
				break;
			case DEP_LIFE:
				switch (c)
				{
				case COMMAND_ATTACK_RIGHT:
					
					if (indexForValidPetal != INVALID_PETAL_INDEX)
					{
						u.attack(DIRECTION_RIGHT);
						petalArray[indexForValidPetal].spawn(u.getTeam(), u.getX() + 1, u.getY(), DIRECTION_RIGHT);
					}

					break;
				case COMMAND_ATTACK_UP:
					
					if (indexForValidPetal = getValidPetalIndex() != INVALID_PETAL_INDEX)
					{
						u.attack(DIRECTION_UP);
						petalArray[indexForValidPetal].spawn(u.getTeam(), u.getX(), u.getY() + 1, DIRECTION_UP);
					}
					break;
				case COMMAND_ATTACK_LEFT:

					if (indexForValidPetal = getValidPetalIndex() != INVALID_PETAL_INDEX)
					{
						u.attack(DIRECTION_LEFT);
						petalArray[indexForValidPetal].spawn(u.getTeam(), u.getX() - 1, u.getY(), DIRECTION_LEFT);
					}

					break;
				case COMMAND_ATTACK_DOWN:

					if (indexForValidPetal = getValidPetalIndex() != INVALID_PETAL_INDEX)
					{
						petalArray[indexForValidPetal].spawn(u.getTeam(), u.getX(), u.getY() - 1, DIRECTION_DOWN);
						u.attack(DIRECTION_DOWN);
					}
					break;
				}
				break;
			case DEP_CHEM: // ÀÌ°Å µµ index ¹Ù²ã³ö¾ß Á¦´ë·Î ÀÛµ¿ÇÔ.... ~!!!!!! mushroomµµ ÇÏ¼À
				switch (c)
				{
				case COMMAND_ATTACK_RIGHT:
					u.attack(DIRECTION_RIGHT);

					for (b = 0; b < POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() + 1, u.getY());
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() + 2, u.getY());
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() + 3, u.getY());
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() + 4, u.getY());

					break;
				case COMMAND_ATTACK_UP:
					u.attack(DIRECTION_UP);

					for (b = 0; b < POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX(), u.getY() + 1);
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX(), u.getY() + 2);
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX(), u.getY() + 3);
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX(), u.getY() + 4);
					break;
				case COMMAND_ATTACK_LEFT:
					u.attack(DIRECTION_LEFT);

					for (b = 0; b < POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() - 1, u.getY());
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() - 2, u.getY());
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() - 3, u.getY());
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() - 4, u.getY());
					break;
				case COMMAND_ATTACK_DOWN:
					u.attack(DIRECTION_DOWN);

					for (b = 0; b < POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX(), u.getY() - 1);
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX(), u.getY() - 2);
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX(), u.getY() - 3);
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid==0)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX(), u.getY() - 4);
					break;
				}
				break;
			}

		}

	}
	// End of attack command
}

void Game::ruleSkill()
{
	//skill command
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{

		Unit& u = unitArray[i];
		protocol_command c = Network::getCommand(i);


		if ( ((c == COMMAND_SKILL_RIGHT ||
			c == COMMAND_SKILL_UP ||
			c == COMMAND_SKILL_LEFT ||
			c == COMMAND_SKILL_DOWN) ) && u.getHero() == TRUE && u.getCooltime() == 0)
		{
			int b;
			switch (u.getDep())
			{
			case DEP_CSE:
				switch (c)
				{
				case COMMAND_SKILL_RIGHT:
					u.skill(DIRECTION_RIGHT);
					break;
				case COMMAND_SKILL_UP:
					u.skill(DIRECTION_UP);
					break;
				case COMMAND_SKILL_LEFT:
					u.skill(DIRECTION_LEFT);
					break;
				case COMMAND_SKILL_DOWN:
					u.skill(DIRECTION_DOWN);
					break;
				}
				for (int a = 0; a < UNIT_NUM_MAX; a++)
				{
					if (abs((unitArray[a].getX() - u.getX()) ) <= 3 && (abs(unitArray[a].getY() - u.getY()) <= 3))
						if (unitArray[a].getTeam() != u.getTeam()) unitArray[a].stun(5);
				}
				break;
			case DEP_PHYS:
				switch (c)
				{
				case COMMAND_SKILL_RIGHT:
					u.skill(DIRECTION_RIGHT);
					for (int a = 0; a < UNIT_NUM_MAX; a++)
					{
						if (abs((unitArray[a].getX() + 2 - u.getX()) <= 1) && (abs(unitArray[a].getY() - u.getY()) <= 1))
							if (unitArray[a].getTeam() != u.getTeam())unitArray[a].damage(BLACKHOLE_DAMAGE);
					}
					break;
				case COMMAND_SKILL_UP:
					u.skill(DIRECTION_UP);
					for (int a = 0; a < UNIT_NUM_MAX; a++)
					{
						if (abs((unitArray[a].getX() - u.getX()) <= 1) && (abs(unitArray[a].getY() + 2 - u.getY()) <= 1))
							if (unitArray[a].getTeam() != u.getTeam())unitArray[a].damage(BLACKHOLE_DAMAGE);
					}
					break;
				case COMMAND_SKILL_LEFT:
					u.skill(DIRECTION_LEFT);
					for (int a = 0; a < UNIT_NUM_MAX; a++)
					{
						if (abs((unitArray[a].getX() - 2 - u.getX()) <= 1) && (abs(unitArray[a].getY() - u.getY()) <= 1))
							if (unitArray[a].getTeam() != u.getTeam())unitArray[a].damage(BLACKHOLE_DAMAGE);
					}
					break;
				case COMMAND_SKILL_DOWN:
					u.skill(DIRECTION_DOWN);
					for (int a = 0; a < UNIT_NUM_MAX; a++)
					{
						if (abs((unitArray[a].getX() - u.getX()) <= 1) && (abs(unitArray[a].getY() - 2 - u.getY()) <= 1))
							if (unitArray[a].getTeam() != u.getTeam())unitArray[a].damage(BLACKHOLE_DAMAGE);
					}
					break;
				}

				break;
			case DEP_LIFE:

				switch (c)
				{
				case COMMAND_SKILL_RIGHT:
					u.skill(DIRECTION_RIGHT);

					for (b = 0; b < PETAL_NUM_MAX; b++)
					{
						if (petalArray[b].getProtocol()->valid)
							break;
					}
					if (b<PETAL_NUM_MAX) petalArray[b].spawn(u.getTeam(), u.getX() + 1, u.getY(), DIRECTION_RIGHT);
					break;
				case COMMAND_SKILL_UP:
					u.skill(DIRECTION_UP);

					for (b = 0; b < PETAL_NUM_MAX; b++)
					{
						if (petalArray[b].getProtocol()->valid)
							break;
					}
					if (b<PETAL_NUM_MAX) petalArray[b].spawn(u.getTeam(), u.getX(), u.getY() + 1, DIRECTION_UP);
					break;
				case COMMAND_SKILL_LEFT:
					u.skill(DIRECTION_LEFT);

					for (b = 0; b < PETAL_NUM_MAX; b++)
					{
						if (petalArray[b].getProtocol()->valid)
							break;
					}
					if (b<PETAL_NUM_MAX) petalArray[b].spawn(u.getTeam(), u.getX() - 1, u.getY(), DIRECTION_LEFT);
					break;
				case COMMAND_SKILL_DOWN:
					u.skill(DIRECTION_DOWN);

					for (b = 0; b < PETAL_NUM_MAX; b++)
					{
						if (petalArray[b].getProtocol()->valid)
							break;
					}
					if (b<PETAL_NUM_MAX) petalArray[b].spawn(u.getTeam(), u.getX(), u.getY() - 1, DIRECTION_DOWN);
					break;
				}
				break;
			case DEP_ME:
				switch (c)
				{
					int minmax;
					int minmaxIndex;

				case COMMAND_SKILL_RIGHT:
					u.skill(DIRECTION_RIGHT);
					minmax = MAP_WIDTH;
					minmaxIndex = -1;
					for (int w = 0; w < UNIT_NUM_MAX; w++)
					{
						if(unitArray[w].getY() == u.getY() && unitArray[w].getX() > u.getX())
							if (minmax > unitArray[w].getX())
							{
								minmax = unitArray[w].getX();
								minmaxIndex = w;
							}
					}
					if (minmaxIndex != -1)
					{
						unitArray[minmaxIndex].damage(ACCIDENT_DAMAGE);
						u.setPosition(unitArray[minmaxIndex].getX(), u.getY());
					}
					else u.setPosition(MAP_WIDTH - 1, u.getY());
					break;
				case COMMAND_SKILL_UP:
					u.skill(DIRECTION_UP);
					minmax = MAP_HEIGHT;
					minmaxIndex = -1;
					for (int w = 0; w < UNIT_NUM_MAX; w++)
					{
						if (unitArray[w].getX() == u.getX() && unitArray[w].getY() > u.getY())
							if (minmax > unitArray[w].getY())
							{
								minmax = unitArray[w].getY();
								minmaxIndex = w;
							}
					}
					if (minmaxIndex != -1)
					{
						unitArray[minmaxIndex].damage(ACCIDENT_DAMAGE);
						u.setPosition(u.getX(), unitArray[minmaxIndex].getY());
					}
					else u.setPosition(u.getX(), MAP_HEIGHT-1);

					break;
				case COMMAND_SKILL_LEFT:
					u.skill(DIRECTION_LEFT);
					minmax = 0;
					minmaxIndex = -1;
					for (int w = 0; w < UNIT_NUM_MAX; w++)
					{
						if (unitArray[w].getY() == u.getY() && unitArray[w].getX() < u.getX())
							if (minmax < unitArray[w].getX())
							{
								minmax = unitArray[w].getX();
								minmaxIndex = w;
							}
					}
					if (minmaxIndex != -1)
					{
						unitArray[minmaxIndex].damage(ACCIDENT_DAMAGE);
						u.setPosition(unitArray[minmaxIndex].getX(), u.getY());
					}
					else u.setPosition(0, u.getY());
					break;
				case COMMAND_SKILL_DOWN:
					u.skill(DIRECTION_DOWN);
					minmax = 0;
					minmaxIndex = -1;
					for (int w = 0; w < UNIT_NUM_MAX; w++)
					{
						if (unitArray[w].getX() == u.getX() && unitArray[w].getY() < u.getY())
							if (minmax < unitArray[w].getY())
							{
								minmax = unitArray[w].getY();
								minmaxIndex = w;
							}
					}
					if (minmaxIndex != -1)
					{
						unitArray[minmaxIndex].damage(ACCIDENT_DAMAGE);
						u.setPosition(u.getX(), unitArray[minmaxIndex].getY());
					}
					else u.setPosition(u.getX(), 0);
					break;
				}
				break;
			case DEP_CHEM:
				switch (c)
				{
				case COMMAND_SKILL_RIGHT:
					u.skill(DIRECTION_RIGHT);
					if ((b = getValidMushroomIndex()) != INVALID_POISON_INDEX) mushroomArray[b].spawn(u.getTeam(), u.getX() + 1, u.getY());
					break;
				case COMMAND_SKILL_UP:
					u.skill(DIRECTION_UP);
					if ((b = getValidMushroomIndex()) != INVALID_POISON_INDEX) mushroomArray[b].spawn(u.getTeam(), u.getX(), u.getY() + 1);
					break;
				case COMMAND_SKILL_LEFT:
					u.skill(DIRECTION_LEFT);
					if ((b = getValidMushroomIndex()) != INVALID_POISON_INDEX) mushroomArray[b].spawn(u.getTeam(), u.getX() - 1, u.getY());
					break;
				case COMMAND_SKILL_DOWN:
					u.skill(DIRECTION_DOWN);
					if ((b = getValidMushroomIndex()) != INVALID_POISON_INDEX) mushroomArray[b].spawn(u.getTeam(), u.getX(), u.getY() - 1);
					break;
				}
				break;
			}//end of case
			u.setHero(false);
		}

	} // end of skill
}

void Game::ruleSpawn()
{
	//spawn command
	
	
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{

		Unit& u = unitArray[i];
		protocol_command c = Network::getCommand(i);

		if ((c == COMMAND_SPAWN_CSE ||
			c == COMMAND_SPAWN_PHYS ||
			c == COMMAND_SPAWN_LIFE ||
			c == COMMAND_SPAWN_ME ||
			c == COMMAND_SPAWN_CHEM) && u.getState() == STATE_DEAD)
		{
			int b;
			spawn[u.getTeam() - 1]++;
			switch (c)
			{
			case COMMAND_SPAWN_CSE:
				u.spawn(DEP_CSE);
				break;
			case COMMAND_SPAWN_PHYS:
				u.spawn(DEP_PHYS);
				break;
			case COMMAND_SPAWN_LIFE:
				u.spawn(DEP_LIFE);
				break;
			case COMMAND_SPAWN_ME:
				u.spawn(DEP_ME);
				break;
			case COMMAND_SPAWN_CHEM:
				u.spawn(DEP_CHEM);
				break;
			}
			if (u.getTeam() == TEAM_POSTECH && (spawn[TEAM_POSTECH - 1] % HERO_DELAY == 0) && spawn[TEAM_POSTECH-1] != 0) u.setHero(true);
			if (u.getTeam() == TEAM_KAIST && (spawn[TEAM_KAIST - 1] % HERO_DELAY == 0) && spawn[TEAM_KAIST - 1] != 0) u.setHero(true);
		}

	} // end of spawn
}

void Game::ruleFlag()
{
	// flag command
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{

		Unit& u = unitArray[i];
		protocol_command c = Network::getCommand(i);

		if (c == COMMAND_FLAG)
		{
			for (int f = 0; f < FLAG_NUM_MAX; f++)
			{
				if (u.getX() == flagArray[f].getX() && u.getY() == flagArray[f].getY())
				{
					flagArray[f].own(u.getTeam());
					break;
				}
			}
		}

	} // end of flag
}
void Game::turn() {
	turnleft--;
	if (turnleft <= 0) return;

	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].turn();
	for (int i = 0; i < FLAG_NUM_MAX; i++) flagArray[i].turn();
	for (int i = 0; i < POISON_NUM_MAX; i++) poisonArray[i].turn();
	for (int i = 0; i < PETAL_NUM_MAX; i++) petalArray[i].turn();
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) mushroomArray[i].turn();

	ruleMove();
	ruleAttack();
	ruleSkill();
	ruleSpawn();
	ruleFlag();
	
	for (int a = 0; a < UNIT_NUM_MAX; a++)
	{
		Unit& u = unitArray[a];
		if (!u.isAlive())
			continue;

		for (int b = 0; b < PETAL_NUM_MAX; b++) // petal confliction check
		{
			if (petalArray[b].getProtocol()->valid && u.getY() == petalArray[b].getY() && u.getX() == petalArray[b].getX())
			{
				if (u.getTeam() != petalArray[b].getTeam())
				{
					u.damage(1);
					petalArray[b].invalidate();
				}
				else u.heal(1);
			}
		}
		for (int b = 0; b < POISON_NUM_MAX; b++) // poison confliction check
		{
			if (poisonArray[b].getProtocol()->valid && u.getY() == poisonArray[b].getY() && u.getX() == poisonArray[b].getX())
			{
				if (u.getTeam() != poisonArray[b].getTeam()) u.damage(1);
			}
		}
		for (int b = 0; b < MUSHROOM_NUM_MAX; b++) // mushroom confliction check
		{
			if (mushroomArray[b].getProtocol()->valid && u.getY() == mushroomArray[b].getY() && u.getX() == mushroomArray[b].getX())
			{
				if (u.getTeam() != mushroomArray[b].getTeam())
				{
					u.damage(1);
					mushroomArray[b].invalidate();
				}
			}
		}
	}

	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		Unit& u = unitArray[i];
		if (u.getDep() == DEP_ME && u.isAlive())
		{
			for (int a = 0; a < UNIT_NUM_MAX; a++)
			{
				Unit& other = unitArray[a];
				if (u.getTeam() != other.getTeam())
					if (abs(u.getX() - other.getX()) + abs(u.getY() - other.getY()) <= 1)
						other.damage(2);
			}
		}
	}



	score[TEAM_POSTECH-1] = 0;
	score[TEAM_KAIST-1] = 0;
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		if(unitArray[i].getTeam() == TEAM_KAIST) score[TEAM_POSTECH-1] = score[TEAM_POSTECH-1] + DEATH_PENALTY*unitArray[i].getDeath();
		if(unitArray[i].getTeam() == TEAM_POSTECH) score[TEAM_KAIST - 1] = score[TEAM_KAIST - 1] + DEATH_PENALTY*unitArray[i].getDeath();
	}
	for (int i = 0; i < FLAG_NUM_MAX; i++)
	{
		if (flagArray[i].getTeam() == TEAM_POSTECH) score[TEAM_POSTECH - 1] = score[TEAM_POSTECH - 1] + FLAG_SCORE;
		if (flagArray[i].getTeam() == TEAM_KAIST) score[TEAM_KAIST - 1] = score[TEAM_KAIST - 1] + FLAG_SCORE;
	}
//	if(Network::getMode() == MODE_SERVER) makeProtocol();

	printf("petal \n");
	for (int i = 0; i < PETAL_NUM_MAX; i++)
	{
		printf("%d ", petalArray[i].getProtocol()->valid);
	}
	printf("petal end\n");
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
