//
// CNH 2016.07.20
// effect class
//

#include "effect.h"
#include "gui.h"
#include "resource.h"
#include "draw.h"
#include <cmath>

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
	if (!Gui::isPost())
		return;

	int ind = Gui::aniPostPhaseCombinate(8) % 4;

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

	std::cout << horizontal << std::endl;
	std::cout << "x from " << x << " to " << x2 << std::endl;
	std::cout << "y from " << y << " to " << y2 << std::endl;
}