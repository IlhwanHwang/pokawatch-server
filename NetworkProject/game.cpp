#include <cmath>
#include <vector>
#include "game.h"
#include "protocol.h"
#include "network.h"
#include "draw.h"
#include "resource.h"

Unit Game::unitArray[UNIT_NUM_MAX] = { 
	Unit(0, MAP_HEIGHT / 2 - 1, TEAM_POSTECH),
	Unit(0, MAP_HEIGHT / 2, TEAM_POSTECH),
	Unit(0, MAP_HEIGHT / 2 + 1, TEAM_POSTECH),
	Unit(MAP_WIDTH - 1, MAP_HEIGHT / 2 - 1, TEAM_KAIST),
	Unit(MAP_WIDTH - 1, MAP_HEIGHT / 2, TEAM_KAIST),
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

void Game::init()
{
	score[0] = 0;
	score[1] = 0;
	turnleft = TURN_MAX;
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
	
	printf("UNIT_INFO\n");
	for (int i = 0; i < UNIT_NUM_MAX; i++) printf("team %d dep %d x : %d y : %d state : %d health : %d hero : %d cooltime : %d respawn : %d stun : %d\n" ,unitArray[i].getProtocol()->team, unitArray[i].getProtocol()->dep, unitArray[i].getProtocol()->x, unitArray[i].getProtocol()->y, unitArray[i].getProtocol()->state, unitArray[i].getProtocol()->health, unitArray[i].getProtocol()->hero, unitArray[i].getProtocol()->cooltime, unitArray[i].getProtocol()->respawn, unitArray[i].getProtocol()->stun);
	printf("FLAG_INFO\n");
	for (int i = 0; i < FLAG_NUM_MAX; i++) printf("team %d x : %d y : %d\n", flagArray[i].getTeam(), flagArray[i].getX(), flagArray[i].getY());
	
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
			Draw::onmap((i + j) % 2 == 0 ? light : dark, -100.0, i, j);
		}
	}

	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].draw();
	for (int i = 0; i < FLAG_NUM_MAX; i++) flagArray[i].draw();
	for (int i = 0; i < POISON_NUM_MAX; i++) poisonArray[i].draw();
	for (int i = 0; i < PETAL_NUM_MAX; i++) petalArray[i].draw();
	for (int i = 0; i < MUSHROOM_NUM_MAX; i++) mushroomArray[i].draw();
}

void Game::release() {
	for (int i = 0; i < UNIT_NUM_MAX; i++) unitArray[i].release();
}

