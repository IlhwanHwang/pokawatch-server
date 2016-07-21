//
// CNH 2016.05.22
// resources
//

#include "resource.h"
#include <string>

Sprite Rspr::temp;
Sprite Rspr::error;

Sprite Rspr::unitCSEP;
Sprite Rspr::unitCHEMP;
Sprite Rspr::unitMEP;
Sprite Rspr::unitLIFEP;
Sprite Rspr::unitPHYSP;
Sprite Rspr::unitCSEK;
Sprite Rspr::unitCHEMK;
Sprite Rspr::unitMEK;
Sprite Rspr::unitLIFEK;
Sprite Rspr::unitPHYSK;
Sprite Rspr::unitHeart;

Sprite Rspr::faceCSE;
Sprite Rspr::faceCHEM;
Sprite Rspr::faceME;
Sprite Rspr::faceLIFE;
Sprite Rspr::facePHYS;
Sprite Rspr::faceDEAD;
Sprite Rspr::faceFrame;

Sprite Rspr::petal;
Sprite Rspr::poison;
Sprite Rspr::beamH[4];
Sprite Rspr::beamV[4];
Sprite Rspr::attackPHYSPH[4];
Sprite Rspr::attackPHYSPV[4];
Sprite Rspr::attackPHYSKH[4];
Sprite Rspr::attackPHYSKV[4];
Sprite Rspr::CSEBlink;
Sprite Rspr::attackCSE[4];
Sprite Rspr::sparkboom[4];
Sprite Rspr::mushroom;

Sprite Rspr::hero;
Sprite Rspr::stun[4];

Sprite Rspr::tileLight;
Sprite Rspr::tileDark;
Sprite Rspr::flagNull;
Sprite Rspr::flagP[4];
Sprite Rspr::flagK[4];

Sprite Rspr::number[10];
Sprite Rspr::bignumber[10];

Sprite Rspr::bg;
Sprite Rspr::intengrad;

Sprite Rspr::winPostech;
Sprite Rspr::winKaist;
Sprite Rspr::winDraw;

Sprite Rspr::pointNorm;
Sprite Rspr::pointP;
Sprite Rspr::pointK;
Sprite Rspr::pointBarP;
Sprite Rspr::pointBarK;
Sprite Rspr::pointBarFrame;

Sprite Rspr::own[8];
Sprite Rspr::ownFlag[8];

Sprite Rspr::infoMain;
Sprite Rspr::infoServer;
Sprite Rspr::infoClient1;
Sprite Rspr::infoClient2;
Sprite Rspr::infoClient3;

#define RESOURCE_LOCATION "images\\"

// Macro to load multiple consecutive images at once
#define MULTILOAD(buf, res, num, filename) \
	for (int i = 0; i < num; i++) \
	{ \
		sprintf(buf, RESOURCE_LOCATION filename ".png", i); \
		Rspr::res[i].load(buf).dot(); \
	}

// Single image load macro
#define LOAD(res, filename) Rspr::res.load(RESOURCE_LOCATION filename ".png")

void Resource::init() {
	char buf[100];

	LOAD(infoMain, "info_main").setOffset(0.0, 240.0);
	LOAD(infoServer, "info_server").setOffset(0.0, 240.0);
	LOAD(infoClient1, "info_client1").setOffset(0.0, 240.0);
	LOAD(infoClient2, "info_client2").setOffset(0.0, 240.0);
	LOAD(infoClient3, "info_client3").setOffset(0.0, 240.0);
}

void Resource::postinit() {
	char buf[100];

	LOAD(temp, "temp").setOffset(20.0, 80.0).dot();
	LOAD(error, "error");

	LOAD(unitCSEP, "body_cse_p").dot().setOffset(20.0, 74.0);
	LOAD(unitCHEMP, "body_chem_p").dot().setOffset(20.0, 74.0);
	LOAD(unitMEP, "body_me_p").dot().setOffset(30.0, 70.0);
	LOAD(unitLIFEP, "body_life_p").dot().setOffset(20.0, 74.0);
	LOAD(unitPHYSP, "body_phys_p").dot().setOffset(20.0, 74.0);
	LOAD(unitCSEK, "body_cse_k").dot().setOffset(20.0, 74.0);
	LOAD(unitCHEMK, "body_chem_k").dot().setOffset(20.0, 74.0);
	LOAD(unitMEK, "body_me_k").dot().setOffset(30.0, 70.0);
	LOAD(unitLIFEK, "body_life_k").dot().setOffset(20.0, 74.0);
	LOAD(unitPHYSK, "body_phys_k").dot().setOffset(20.0, 74.0);
	LOAD(unitHeart, "heart").dot();

	LOAD(faceCSE, "face_cse");
	LOAD(faceCHEM, "face_chem");
	LOAD(faceME, "face_me");
	LOAD(faceLIFE, "face_life");
	LOAD(facePHYS, "face_phys");
	LOAD(faceDEAD, "face_dead");
	LOAD(faceFrame, "face_frame");

	LOAD(petal, "petal").dot();
	LOAD(poison, "poison").dot();
	MULTILOAD(buf, beamH, 4, "beam_h%d");
	MULTILOAD(buf, beamV, 4, "beam_v%d");
	MULTILOAD(buf, attackPHYSPH, 4, "attack_phys_p_h_%d");
	MULTILOAD(buf, attackPHYSPV, 4, "attack_phys_p_v_%d");
	MULTILOAD(buf, attackPHYSKH, 4, "attack_phys_k_h_%d");
	MULTILOAD(buf, attackPHYSKV, 4, "attack_phys_k_v_%d");
	MULTILOAD(buf, stun, 4, "stun%d");
	LOAD(CSEBlink, "blink_cse").dot().setOffset(20.0, 74.0);
	MULTILOAD(buf, attackCSE, 4, "attack_cse_%d");
	MULTILOAD(buf, sparkboom, 4, "sparkboom%d");
	LOAD(mushroom, "mushroom").dot();

	LOAD(hero, "hero").dot().setOffset(30.0, 50.0);

	LOAD(tileLight, "tile_light").dot();
	LOAD(tileDark, "tile_dark").dot();
	LOAD(flagNull, "flag_null").dot();
	MULTILOAD(buf, flagP, 4, "flag_p_%d");
	MULTILOAD(buf, flagK, 4, "flag_k_%d");

	MULTILOAD(buf, number, 10, "num_%d");
	MULTILOAD(buf, bignumber, 10, "bignum%d");

	LOAD(bg, "bg").dot();
	LOAD(intengrad, "intengrad").dot();

	LOAD(pointNorm, "point").dot();
	LOAD(pointP, "point_p").dot();
	LOAD(pointK, "point_k").dot();
	LOAD(pointBarP, "point_bar_p").dot().setOffset(256.0, 16.0);
	LOAD(pointBarK, "point_bar_k").dot().setOffset(0.0, 16.0);
	LOAD(pointBarFrame, "point_bar_frame").dot();

	MULTILOAD(buf, own, 8, "own_%d");
	MULTILOAD(buf, ownFlag, 8, "own_flag_%d");

	LOAD(winPostech, "win_postech").dot();
	LOAD(winKaist, "win_kaist").dot();
	LOAD(winDraw, "win_draw").dot();
}