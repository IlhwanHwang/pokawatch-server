//
// CNH 2016.05.22
// resources
//

#pragma once

#include "sprite.h"

class Rspr {
public:
	static Sprite temp;
	static Sprite error;

	static Sprite unitCSE;
	static Sprite unitCHEM;
	static Sprite unitME;
	static Sprite unitLIFE;
	static Sprite unitPHYS;
	static Sprite unitHeart;

	static Sprite faceCSE;
	static Sprite faceCHEM;
	static Sprite faceME;
	static Sprite faceLIFE;
	static Sprite facePHYS;
	static Sprite faceDEAD;
	static Sprite faceFrame;

	static Sprite petal;
	static Sprite poison;
	static Sprite beamH[4];
	static Sprite beamV[4];
	static Sprite spark[4];
	static Sprite sparkboom[4];
	static Sprite mushroom;

	static Sprite hero;
	static Sprite stun[4];

	static Sprite tileLight;
	static Sprite tileDark;

	static Sprite flagNull;
	static Sprite flagPostech;
	static Sprite flagKaist;

	static Sprite number[10];
	static Sprite bignumber[10];
};

class Resource {
public:
	static void init();
};