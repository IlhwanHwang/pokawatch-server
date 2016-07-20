//
// CNH 2016.07.20
// effect class
//

#pragma once

#include <iostream>
#include <vector>
#include "protocol.h"

class EffectBase {
protected:
	float phase;
	float x, y;
public:
	EffectBase(float x, float y) : x(x), y(y), phase(0.0) { };
	virtual void draw() const = 0;
	virtual void update() { phase += DELTA_PER_TURN; };
	bool isValid() { return phase < 1.0; }
};

class Effect {
private:
	static std::vector<EffectBase*> pool;
public:
	static void push(EffectBase* eff);
	static void draw();
	static void update();
	static void clear();
};

class EffectTeam : public EffectBase {
protected:
	protocol_team t;
public:
	EffectTeam(protocol_team t, float x, float y) : t(t), EffectBase(x, y) {};
};

class EffectCSEAttack : public EffectTeam {
public:
	EffectCSEAttack(protocol_team t, float x, float y) : EffectTeam(t, x, y) {};
	void draw() const;
};

class EffectPHYSAttack : public EffectTeam {
protected:
	bool horizontal;
	float x2, y2;
public:
	EffectPHYSAttack(protocol_team t, float x, float y, float x2, float y2)
		: x2(x2), y2(y2), EffectTeam(t, x, y) {
		if (x == x2) {
			horizontal = false;
			if (y > y2) {
				float tmp = y;
				y = y2;
				y2 = tmp;
			}
		}
		if (y == y2) {
			horizontal = true;
			if (x > x2) {
				float tmp = x;
				x = x2;
				x2 = tmp;
			}
		}
	};
	void draw() const;
};