//
// CNH 2016.05.13
// protocol definition
//

#pragma once

#define TEAM_NULL 0
#define TEAM_POSTECH 1
#define TEAM_KAIST 2

struct protocol_unit {
	int team;
	int dep;
	int x;
	int y;
	int state;
	int health;
	int hero;
	int cooltime;
	int respawn;
};

#define UNIT_TEAM_NONE TEAM_NULL
#define UNIT_TEAM_POSTECH TEAM_POSTECH
#define UNIT_TEAM_KAIST TEAM_KAIST

#define UNIT_DEP_NULL 0
#define UNIT_DEP_CSE 1
#define UNIT_DEP_PHYS 2
#define UNIT_DEP_LIFE 3
#define UNIT_DEP_ME 4
#define UNIT_DEP_CHEM 5

#define UNIT_STATE_NULL 0
#define UNIT_STATE_NORMAL 1
#define UNIT_STATE_DEAD 2
#define UNIT_STATE_ATTACK_RIGHT 0x11
#define UNIT_STATE_ATTACK_UP 0x12
#define UNIT_STATE_ATTACK_LEFT 0x13
#define UNIT_STATE_ATTACK_DOWN 0x14
#define UNIT_STATE_STUN_1 0x21
#define UNIT_STATE_STUN_2 0x22
#define UNIT_STATE_STUN_3 0x23

#define UNIT_HERO_HERO 1
#define UNIT_HERO_NONHERO 0

struct protocol_flag {
	int team;
	int x;
	int y;
};

#define FLAG_TEAM_NULL TEAM_NULL
#define FLAG_TEAM_POSTECH TEAM_POSTECH
#define FLAG_TEAM_KAIST TEAM_KAIST

struct protocol_poison {
	int team;
	int x;
	int y;
};

struct protocol_petal {
	int team;
	int x;
	int y;
	int direction;
};

struct protocol_mushroom {
	int team;
	int x;
	int y;
};

#define UNIT_NUM_MAX 6
#define FLAG_NUM_MAX 3
#define POISON_NUM_MAX 30
#define PETAL_NUM_MAX 12
#define MUSHROOM_NUM_MAX 5

struct protocol_data {
	struct protocol_unit unit[UNIT_NUM_MAX];
	struct protocol_flag flag[FLAG_NUM_MAX];
	struct protocol_poison poison[POISON_NUM_MAX];
	struct protocol_petal petal[PETAL_NUM_MAX];
	struct protocol_mushroom mushroom[MUSHROOM_NUM_MAX];
	int score[2];
	int turnleft;
};

#define COMMAND_MOVE_RIGHT 1
#define COMMAND_MOVE_UP 2
#define COMMAND_MOVE_LEFT 3
#define COMMAND_MOVE_DOWN 4

#define COMMAND_ATTACK_RIGHT 0x11
#define COMMAND_ATTACK_UP 0x12
#define COMMAND_ATTACK_LEFT 0x13
#define COMMAND_ATTACK_DOWN 0x14

#define COMMAND_SKILL_RIGHT 0x21
#define COMMAND_SKILL_UP 0x22
#define COMMAND_SKILL_LEFT 0x23
#define COMMAND_SKILL_DOWN 0x24

#define COMMAND_SPAWN_CSE 0x31
#define COMMAND_SPAWN_PHYS 0x32
#define COMMAND_SPAWN_LIFE 0x33
#define COMMAND_SPAWN_ME 0x34
#define COMMAND_SPAWN_CHEM 0x35

#define COMMAND_FLAG 0x41

struct protocol_command {
	int command[UNIT_NUM_MAX];
};

#define MAP_WIDTH 15
#define MAP_HEIGHT 15

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

//socket [x] is mapped to unit number [x]