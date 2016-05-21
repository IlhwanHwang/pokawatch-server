#include "game.h"
#include "protocol.h"

Unit Game::unitArray[UNIT_NUM_MAX] = { Unit(TEAM_POSTECH), Unit(TEAM_POSTECH), Unit(TEAM_POSTECH), Unit(TEAM_KAIST), Unit(TEAM_KAIST), Unit(TEAM_KAIST) };
Flag Game::flagArray[FLAG_NUM_MAX] = { Flag(FLAG1_X, FLAG1_Y), Flag(FLAG2_X, FLAG2_Y) , Flag(FLAG3_X, FLAG3_Y) , Flag(FLAG4_X, FLAG4_Y) , Flag(FLAG5_X, FLAG5_Y) };
Poison Game::poisonArray[POISON_NUM_MAX];
Petal Game::petalArray[PETAL_NUM_MAX];
Mushroom Game::mushroomArray[MUSHROOM_NUM_MAX];
int Game::score[2];
int Game::turnleft;

void Game::init()
{
	score[0] = 0;
	score[1] = 0;
	turnleft = TURN_MAX;
}