void Game::ruleMove() {
	int turnLeft = 0; //temporary
	std::vector<int> movable;
	std::vector<int> alive;

	for (int i = 0; i < UNIT_NUM_MAX; i++) {
		// Priority for first team at the even turn, second team otherwise.
		int ind = (turnLeft % 2 == 0) ? i : ind = (i + UNIT_NUM_MAX / 2) % UNIT_NUM_MAX;
		Unit& u = unitArray[ind];
		protocol_command c = Network::getCommand(ind);
		protocol_state s = u.getState();

		if (c == COMMAND_MOVE_RIGHT ||
			c == COMMAND_MOVE_UP ||
			c == COMMAND_MOVE_LEFT ||
			c == COMMAND_MOVE_DOWN) {
			movable.push_back(ind);
		}

		if (s != STATE_NULL || s != STATE_DEAD) {
			alive.push_back(ind);
		}
	}

	bool changed = true; // is there any change in the map

	while (changed) {
		changed = false; // assuming no change

		for (int i = 0; i < movable.size(); i++) {
			int ind = movable[i];
			Unit& u = unitArray[ind];
			protocol_command c = Network::getCommand(i);

			switch (c) {
			case COMMAND_MOVE_RIGHT: u.move(DIRECTION_RIGHT); break;
			case COMMAND_MOVE_UP: u.move(DIRECTION_UP); break;
			case COMMAND_MOVE_LEFT: u.move(DIRECTION_LEFT); break;
			case COMMAND_MOVE_DOWN: u.move(DIRECTION_DOWN); break;
			}

			bool duplicated = false;

			for (int j = 0; j < alive.size(); j++) {
				if (j == ind)
					continue;

				Unit& other = unitArray[alive[j]];
				if (other.getX() == u.getX() && other.getY() == u.getY()) {
					duplicated = true;
					break;
				}
			}

			if (duplicated) {
				// if duplicated, then go back
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

void Game::turn() {
	for (int i = 0; i < UNIT_NUM_MAX; i++) {
		Unit& u = unitArray[i];
		u.turn();
	}

	ruleMove();

	//attack command
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		
		Unit& u = unitArray[i];
		protocol_command c = Network::getCommand(i);
		
		
		if (c == COMMAND_ATTACK_RIGHT||
			c == COMMAND_ATTACK_UP ||
			c == COMMAND_ATTACK_LEFT ||
			c == COMMAND_ATTACK_DOWN)
		{
			int b;
			switch (u.getDep())
			{
			case DEP_CSE:
				switch (c)
				{
				case COMMAND_ATTACK_RIGHT:
					u.attack(DIRECTION_RIGHT);
					printf("A\nT\nT\nA\nC\nK\n");
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
					if (abs((unitArray[a].getX() - u.getX()) <= 1) && (abs(unitArray[a].getY() - u.getY()) <= 1))
						if (unitArray[a].getTeam() != u.getTeam()) unitArray[a].setStun(1);
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
					u.attack(DIRECTION_RIGHT);
					
					for (b = 0; b < PETAL_NUM_MAX; b++)
					{
						if (petalArray[b].getProtocol()->valid)
							break;
					}
					if(b<PETAL_NUM_MAX) petalArray[b].spawn(u.getTeam(), u.getX() + 1, u.getY(), DIRECTION_RIGHT);
					break;
				case COMMAND_ATTACK_UP:
					u.attack(DIRECTION_UP);
					
					for (b = 0; b < PETAL_NUM_MAX; b++)
					{
						if (petalArray[b].getProtocol()->valid)
							break;
					}
					if (b<PETAL_NUM_MAX) petalArray[b].spawn(u.getTeam(), u.getX(), u.getY() + 1, DIRECTION_UP);
					break;
				case COMMAND_ATTACK_LEFT:
					u.attack(DIRECTION_LEFT);
					
					for (b = 0; b < PETAL_NUM_MAX; b++)
					{
						if (petalArray[b].getProtocol()->valid)
							break;
					}
					if (b<PETAL_NUM_MAX) petalArray[b].spawn(u.getTeam(), u.getX() - 1, u.getY(), DIRECTION_LEFT);
					break;
				case COMMAND_ATTACK_DOWN:
					u.attack(DIRECTION_DOWN);
					
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
					if (abs((unitArray[a].getX() - u.getX()) <= 1) && (abs(unitArray[a].getY() - u.getY()) <= 1) && (unitArray[a].getY() - u.getY())*(unitArray[a].getX() - u.getX()) == 0)
						if (unitArray[a].getTeam() != u.getTeam()) unitArray[a].damage(2);
				}
				break;
			case DEP_CHEM:
				switch (c)
				{
				case COMMAND_ATTACK_RIGHT:
					u.attack(DIRECTION_RIGHT);
					
					for (b = 0; b < POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() + 1, u.getY());
					for(b=b+1;b<POISON_NUM_MAX;b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() + 2, u.getY());
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() + 3, u.getY());
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() + 4, u.getY());
					
					break;
				case COMMAND_ATTACK_UP:
					u.attack(DIRECTION_UP);
					
					for (b = 0; b < POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX(), u.getY() +1);
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX(), u.getY()+2);
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() , u.getY()+3);
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() , u.getY()+4);
					break;
				case COMMAND_ATTACK_LEFT:
					u.attack(DIRECTION_LEFT);
					
					for (b = 0; b < POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() - 1, u.getY());
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() - 2, u.getY());
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() - 3, u.getY());
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() - 4, u.getY());
					break;
				case COMMAND_ATTACK_DOWN:
					u.attack(DIRECTION_DOWN);
					
					for (b = 0; b < POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() , u.getY() - 1);
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() , u.getY() - 2);
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() , u.getY() -3);
					for (b = b + 1; b<POISON_NUM_MAX; b++)
					{
						if (poisonArray[b].getProtocol()->valid)
							break;
					}
					if (b < POISON_NUM_MAX) poisonArray[b].spawn(u.getTeam(), u.getX() , u.getY() -4);
					break;
				}
				break;
			}

		}

	}
	// End of attack command

	//skill command
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{

		Unit& u = unitArray[i];
		protocol_command c = Network::getCommand(i);


		if (c == COMMAND_SKILL_RIGHT ||
			c == COMMAND_SKILL_UP ||
			c == COMMAND_SKILL_LEFT ||
			c == COMMAND_SKILL_DOWN)
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
					if (abs((unitArray[a].getX() - u.getX()) <= 3) && (abs(unitArray[a].getY() - u.getY()) <= 3))
						if (unitArray[a].getTeam() != u.getTeam()) unitArray[a].stun(1);
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
						if (abs((unitArray[a].getX() - u.getX()) <= 1) && (abs(unitArray[a].getY() +2 - u.getY()) <= 1))
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
						if (abs((unitArray[a].getX() - u.getX()) <= 1) && (abs(unitArray[a].getY() -2 - u.getY()) <= 1))
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
				case COMMAND_SKILL_RIGHT:
					u.skill(DIRECTION_RIGHT);
					u.setPosition(MAP_WIDTH,u.getY());
					break;
				case COMMAND_SKILL_UP:
					u.skill(DIRECTION_UP);
					u.setPosition(u.getX(), MAP_HEIGHT);
					break;
				case COMMAND_SKILL_LEFT:
					u.skill(DIRECTION_LEFT);
					u.setPosition(0, u.getY());
					break;
				case COMMAND_SKILL_DOWN:
					u.skill(DIRECTION_DOWN);
					u.setPosition(u.getX(), 0);
					break;
				}
				break;
			case DEP_CHEM:
				switch (c)
				{
				case COMMAND_SKILL_RIGHT:
					u.skill(DIRECTION_RIGHT);
					if ((b = getValidMushroomIndex()) != INVALID_POISON_INDEX) mushroomArray[b].spawn(u.getTeam(), u.getX()+1, u.getY());
					break;
				case COMMAND_SKILL_UP:
					u.skill(DIRECTION_UP);
					if ((b = getValidMushroomIndex()) != INVALID_POISON_INDEX) mushroomArray[b].spawn(u.getTeam(), u.getX() , u.getY()+1);
					break;
				case COMMAND_SKILL_LEFT:
					u.skill(DIRECTION_LEFT);
					if ((b = getValidMushroomIndex()) != INVALID_POISON_INDEX) mushroomArray[b].spawn(u.getTeam(), u.getX() -1, u.getY());
					break;
				case COMMAND_SKILL_DOWN:
					u.skill(DIRECTION_DOWN);
					if ((b = getValidMushroomIndex()) != INVALID_POISON_INDEX) mushroomArray[b].spawn(u.getTeam(), u.getX() , u.getY() -1);
					break;
				}
				break;
			}

		}

	} // end of skill

	  //spawn command
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{

		Unit& u = unitArray[i];
		protocol_command c = Network::getCommand(i);

		if (c == COMMAND_SPAWN_CSE ||
			c == COMMAND_SPAWN_PHYS ||
			c == COMMAND_SPAWN_LIFE ||
			c == COMMAND_SPAWN_ME ||
			c == COMMAND_SPAWN_CHEM)
		{
			int b;
			switch (c)
			{
			case COMMAND_SPAWN_CSE:
				if (u.getTeam() == TEAM_POSTECH) u.spawn(DEP_CSE);
				else u.spawn(DEP_CSE);
				break;
			case COMMAND_SPAWN_PHYS:
				if (u.getTeam() == TEAM_POSTECH) u.spawn(DEP_PHYS);
				else u.spawn(DEP_PHYS);
				break;
			case COMMAND_SPAWN_LIFE:
				if (u.getTeam() == TEAM_POSTECH) u.spawn(DEP_LIFE);
				else u.spawn(DEP_LIFE);
				break;
			case COMMAND_SPAWN_ME:
				if (u.getTeam() == TEAM_POSTECH) u.spawn(DEP_ME);
				else u.spawn(DEP_ME);
				break;
			case COMMAND_SPAWN_CHEM:
				if (u.getTeam() == TEAM_POSTECH) u.spawn(DEP_CHEM);
				else u.spawn(DEP_CHEM);
				break;
			}
		}

	} // end of spawn

	// flag command
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{

		Unit& u = unitArray[i];
		protocol_command c = Network::getCommand(i);

		if (c == COMMAND_FLAG )
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

	for (int a = 0; a < UNIT_NUM_MAX; a++)
	{
		for (int b = 0; b < PETAL_NUM_MAX; b++) // petal confliction
		{
			if (unitArray[a].getY() == petalArray[b].getY() && unitArray[a].getX() == petalArray[b].getX())
			{
				if (unitArray[a].getTeam() != petalArray[b].getTeam()) unitArray[a].damage(1);
			}
		}
		for (int b = 0; b < POISON_NUM_MAX; b++) // poison confliction
		{
			if (unitArray[a].getY() == poisonArray[b].getY() && unitArray[a].getX() == poisonArray[b].getX())
			{
				if (unitArray[a].getTeam() != poisonArray[b].getTeam()) unitArray[a].damage(1);
			}
		}
		for (int b = 0; b < MUSHROOM_NUM_MAX; b++) // mushroom confliction
		{
			if (unitArray[a].getY() == mushroomArray[b].getY() && unitArray[a].getX() == mushroomArray[b].getX())
			{
				if (unitArray[a].getTeam() != mushroomArray[b].getTeam()) unitArray[a].damage(1);
			}
		}
	}

	makeProtocol();
}

int Game::getValidPoisonIndex()
{
	int b;
	for ( b = 0; b < POISON_NUM_MAX; b++)
	{
		if (poisonArray[b].getProtocol()->valid)
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
		if (mushroomArray[b].getProtocol()->valid)
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
		if (petalArray[b].getProtocol()->valid)
			break;
	}
	if (b < PETAL_NUM_MAX) return b;
	else return INVALID_PETAL_INDEX;
}
