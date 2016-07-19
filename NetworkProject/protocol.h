//
// CNH 2016.05.13
// protocol definition
//

#pragma once

// Protocol definitions. determine which information is contained on network communication.
// Especially, protocol structure is seen by remote AI program.

// Team enum
// do not touch. TEAM_POSTECH = 1, TEAM_KAIST = 2, for array index purpose.
typedef enum {
	TEAM_NULL,
	TEAM_POSTECH = 1,
	TEAM_KAIST = 2
} protocol_team;

// Unit department enum
typedef enum {
	DEP_NULL,
	DEP_CSE,
	DEP_PHYS,
	DEP_LIFE,
	DEP_ME,
	DEP_CHEM
} protocol_dep;

// Unit state enum
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

// Macros categorizing states
#define STATE_KIND_ATTACK(x) ((x) == STATE_ATTACK_RIGHT || (x) == STATE_ATTACK_DOWN || (x) == STATE_ATTACK_LEFT || (x) == STATE_ATTACK_UP)
#define STATE_KIND_SKILL(x) ((x) == STATE_SKILL_RIGHT || (x) == STATE_SKILL_DOWN || (x) == STATE_SKILL_LEFT || (x) == STATE_SKILL_UP)

// Direction enum
typedef enum {
	DIRECTION_NULL,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_LEFT,
	DIRECTION_DOWN,
} protocol_direction;

// A complete unit structure.
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
	int invincible;
} protocol_unit;

// Flag structure
typedef struct {
	protocol_team team;
	int x, y;
} protocol_flag;

// Poison, made by CHEM unit
typedef struct {
	bool valid;
	protocol_team team;
	int x, y;
	int span;
} protocol_poison;

// Petal, made by LIFE unit
typedef struct {
	bool valid;
	protocol_team team;
	int x, y;
	protocol_direction direction;
} protocol_petal;

// Mushroom, made by CHEM unit, a hero skill.
typedef struct {
	bool valid;
	protocol_team team;
	int x, y;
} protocol_mushroom;

#define CLIENT_NUM_MAX 2
#define UNIT_NUM_MAX 6
#define FLAG_NUM_MAX 5
#define POISON_NUM_MAX 30
#define PETAL_NUM_MAX 12
#define MUSHROOM_NUM_MAX 5

// A complete network protocol.
typedef struct {
	protocol_unit unit[UNIT_NUM_MAX];
	protocol_flag flag[FLAG_NUM_MAX];
	protocol_poison poison[POISON_NUM_MAX];
	protocol_petal petal[PETAL_NUM_MAX];
	protocol_mushroom mushroom[MUSHROOM_NUM_MAX];
	int score[2];
	int turnleft;
} protocol_data;

// Command enum
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

// Macros categorizing commands
#define COMMAND_KIND_MOVE(x) ((x) == COMMAND_MOVE_RIGHT || (x) == COMMAND_MOVE_DOWN || (x) == COMMAND_MOVE_LEFT || (x) == COMMAND_MOVE_UP)
#define COMMAND_KIND_ATTACK(x) ((x) == COMMAND_ATTACK_RIGHT || (x) == COMMAND_ATTACK_DOWN || (x) == COMMAND_ATTACK_LEFT || (x) == COMMAND_ATTACK_UP)
#define COMMAND_KIND_SKILL(x) ((x) == COMMAND_SKILL_RIGHT || (x) == COMMAND_SKILL_DOWN || (x) == COMMAND_SKILL_LEFT || (x) == COMMAND_SKILL_UP)
#define COMMAND_KIND_SPAWN(x) ((x) == COMMAND_SPAWN_CSE || (x) == COMMAND_SPAWN_PHYS || (x) == COMMAND_SPAWN_LIFE || (x) == COMMAND_SPAWN_ME || (x) == COMMAND_SPAWN_CHEM)

typedef struct {
	protocol_command command[UNIT_NUM_MAX];
} protocol_command_array;

// Other various macro definitions.
#define MAP_WIDTH 15
#define MAP_HEIGHT 15

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

#define FRAME_INTERVAL 16
#define FRAME_PER_TURN 30
#define DELTA_PER_TURN (1.0 / FRAME_PER_TURN)
#define ANIMATION_SPAN 15
#define DELTA_ANIMATION (1.0 / ANIMATION_SPAN)

#define RESPAWN_COOLTIME 10

//socket [x] is mapped to unit number [x]

//NETWORK PROTOCOL

#define PORT_STRING "2222"
#define SERV_IP_STRING "141.223.85.241"


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

#define FLAG1_X 7
#define FLAG1_Y 1
#define FLAG2_X 7
#define FLAG2_Y 4
#define FLAG3_X 7
#define FLAG3_Y 7
#define FLAG4_X 7
#define FLAG4_Y 10
#define FLAG5_X 7
#define FLAG5_Y 13

#define TURN_MAX 300

#define INVALID_POISON_INDEX -1
#define INVALID_MUSHROOM_INDEX -1
#define INVALID_PETAL_INDEX -1

#define BLACKHOLE_DAMAGE 10
#define ACCIDENT_DAMAGE 10
#define POISON_LENGTH 4

#define INVINCIBLE_SPAN 5

#define DEATH_PENALTY 10
#define FLAG_SCORE 50

#define HERO_DELAY 5

#define ARBITRARY_BIG_NUM 999