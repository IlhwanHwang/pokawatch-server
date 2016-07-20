#pragma once

class Ai {
public:
	static void aiInit(void);
	static void ai(void);
	static void CharacterInit(int i, int x);
	static void move(int i, int x);
	static void attack(int i, int x);
	static void skill(int i, char x);
	static void flag(int i);
};