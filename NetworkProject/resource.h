//
// CNH 2016.05.22
// resources
//

#pragma once

#include "sprite.h"

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
	static Sprite attackPHYSPH[4];
	static Sprite attackPHYSPV[4];
	static Sprite attackPHYSKH[4];
	static Sprite attackPHYSKV[4];
	static Sprite CSEBlink;
	static Sprite attackCSE[4];
	static Sprite sparkboom[4];
	static Sprite mushroom;

	static Sprite hero;
	static Sprite stun[4];

	static Sprite tileLight;
	static Sprite tileDark;

	static Sprite flagNull;
	static Sprite flagP[4];
	static Sprite flagK[4];

	static Sprite number[10];
	static Sprite bignumber[10];

	static Sprite bg;
	static Sprite intengrad;

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

	static Sprite own[8];
	static Sprite ownFlag[8];

	static Sprite infoMain;
	static Sprite infoServer;
	static Sprite infoClient1;
	static Sprite infoClient2;
	static Sprite infoClient3;
};

class Resource {
public:
	static void init();
	static void postinit();
};