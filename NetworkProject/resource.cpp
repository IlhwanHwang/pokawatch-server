//
// CNH 2016.05.22
// resources
//

#include "resource.h"
#include <string>

Sprite Rspr::temp;
Sprite Rspr::error;

Sprite Rspr::unitCSE;
Sprite Rspr::unitCHEM;
Sprite Rspr::unitME;
Sprite Rspr::unitLIFE;
Sprite Rspr::unitPHYS;
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
Sprite Rspr::spark[4];
Sprite Rspr::sparkboom[4];
Sprite Rspr::mushroom;

Sprite Rspr::hero;
Sprite Rspr::stun[4];

Sprite Rspr::tileLight;
Sprite Rspr::tileDark;
Sprite Rspr::flagNull;
Sprite Rspr::flagPostech;
Sprite Rspr::flagKaist;

Sprite Rspr::number[10];
Sprite Rspr::bignumber[10];

Sprite Rspr::bg;
Sprite Rspr::intengrad;

Sprite Rspr::winPostech;
Sprite Rspr::winKaist;
Sprite Rspr::winDraw;

Sprite Rspr::infoMain;
Sprite Rspr::infoServer;
Sprite Rspr::infoClient1;
Sprite Rspr::infoClient2;
Sprite Rspr::infoClient3;

#define RESOURCE_LOCATION "images\\"

#define MULTILOAD(buf, res, num, filename) \
	for (int i = 0; i < num; i++) \
	{ \
		sprintf(buf, RESOURCE_LOCATION filename ".png", i); \
		Rspr::res[i].load(buf).dot(); \
	}

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

	LOAD(unitCSE, "body_cse").dot().setOffset(20.0, 74.0);
	LOAD(unitCHEM, "body_chem").dot().setOffset(20.0, 74.0);
	LOAD(unitME, "body_me").dot().setOffset(30.0, 70.0);
	LOAD(unitLIFE, "body_life").dot().setOffset(20.0, 74.0);
	LOAD(unitPHYS, "body_phys").dot().setOffset(20.0, 74.0);
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
	MULTILOAD(buf, stun, 4, "stun%d");
	MULTILOAD(buf, spark, 4, "spark%d");
	MULTILOAD(buf, sparkboom, 4, "sparkboom%d");
	LOAD(mushroom, "mushroom").dot();

	LOAD(hero, "hero").dot().setOffset(30.0, 50.0);

	LOAD(tileLight, "tile_light").dot();
	LOAD(tileDark, "tile_dark").dot();
	LOAD(flagNull, "flag_null").dot();
	LOAD(flagPostech, "flag_postech").dot();
	LOAD(flagKaist, "flag_kaist").dot();

	MULTILOAD(buf, number, 10, "num_%d");
	MULTILOAD(buf, bignumber, 10, "bignum%d");

	LOAD(bg, "bg").dot();
	LOAD(intengrad, "intengrad").dot();

	LOAD(winPostech, "win_postech").dot();
	LOAD(winKaist, "win_kaist").dot();
	LOAD(winDraw, "win_draw").dot();
}