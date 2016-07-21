#pragma once

#include "protocol.h"

class Ai {
public:
	static void aiInit(void);
	static void ai(protocol_data AI_info);
	static void CharacterInit(int i, int x);
	static void move(int i, protocol_direction x);
	static void attack(int i, protocol_direction x);
	static void skill(int i, protocol_direction x);
	static void spawn(int i, protocol_dep x);
};