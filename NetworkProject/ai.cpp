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
	Ai::CharacterInit(0,DEP_PHYS);
	Ai::CharacterInit(1, DEP_CHEM);
	Ai::CharacterInit(2, DEP_CSE);
}


void Ai::ai(protocol_data AI_info)
{
	if (AI_info.turnleft == 5) { move(0, DIRECTION_RIGHT), printf("55555555555555\n"); }
	else { move(1, DIRECTION_LEFT), move(0, DIRECTION_NULL);}

	/*
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
	*/

	printf("UNIT INFO\n");
	for (int i = 0; i < UNIT_NUM_MAX; i++) printf("team %d dep %d x : %d y : %d state : %d health : %d\nhero : %d cooltime : %d respawn : %d stun : %d\n", AI_info.unit[i].team, AI_info.unit[i].dep, AI_info.unit[i].x, AI_info.unit[i].y, AI_info.unit[i].state, AI_info.unit[i].health, AI_info.unit[i].hero, AI_info.unit[i].cooltime, AI_info.unit[i].respawn, AI_info.unit[i].stun);
	printf("FLAG INFO\n");
	//for (int i = 0; i < FLAG_NUM_MAX; i++) printf("team %d x : %d y : %d\n", AI_info.flag[i].team, AI_info.flag[i].x, AI_info.flag[i].y);
	printf("PETAL INFO\n");
	//for (int i = 0; i < PETAL_NUM_MAX; i++) printf("team %d direction %d valid %d x %d y %d\n", AI_info.petal[i].team, AI_info.petal[i].direction, AI_info.petal[i].valid, AI_info.petal[i].x, AI_info.petal[i].y);
	printf("POISON INFO\n");
	//for (int i = 0; i < POISON_NUM_MAX; i++) printf("team %d valid %d span %d x %d y %d\n", AI_info.poison[i].team, AI_info.poison[i].valid, AI_info.poison[i].span, AI_info.poison[i].x, AI_info.poison[i].y);
	printf("MUSHROOM INFO\n");
	//for (int i = 0; i < MUSHROOM_NUM_MAX; i++) printf("team %d valid %d x %d y %d\n", AI_info.mushroom[i].team, AI_info.mushroom[i].valid, AI_info.mushroom[i].x, AI_info.mushroom[i].y);
	printf("SCORE\n");
	printf("[POSTECH] %d [KAIST] %d\n", AI_info.score[TEAM_POSTECH - 1], AI_info.score[TEAM_KAIST - 1]);
	printf("TURN LEFT\n");
	printf("%d\n", AI_info.turnleft);
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

void Ai::flag(int i)
{
	if (Network::getTeam() == TEAM_POSTECH)
		Network::setCommand(i, COMMAND_FLAG);
	else
		Network::setCommand(i + (UNIT_NUM_MAX) / 2, COMMAND_FLAG);
}

