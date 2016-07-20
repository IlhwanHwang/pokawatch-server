//
// CNH 2016.07.20
// effect class
//

#include "effect.h"
#include "gui.h"
#include "resource.h"
#include "draw.h"

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
	if (Gui::isPost()) {
		Draw::qonmap(
			Rspr::attackCSE[
				Gui::aniPostPhaseCombinate(4)
			], 
			0.0, x, y, 0.0);
	}
}

void EffectPHYSAttack::draw() const {
	if (horizontal) {
		Sprite& spr
		for (float dx = x; dx <= x2; dx += 1.0) {
			Draw::qonmap()
		}
	}
}