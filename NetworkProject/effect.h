//
// CNH 2016.07.20
// effect class
//

#pragma once

#include <iostream>
#include <vector>
#include "protocol.h"
#include "gui.h"
#include "timer.h"

class EffectBase {
protected:
	float phase;
	float x, y;
public:
	EffectBase(float x, float y) : x(x), y(y), phase(0.0) { };
	virtual void draw() const = 0;
	virtual void update() { phase += Timer::getDeltaPerTurn(); };
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
	protocol_team team;
public:
	EffectTeam(protocol_team t, float x, float y) : team(t), EffectBase(x, y) {};
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
		}
		if (y == y2) {
			horizontal = true;
		}
	};
	void draw() const;
};

class EffectMEAccidentH : public EffectTeam {
protected:
	float scale;
	protocol_direction d;
public:
	EffectMEAccidentH(protocol_team t, float x, float y, int length, bool flip) : d(d), EffectTeam(t, x, y) {
		scale = (float)length / 120.0 * GUI_CELL_WIDTH;
		if (flip)
			scale *= -1.0;
	};
	void draw() const;
};

class EffectMEAccidentV : public EffectTeam {
protected:
	float scale;
	protocol_direction d;
public:
	EffectMEAccidentV(protocol_team t, float x, float y, int length, bool flip) : d(d), EffectTeam(t, x, y) {
		scale = (float)length / 120.0 * GUI_CELL_HEIGHT;
		if (flip)
			scale *= -1.0;
	};
	void draw() const;
};

class EffectOwn : public EffectBase {
public:
	EffectOwn(float x, float y) : EffectBase(x, y) {};
	void draw() const;
};

class EffectOwnFlag : public EffectBase {
public:
	EffectOwnFlag(float x, float y) : EffectBase(x, y) {};
	void update() { phase += Timer::getDeltaPerTurn(); }
	void draw() const;
};

class EffectCSEBlink : public EffectBase {
protected:
	bool fliped;
public:
	EffectCSEBlink(float x, float y, bool fliped) : EffectBase(x, y), fliped(fliped) {};
	void draw() const;
};

class EffectDeath : public EffectTeam {
public:
	EffectDeath(protocol_team t, float x, float y) : EffectTeam(t, x, y) {};
	void update() { phase += Timer::getDeltaPerTurn(); }
	void draw() const;
};

class EffectBlackhole : public EffectTeam {
public:
	EffectBlackhole(protocol_team t, float x, float y) : EffectTeam(t, x, y) {};
	void update() { phase += Timer::getDeltaPerTurn(); }
	void draw() const;
};

class EffectStorm : public EffectBase {
public:
	EffectStorm(float x, float y) : EffectBase(x, y) {};
	void update() { phase += Timer::getDeltaPerTurn(); }
	void draw() const;
};