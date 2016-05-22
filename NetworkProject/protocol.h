//
// CNH 2016.05.13
// protocol definition
//

#pragma once

typedef enum {
	TEAM_NULL,
	TEAM_POSTECH,
	TEAM_KAIST
} protocol_team;

typedef enum {
	DEP_NULL,
	DEP_CSE,
	DEP_PHYS,
	DEP_LIFE,
	DEP_ME,
	DEP_CHEM
} protocol_dep;

typedef enum {
	STATE_NULL,
	STATE_IDLE,
	STATE_DEAD,
	STATE_ATTACK_RIGHT,
	STATE_ATTACK_UP,
	STATE_ATTACK_LEFT,
	STATE_ATTACK_DOWN,
	STATE_SKILL_RIGHT,
	STATE_SKILL_UP,
	STATE_SKILL_LEFT,
	STATE_SKILL_DOWN,
	STATE_STUN
} protocol_state;

typedef enum {
	DIRECTION_NULL,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_LEFT,
	DIRECTION_DOWN,
} protocol_direction;

typedef struct {
	protocol_team team;
	protocol_dep dep;
	int x, y;
	protocol_state state;
	int health;
	bool hero;
	int cooltime;
	int respawn;
	int stun;
} protocol_unit;

typedef struct {
	protocol_team team;
	int x, y;
} protocol_flag;

typedef struct {
	bool valid;
	protocol_team team;
	int x, y;
	int span;
} protocol_poison;

typedef struct {
	bool valid;
	protocol_team team;
	int x, y;
	protocol_direction direction;
} protocol_petal;

typedef struct {
	bool valid;
	protocol_team team;
	int x, y;
} protocol_mushroom;

#define UNIT_NUM_MAX 2
#define FLAG_NUM_MAX 5
#define POISON_NUM_MAX 30
#define PETAL_NUM_MAX 12
#define MUSHROOM_NUM_MAX 5

typedef struct {
	protocol_unit unit[UNIT_NUM_MAX];
	protocol_flag flag[FLAG_NUM_MAX];
	protocol_poison poison[POISON_NUM_MAX];
	protocol_petal petal[PETAL_NUM_MAX];
	protocol_mushroom mushroom[MUSHROOM_NUM_MAX];
	int score[2];
	int turnleft;
} protocol_data;

typedef enum {
	COMMAND_MOVE_RIGHT,
	COMMAND_MOVE_UP,
	COMMAND_MOVE_LEFT,
	COMMAND_MOVE_DOWN,
	COMMAND_ATTACK_RIGHT,
	COMMAND_ATTACK_UP,
	COMMAND_ATTACK_LEFT,
	COMMAND_ATTACK_DOWN,
	COMMAND_SKILL_RIGHT,
	COMMAND_SKILL_UP,
	COMMAND_SKILL_LEFT,
	COMMAND_SKILL_DOWN,
	COMMAND_SPAWN_CSE,
	COMMAND_SPAWN_PHYS,
	COMMAND_SPAWN_LIFE,
	COMMAND_SPAWN_ME,
	COMMAND_SPAWN_CHEM,
	COMMAND_FLAG
} protocol_command;

typedef struct {
	protocol_command command[UNIT_NUM_MAX];
} protocol_command_array;

#define MAP_WIDTH 15
#define MAP_HEIGHT 15

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

#define FRAME_INTERVAL 16
#define FRAME_PER_TURN 60
#define DELTA_PER_TURN (1.0 / FRAME_PER_TURN)

#define RESPAWN_COOLTIME 10

//socket [x] is mapped to unit number [x]

//NETWORK PROTOCOL

#define PORT_STRING "2222"
//#define SERV_IP_STRING "141.223.208.207"
#define SERV_IP_STRING "119.202.87.115"
//#define SERV_IP_STRING "141.223.85.241"

#define MODE_NOTHING 7
#define MODE_SERVER 9
#define MODE_CLIENT 8
#define MODE_SERVER_KEY '9'
#define MODE_CLIENT_KEY '8'

#define MESSAGE_T0_CLIENT_SIZE sizeof(protocol_data)+1
#define MESSAGE_TO_SERVER_SIZE 16

#define GAME_START_CHAR 'G'

#define TEAM_POSTECH_SPAWN_X 0
#define TEAM_POSTECH_SPAWN_Y 0
#define TEAM_KAIST_SPAWN_X 3
#define TEAM_KAIST_SPAWN_Y 0

#define FLAG1_X 8
#define FLAG1_Y 1
#define FLAG2_X 8
#define FLAG2_Y 2
#define FLAG3_X 8
#define FLAG3_Y 3
#define FLAG4_X 8
#define FLAG4_Y 4
#define FLAG5_X 8
#define FLAG5_Y 5

#define TURN_MAX 300