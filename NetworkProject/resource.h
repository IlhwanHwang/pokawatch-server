//
// CNH 2016.05.22
// resources
//

#pragma once

#include "sprite.h"
#include <irrKlang.h>

// Every resource is saved on static context
class Rspr {
public:
	static Sprite temp;
	static Sprite error;

	static Sprite unitCSEP;
	static Sprite unitCHEMP;
	static Sprite unitMEP;
	static Sprite unitLIFEP;
	static Sprite unitPHYSP;
	static Sprite unitCSEK;
	static Sprite unitCHEMK;
	static Sprite unitMEK;
	static Sprite unitLIFEK;
	static Sprite unitPHYSK;
	static Sprite unitHeartP;
	static Sprite unitHeartK;

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
	static Sprite attackPHYSPH[4];
	static Sprite attackPHYSPV[4];
	static Sprite attackPHYSKH[4];
	static Sprite attackPHYSKV[4];
	static Sprite PHYSBlackholeP[8];
	static Sprite PHYSBlackholeK[8];
	static Sprite CSEBlink;
	static Sprite MEAccidentPH;
	static Sprite MEAccidentPV;
	static Sprite MEAccidentKH;
	static Sprite MEAccidentKV;
	static Sprite attackCSE[4];
	static Sprite sparkboom[4];
	static Sprite mushroom;
	static Sprite LIFEBlossom[8];

	static Sprite deathP[8];
	static Sprite deathK[8];

	static Sprite hero;
	static Sprite stun[4];

	static Sprite tileLight;
	static Sprite tileDark;

	static Sprite flagNull;
	static Sprite flagP[4];
	static Sprite flagK[4];
	static Sprite extra[2];

	static Sprite number[10];

	static Sprite bg;
	static Sprite wholedim;

	static Sprite winPostech;
	static Sprite winKaist;
	static Sprite winDraw;

	static Sprite kill[4];

	static Sprite pointNorm;
	static Sprite pointP;
	static Sprite pointK;
	static Sprite pointBarP;
	static Sprite pointBarK;
	static Sprite pointBarFrame;

	static Sprite ownPoint;
	static Sprite ownFlag[8];
	static Sprite ownGaugeP[5];
	static Sprite ownGaugeK[5];

	static Sprite turnWaitPlay;
	static Sprite turnWaitWait;
};

class Rsfx {
public:
	static irrklang::ISoundSource* death;
};

class Resource {
public:
	static void init();
	static void initSfx();
	static void postinit();
};