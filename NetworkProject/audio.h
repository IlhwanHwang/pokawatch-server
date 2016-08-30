//
// CNH 2016.08.30
// Audio module
//

#pragma once

#include <irrKlang.h>

class Audio {
private:
	static irrklang::ISoundEngine* engine;
	static irrklang::ISoundSource* bgmNormal;
	static irrklang::ISoundSource* bgmExtra;
	static irrklang::ISound* bgm;
	static bool isextra;
	static bool isturn;
	static irrklang::ik_u32 lastPlayPosition;
public:
	static void init();
	static void start();
	static void update();
	static void extra();
	static bool isTurn();
};