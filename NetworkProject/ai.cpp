// ai.cpp 에서는 //
//접속 시 character 선택,
//각각의 unit들에 대한 command 입력 가능하도록,
//만약 character 선택에 대한 입력을 하지 않으면 아까와 같은 character가 선택되도록 작성
//해서 결국적으로 ai.cpp는 매턴 수행될 알고리즘만 입력하면 될 수 있도록 작성
//예시 ai는 random하게 작성될 수 있도록 작성
//random character 선택, random command로 움직이도록 작성

#include <string.h>
#include "ai.h"
#include "network.h"
#include <cstdlib>
#include <time.h>

int Network::team;

void Ai::aiInit(void)
{
	srand(time(NULL));
	
	Ai::CharacterInit(0,DEP_CSE);
	Ai::CharacterInit(1, DEP_CHEM);
	Ai::CharacterInit(2, DEP_LIFE);

	/*Ai::CharacterInit(0, DEP_ME);
	Ai::CharacterInit(1, DEP_PHYS);
	Ai::CharacterInit(2, DEP_CHEM);*/

}

int ab(int x)
{
	return x > 0 ? x : -x;
}

protocol_direction near_direction(protocol_unit *u, int me)
{
	int min_d = ARBITRARY_BIG_NUM;
	int min_ind = 0;

	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		if (i == me || u[i].team == u[me].team) continue;
		if ((ab(u[i].x - u[me].x) + ab(u[i].y - u[me].y)) < min_d)
		{
			min_d = ab(u[i].x - u[me].x) + ab(u[i].y - u[me].y);
			min_ind = i;
		}
	}
	if (u[min_ind].x - u[me].x > 0) return DIRECTION_RIGHT;
	else if (u[min_ind].x - u[me].x < 0) return DIRECTION_LEFT;
	else
	{
		if (u[min_ind].y - u[me].y > 0) return DIRECTION_UP;
		else if (u[min_ind].y - u[me].y < 0) return DIRECTION_DOWN;
	}
	return DIRECTION_RIGHT;
}
bool same_line(protocol_unit *u, int me)
{
	for (int i = 0; i<UNIT_NUM_MAX; i++)
	{
		if (i == me || u[i].team == u[me].team) continue;
		if ((ab(u[i].x - u[me].x) == 0 || ab(u[i].y - u[me].y)) == 0)
			return true;
	}
	return false;
}
protocol_direction line_direction(protocol_unit *u, int me)
{
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		if (i == me || u[i].team == u[me].team) continue;
		if ((u[i].x - u[me].x) > 0) return DIRECTION_RIGHT;
		else if ((u[i].x - u[me].x) < 0) return DIRECTION_LEFT;
		else if ((u[i].y - u[me].y) > 0) return DIRECTION_UP;
		else return DIRECTION_UP;
	}
}


void Ai::ai(protocol_data AI_info)
{

	/*for (int i = 0; i < (UNIT_NUM_MAX) / 2; i++)
	{
		if (AI_info.unit[i].dep == DEP_ME)
		{
			if (AI_info.unit[i].x <= (MAP_WIDTH) / 2)
				move(i, DIRECTION_RIGHT);
			else if(AI_info.unit[i].x > (MAP_WIDTH) / 2)
				move(i, DIRECTION_LEFT);
			else if (AI_info.unit[i].y <= (MAP_HEIGHT) / 2)
				move(i, DIRECTION_UP);
			else move(i, DIRECTION_DOWN);
		}

		if (AI_info.unit[i].dep == DEP_PHYS || AI_info.unit[i].dep == DEP_CHEM)
		{
			if (same_line(AI_info.unit, i)) attack(i, line_direction(AI_info.unit, i));
			else move(i, near_direction(AI_info.unit, i));
		}
	}
	*/


	
	switch (rand() % 3)
	{
	case 0:
		attack(0, (protocol_direction)(rand() % 4+1));
		move(1, (protocol_direction)(rand() % 4 + 1));
		attack(2, (protocol_direction)(rand() % 4 + 1));
		break;

	case 1:
		move(0, (protocol_direction)(rand() % 4 + 1));
		attack(1, (protocol_direction)(rand() % 4 + 1));
		attack(2, (protocol_direction)(rand() % 4 + 1));
		break;

	case 2:
		attack(0, (protocol_direction)(rand() % 4 + 1));
		attack(1, (protocol_direction)(rand() % 4 + 1));
		move(2, (protocol_direction)(rand() % 4 + 1));
		break;
	}
	

	printf("UNIT INFO\n");
	for (int i = 0; i < UNIT_NUM_MAX; i++) printf("team %d dep %d x : %d y : %d state : %d health : %d\nhero : %d cooltime : %d respawn : %d stun : %d\n", AI_info.unit[i].team, AI_info.unit[i].dep, AI_info.unit[i].x, AI_info.unit[i].y, AI_info.unit[i].state, AI_info.unit[i].health, AI_info.unit[i].hero, AI_info.unit[i].cooltime, AI_info.unit[i].respawn, AI_info.unit[i].stun);

	printf("PETAL INFO\n");
	//for (int i = 0; i < PETAL_NUM_MAX; i++) printf("team %d direction %d valid %d x %d y %d\n", AI_info.petal[i].team, AI_info.petal[i].direction, AI_info.petal[i].valid, AI_info.petal[i].x, AI_info.petal[i].y);
	
	printf("POISON INFO\n");
	//for (int i = 0; i < POISON_NUM_MAX; i++) printf("team %d valid %d span %d x %d y %d\n", AI_info.poison[i].team, AI_info.poison[i].valid, AI_info.poison[i].span, AI_info.poison[i].x, AI_info.poison[i].y);
	
	printf("MUSHROOM INFO\n");
	//for (int i = 0; i < MUSHROOM_NUM_MAX; i++) printf("team %d valid %d x %d y %d\n", AI_info.mushroom[i].team, AI_info.mushroom[i].valid, AI_info.mushroom[i].x, AI_info.mushroom[i].y);
	//printf("SCORE\n");
	//printf("[POSTECH] %d [KAIST] %d\n", AI_info.score[TEAM_POSTECH - 1], AI_info.score[TEAM_KAIST - 1]);
	
	printf("OWN\n");
	for (int i = 0; i < TEAM_NUM_MAX; i++) printf("%d\n", AI_info.own[i]);

	printf("WIN\n");
	for (int i = 0; i < TEAM_NUM_MAX; i++) printf("%d\n", AI_info.win[i]);

	printf("EXTRA\n");
	printf("%d\n", AI_info.extra);

	printf("TURN ELAPSED\n");
	printf("%d\n", AI_info.elapsed);
	printf("\n");
}

void Ai::CharacterInit(int i, int x)
{
	Network::setCharacterSelection(i,(protocol_dep)x);
}

void Ai::move(int i, protocol_direction x)
{
	if (Network::getTeam() == TEAM_POSTECH)
		Network::setCommand(i, direction_to_movecommand(x));
	else
		Network::setCommand(i, direction_to_movecommand(direction_mirror(x)));
}

void Ai::attack(int i, protocol_direction x)
{
	if (Network::getTeam() == TEAM_POSTECH)
		Network::setCommand(i, direction_to_attackcommand(x));
	else
		Network::setCommand(i, direction_to_attackcommand(direction_mirror(x)));
}

void Ai::skill(int i, protocol_direction x)
{
	if (Network::getTeam() == TEAM_POSTECH)
		Network::setCommand(i, direction_to_skillcommand(x));
	else
		Network::setCommand(i, direction_to_skillcommand(direction_mirror(x)));
}