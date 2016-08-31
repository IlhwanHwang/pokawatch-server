//
// CNH 2016.08.30
// Audio module
//

#include "audio.h"
#include <iostream>

irrklang::ISoundEngine* Audio::engine;
irrklang::ISoundSource* Audio::bgmNormal;
irrklang::ISoundSource* Audio::bgmExtra;
irrklang::ISound* Audio::bgm = nullptr;
bool Audio::isextra;
bool Audio::isturn;
irrklang::ik_u32 Audio::lastPlayPosition;

void Audio::init() {
	engine = irrklang::createIrrKlangDevice();

	if (!engine)
	{
		std::cout << "Could not startup engine\n" << std::endl;
		return;
	}

	lastPlayPosition = 300;
	isextra = false;
	isturn = false;
	bgmNormal = engine->addSoundSourceFromFile("sounds/KOTH(Retro Mix).ogg", irrklang::ESM_AUTO_DETECT, true);
	bgmExtra = engine->addSoundSourceFromFile("sounds/KOTH(Kapo Mix).ogg", irrklang::ESM_AUTO_DETECT, true);
	bgmNormal->setDefaultVolume(0.8);
	bgmExtra->setDefaultVolume(1.0);
}

void Audio::start() {
	bgm = engine->play2D(bgmNormal, false, false, true);
}

void Audio::extra() {
	if (isextra)
		return;

	isextra = true;

	irrklang::ik_u32 pos = bgm->getPlayPosition();

	bgm->stop();
	bgm->drop();
	bgm = engine->play2D(bgmExtra, false, false, true);
	bgm->setPlayPosition(pos);
}

void Audio::update() {
	if (bgm == nullptr)
		return;

	irrklang::ik_u32 pos = bgm->getPlayPosition();

	if (lastPlayPosition + 500 < pos) {
		lastPlayPosition += 500;
		isturn = true;
	}
}

bool Audio::isTurn() {
	if (isturn) {
		isturn = false;
		return true;
	}
	else
		return false;
}

void Audio::play(irrklang::ISoundSource* sfx) {
	engine->play2D(sfx);
}