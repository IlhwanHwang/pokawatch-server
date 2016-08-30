//
// CNH 2016.05.25
// GUI definition
//

#include "gui.h"
#include "shader.h"
#include "timer.h"

float Gui::animationPhase = 0.0;
float Gui::animationPostPhase = 0.0;
float Gui::animationFullPhase = 0.0;
float Gui::animationIndpPhase = 0.0;
float Gui::shakePhase = 0.0;
float Gui::shakeMagnitude = 0.0;

void Gui::turn() {
	animationPhase = 0.0;
	animationPostPhase = 0.0;
	animationFullPhase = 0.0;
}

void Gui::update() {
	if (animationPhase < 1.0) {
		animationPhase += Timer::getDeltaPerTurn() / ANIMATION_SPAN_RATE;
	}
	else if (animationPostPhase < 1.0) {
		animationPostPhase += Timer::getDeltaPerTurn() / (1.0 - ANIMATION_SPAN_RATE);
		animationPhase = 1.0;
	}
	else {
		animationPhase = 1.0;
		animationPostPhase = 1.0;
	}

	if (animationFullPhase < 1.0) {
		animationFullPhase += Timer::getDeltaPerTurn();
	}
	else {
		animationFullPhase = 1.0;
	}

	animationIndpPhase += 1.0;

	if (shakePhase > 0.0) {
		shakePhase -= Timer::getDeltaPerTurn();
	}
	else {
		shakePhase = 0.0;
		shakeMagnitude = 0.0;
	}
}

void Gui::drawPre() {
	float off = aniFullPhaseCombinate(8) % 2 == 0 ? -1.0 : 1.0;
	Shader::setOffset(0.0, off * shakeMagnitude * shakePhase / WINDOW_HEIGHT * 6.0);
}