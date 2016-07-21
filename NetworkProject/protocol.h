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

// Direction enum
typedef enum {
	DIRECTION_NULL,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_LEFT,
	DIRECTION_DOWN,
} protocol_direction;

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
#define TEAM_NUM_MAX 2
#define UNIT_NUM_MAX 6
#define POISON_NUM_MAX 30
#define PETAL_NUM_MAX 12
#define MUSHROOM_NUM_MAX 5

// A complete network protocol.
typedef struct {
	protocol_unit unit[UNIT_NUM_MAX];
	protocol_poison poison[POISON_NUM_MAX];
	protocol_petal petal[PETAL_NUM_MAX];
	protocol_mushroom mushroom[MUSHROOM_NUM_MAX];
	int own[TEAM_NUM_MAX];
	int win[TEAM_NUM_MAX];
	int extra;
	int elapsed;
} protocol_data;

// Command enum
typedef enum {
	COMMAND_NULL,
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

// Other various macro definitions.
#define MAP_WIDTH 21
#define MAP_HEIGHT 15

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960

#define FRAME_INTERVAL 16
#define FRAME_PER_TURN 32
#define DELTA_PER_TURN (1.0 / FRAME_PER_TURN)
#define ANIMATION_SPAN 16
#define DELTA_ANIMATION (1.0 / ANIMATION_SPAN)
#define POSTANIMATION_SPAN (FRAME_PER_TURN - ANIMATION_SPAN)
#define DELTA_POSTANIMATION (1.0 / POSTANIMATION_SPAN)

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

#define MESSAGE_T0_CLIENT_SIZE (sizeof(protocol_data)+1)
#define MESSAGE_TO_SERVER_SIZE 16

#define GAME_START_CHAR 'G'

#define TEAM_POSTECH_SPAWN_X 0
#define TEAM_POSTECH_SPAWN_Y 0
#define TEAM_KAIST_SPAWN_X 3
#define TEAM_KAIST_SPAWN_Y 0

#define POINT_X1 (MAP_WIDTH / 2 - 2)
#define POINT_Y1 (MAP_HEIGHT / 2 - 3)
#define POINT_X2 (MAP_WIDTH / 2 + 2)
#define POINT_Y2 (MAP_HEIGHT / 2 + 3)
#define POINT_TURN_OWN 10
#define POINT_TURN_WIN 30
#define POINT_TURN_EXTRA 5
#define TURN_MAX 300

#define INVALID_POISON_INDEX -1
#define INVALID_MUSHROOM_INDEX -1
#define INVALID_PETAL_INDEX -1

#define BLACKHOLE_DAMAGE 10
#define ACCIDENT_DAMAGE 10
#define POISON_LENGTH 4

#define INVINCIBLE_SPAN 5

#define HERO_DELAY 5

#define ARBITRARY_BIG_NUM 999

inline int team_to_index(protocol_team t) {
	switch (t) {
	case TEAM_POSTECH:
		return 0;
	case TEAM_KAIST:
		return 1;
	default:
		return -1;
	}
}

inline protocol_team index_to_team(int i) {
	switch (i) {
	case 0:
		return TEAM_POSTECH;
	case 1:
		return TEAM_KAIST;
	default:
		return TEAM_NULL;
	}
}

inline protocol_team team_invert(protocol_team t) {
	switch (t) {
	case TEAM_POSTECH:
		return TEAM_KAIST;
	case TEAM_KAIST:
		return TEAM_POSTECH;
	default:
		return TEAM_NULL;
	}
}

inline int direction_to_dx(protocol_direction d) {
	switch (d) {
	case DIRECTION_RIGHT:
		return 1;
	case DIRECTION_UP:
		return 0;
	case DIRECTION_LEFT:
		return -1;
	case DIRECTION_DOWN:
		return 0;
	}
}

inline int direction_to_dy(protocol_direction d) {
	switch (d) {
	case DIRECTION_RIGHT:
		return 0;
	case DIRECTION_UP:
		return 1;
	case DIRECTION_LEFT:
		return 0;
	case DIRECTION_DOWN:
		return -1;
	}
}

// categorizing states
inline bool state_kind_attack(protocol_state s) {
	switch (s) {
	case STATE_ATTACK_RIGHT:
	case STATE_ATTACK_UP:
	case STATE_ATTACK_LEFT:
	case STATE_ATTACK_DOWN:
		return true;
	default:
		return false;
	}
}

inline bool state_kind_skill(protocol_state s) {
	switch (s) {
	case STATE_SKILL_RIGHT:
	case STATE_SKILL_UP:
	case STATE_SKILL_LEFT:
	case STATE_SKILL_DOWN:
		return true;
	default:
		return false;
	}
}

// converting states
inline protocol_direction state_to_direction(protocol_state s) {
	switch (s) {
	case STATE_ATTACK_RIGHT:
	case STATE_SKILL_RIGHT:
		return DIRECTION_RIGHT;
	case STATE_ATTACK_UP:
	case STATE_SKILL_UP:
		return DIRECTION_UP;
	case STATE_ATTACK_LEFT:
	case STATE_SKILL_LEFT:
		return DIRECTION_LEFT;
	case STATE_ATTACK_DOWN:
	case STATE_SKILL_DOWN:
		return DIRECTION_DOWN;
	default:
		return DIRECTION_NULL;
	}
}

inline protocol_state direction_to_attackstate(protocol_direction d) {
	switch (d) {
	case DIRECTION_RIGHT:
		return STATE_ATTACK_RIGHT;
	case DIRECTION_UP:
		return STATE_ATTACK_UP;
	case DIRECTION_LEFT:
		return STATE_ATTACK_LEFT;
	case DIRECTION_DOWN:
		return STATE_ATTACK_DOWN;
	default:
		return STATE_NULL;
	}
}

inline protocol_state direction_to_skillstate(protocol_direction d) {
	switch (d) {
	case DIRECTION_RIGHT:
		return STATE_SKILL_RIGHT;
	case DIRECTION_UP:
		return STATE_SKILL_UP;
	case DIRECTION_LEFT:
		return STATE_SKILL_LEFT;
	case DIRECTION_DOWN:
		return STATE_SKILL_DOWN;
	default:
		return STATE_NULL;
	}
}

// categorizing commands
inline bool command_kind_move(protocol_command x) {
	return x == COMMAND_MOVE_RIGHT || x == COMMAND_MOVE_DOWN || x == COMMAND_MOVE_LEFT || x == COMMAND_MOVE_UP;
}

inline bool command_kind_attack(protocol_command x) {
	return x == COMMAND_ATTACK_RIGHT || x == COMMAND_ATTACK_DOWN || x == COMMAND_ATTACK_LEFT || x == COMMAND_ATTACK_UP;
}

inline bool command_kind_skill(protocol_command x) {
	return x == COMMAND_SKILL_RIGHT || x == COMMAND_SKILL_DOWN || x == COMMAND_SKILL_LEFT || x == COMMAND_SKILL_UP;
}

inline bool command_kind_spawn(protocol_command x) {
	return x == COMMAND_SPAWN_CSE || x == COMMAND_SPAWN_PHYS || x == COMMAND_SPAWN_LIFE || x == COMMAND_SPAWN_ME || x == COMMAND_SPAWN_CHEM;
}

// converting commands
inline protocol_direction command_to_direction(protocol_command x) {
	switch (x) {
	case COMMAND_MOVE_RIGHT:
	case COMMAND_ATTACK_RIGHT:
	case COMMAND_SKILL_RIGHT:
		return DIRECTION_RIGHT;
	case COMMAND_MOVE_UP:
	case COMMAND_ATTACK_UP:
	case COMMAND_SKILL_UP:
		return DIRECTION_UP;
	case COMMAND_MOVE_LEFT:
	case COMMAND_ATTACK_LEFT:
	case COMMAND_SKILL_LEFT:
		return DIRECTION_LEFT;
	case COMMAND_MOVE_DOWN:
	case COMMAND_ATTACK_DOWN:
	case COMMAND_SKILL_DOWN:
		return DIRECTION_DOWN;
	default:
		return DIRECTION_NULL;
	}
}

inline protocol_dep command_to_dep(protocol_command x) {
	switch (x) {
	case COMMAND_SPAWN_CSE:
		return DEP_CSE;
	case COMMAND_SPAWN_PHYS:
		return DEP_CSE;
	case COMMAND_SPAWN_LIFE:
		return DEP_CSE;
	case COMMAND_SPAWN_ME:
		return DEP_CSE;
	case COMMAND_SPAWN_CHEM:
		return DEP_CSE;
	default:
		return DEP_NULL;
	}
}

inline protocol_command direction_to_movecommand(protocol_direction d) {
	switch (d) {
	case DIRECTION_RIGHT:
		return COMMAND_MOVE_RIGHT;
	case DIRECTION_UP:
		return COMMAND_MOVE_UP;
	case DIRECTION_LEFT:
		return COMMAND_MOVE_LEFT;
	case DIRECTION_DOWN:
		return COMMAND_MOVE_DOWN;
	default:
		return COMMAND_NULL;
	}
}

inline protocol_command direction_to_attackcommand(protocol_direction d) {
	switch (d) {
	case DIRECTION_RIGHT:
		return COMMAND_ATTACK_RIGHT;
	case DIRECTION_UP:
		return COMMAND_ATTACK_UP;
	case DIRECTION_LEFT:
		return COMMAND_ATTACK_LEFT;
	case DIRECTION_DOWN:
		return COMMAND_ATTACK_DOWN;
	default:
		return COMMAND_NULL;
	}
}

inline protocol_command direction_to_skillcommand(protocol_direction d) {
	switch (d) {
	case DIRECTION_RIGHT:
		return COMMAND_SKILL_RIGHT;
	case DIRECTION_UP:
		return COMMAND_SKILL_UP;
	case DIRECTION_LEFT:
		return COMMAND_SKILL_LEFT;
	case DIRECTION_DOWN:
		return COMMAND_SKILL_DOWN;
	default:
		return COMMAND_NULL;
	}
}

inline protocol_direction direction_flip(protocol_direction d) {
	switch (d) {
	case DIRECTION_RIGHT:
		return DIRECTION_LEFT;
	case DIRECTION_UP:
		return DIRECTION_DOWN;
	case DIRECTION_LEFT:
		return DIRECTION_RIGHT;
	case DIRECTION_DOWN:
		return DIRECTION_UP;
	default:
		return DIRECTION_NULL;
	}
}


inline protocol_direction direction_mirror(protocol_direction d) {
	switch (d) {
	case DIRECTION_RIGHT:
		return DIRECTION_LEFT;
	case DIRECTION_UP:
		return DIRECTION_UP;
	case DIRECTION_LEFT:
		return DIRECTION_RIGHT;
	case DIRECTION_DOWN:
		return DIRECTION_DOWN;
	default:
		return DIRECTION_NULL;
	}
}


inline protocol_command dep_to_spawncommand(protocol_dep dep) {
	switch (dep) {
	case DEP_CSE:
		return COMMAND_SPAWN_CSE;
	case DEP_PHYS:
		return COMMAND_SPAWN_PHYS;
	case DEP_LIFE:
		return COMMAND_SPAWN_LIFE;
	case DEP_ME:
		return COMMAND_SPAWN_ME;
	case DEP_CHEM:
		return COMMAND_SPAWN_CHEM;
	default:
		return COMMAND_NULL;
	}
}

#define DEP_SELECT(dep, s1, s2, s3, s4, s5) \
	((dep) == DEP_CSE ? (s1) : \
	((dep) == DEP_PHYS ? (s2) : \
	((dep) == DEP_LIFE ? (s3) : \
	((dep) == DEP_ME ? (s4) : (s5) \
	))))