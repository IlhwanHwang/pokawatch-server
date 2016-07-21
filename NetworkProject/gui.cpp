//
// CNH 2016.05.25
// GUI definition
//

#include "gui.h"

float Gui::animationPhase = 0.0;
float Gui::animationPostPhase = 0.0;
float Gui::animationFullPhase = 0.0;
float Gui::animationIndpPhase = 0.0;

void Gui::turn() {
	animationPhase = 0.0;
	animationPostPhase = 0.0;
	animationFullPhase = 0.0;
}

void Gui::update() {
	if (animationPhase < 1.0) {
		animationPhase += DELTA_ANIMATION;
	}
	else {
		animationPostPhase += DELTA_POSTANIMATION;
		animationPhase = 1.0;
	}

	animationFullPhase += DELTA_PER_TURN;

	animationIndpPhase += 1.0;
}
