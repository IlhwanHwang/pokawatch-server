//
// CNH 2016.07.20
// effect class
//

#include "effect.h"
#include "gui.h"
#include "resource.h"
#include "draw.h"
#include <cmath>
#include "utility.h"

std::vector<EffectBase*> Effect::pool;

void Effect::push(EffectBase* eff) {
	pool.push_back(eff);
}

void Effect::draw() {
	for (int i = 0; i < pool.size(); i++) {
		pool[i]->draw();
	}
}

void Effect::update() {
	for (int i = 0; i < pool.size(); i++) {
		pool[i]->update();
		if (!pool[i]->isValid()) {
			delete pool[i];
			pool.erase(pool.begin() + i);
			i--;
		}
	}
}

void Effect::clear() {
	for (int i = 0; i < pool.size(); i++) {
		delete pool[i];
	}

	pool.clear();
}

void EffectCSEAttack::draw() const {
	Draw::qonmap(
		Rspr::attackCSE[(int)(phase * 4)], 
		-0.1, x, y, 0.0);
}

void EffectMEAccidentH::draw() const {
	Draw::qonmapSB(
		team == TEAM_POSTECH ? Rspr::MEAccidentPH : Rspr::MEAccidentKH,
		-0.6, x, y, 0.5, scale, 1.0, Color::white, 1.0 - phase);
}

void EffectMEAccidentV::draw() const {
	Draw::qonmapSB(
		team == TEAM_POSTECH ? Rspr::MEAccidentPV : Rspr::MEAccidentKV,
		-0.6, x, y, 0.5, 1.0, scale, Color::white, 1.0 - phase);
}

void EffectPHYSAttack::draw() const {
	int ind = (int)(phase * 8) % 4;

	if (horizontal) {
		Sprite& spr =
			team == TEAM_POSTECH ?
			Rspr::attackPHYSPH[ind] :
			Rspr::attackPHYSKH[ind];
		for (float dx = fminf(x, x2); dx <= fmaxf(x, x2); dx += 1.0) {
			Draw::qonmap(spr, 0.0, dx, y, 0.5);
		}
	}
	else {
		Sprite& spr =
			team == TEAM_POSTECH ?
			Rspr::attackPHYSPV[ind] :
			Rspr::attackPHYSKV[ind];
		for (float dy = fminf(y, y2); dy <= fmaxf(y, y2); dy += 1.0) {
			Draw::qonmap(spr, 0.0, x, dy, 0.5);
		}
	}
}

void EffectOwn::draw() const {
	Draw::draw(Rspr::own[(int)(phase * 8)], x, y);
}

void EffectOwnFlag::draw() const {
	Draw::draw(Rspr::ownFlag[(int)(phase * 8)], x, y);
}

void EffectCSEBlink::draw() const {
	Draw::qonmapSB(Rspr::CSEBlink, -0.1, x, y, 0.0, fliped ? -1.0 : 1.0, 1.0, Color::white, 1.0 - phase);
}