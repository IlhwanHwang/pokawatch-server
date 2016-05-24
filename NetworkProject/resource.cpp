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

#define MULTILOAD(buf, res, num, filename) \
	for (int i = 0; i < num; i++) \
	{ \
		sprintf(buf, filename, i); \
		Rspr::res[i].load(buf).dot(); \
	}

void Resource::init() {
	char buf[100];

	Rspr::temp.load("cse_temp.png").setOffset(20.0, 80.0).dot();
	Rspr::error.load("error.png");

	Rspr::unitCSE.load("body_cse.png").dot().setOffset(20.0, 74.0);
	Rspr::unitCHEM.load("body_chem.png").dot().setOffset(20.0, 74.0);
	Rspr::unitME.load("body_me.png").dot().setOffset(30.0, 70.0);
	Rspr::unitLIFE.load("body_life.png").dot().setOffset(20.0, 74.0);
	Rspr::unitPHYS.load("body_phys.png").dot().setOffset(20.0, 74.0);
	Rspr::unitHeart.load("heart.png").dot();

	Rspr::faceCSE.load("face_cse.png");
	Rspr::faceCHEM.load("face_chem.png");
	Rspr::faceME.load("face_me.png");
	Rspr::faceLIFE.load("face_life.png");
	Rspr::facePHYS.load("face_phys.png");
	Rspr::faceDEAD.load("face_dead.png");
	Rspr::faceFrame.load("face_frame.png");

	Rspr::petal.load("petal.png").dot();
	Rspr::poison.load("poison.png").dot();
	MULTILOAD(buf, beamH, 4, "beam_h%d.png");
	MULTILOAD(buf, beamV, 4, "beam_v%d.png");
	MULTILOAD(buf, stun, 4, "stun%d.png");
	MULTILOAD(buf, spark, 4, "spark%d.png");
	MULTILOAD(buf, sparkboom, 4, "sparkboom%d.png");
	Rspr::mushroom.load("mushroom.png").dot();

	Rspr::hero.load("hero.png").dot().setOffset(30.0, 50.0);

	Rspr::tileLight.load("tile_light.png").dot();
	Rspr::tileDark.load("tile_dark.png").dot();
	Rspr::flagNull.load("flag_null.png").dot();
	Rspr::flagPostech.load("flag_postech.png").dot();
	Rspr::flagKaist.load("flag_kaist.png").dot();

	MULTILOAD(buf, number, 10, "num_%d.png");
	MULTILOAD(buf, bignumber, 10, "bignum%d.png");

	Rspr::bg.load("bg.png").dot();
	Rspr::intengrad.load("intengrad.png").dot();

	Rspr::winPostech.load("win_postech.png").dot();
	Rspr::winKaist.load("win_kaist.png").dot();
	Rspr::winDraw.load("win_draw.png").dot();

	Rspr::infoMain.load("info_main.png").setOffset(0.0, 240.0);
	Rspr::infoServer.load("info_server.png").setOffset(0.0, 240.0);
	Rspr::infoClient1.load("info_client1.png").setOffset(0.0, 240.0);
	Rspr::infoClient2.load("info_client2.png").setOffset(0.0, 240.0);
}