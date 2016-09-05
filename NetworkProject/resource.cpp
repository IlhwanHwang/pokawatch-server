//
// CNH 2016.05.22
// resources
//

#include "resource.h"
#include <string>
#include "audio.h"

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
Sprite Rspr::unitHeartP;
Sprite Rspr::unitHeartK;

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
Sprite Rspr::PHYSBlackholeP[8];
Sprite Rspr::PHYSBlackholeK[8];
Sprite Rspr::CSEBlink;
Sprite Rspr::MEAccidentPH;
Sprite Rspr::MEAccidentPV;
Sprite Rspr::MEAccidentKH;
Sprite Rspr::MEAccidentKV;
Sprite Rspr::attackCSE[4];
Sprite Rspr::sparkboom[4];
Sprite Rspr::mushroom;

Sprite Rspr::deathP[8];
Sprite Rspr::deathK[8];

Sprite Rspr::hero;
Sprite Rspr::stun[4];

Sprite Rspr::tileLight;
Sprite Rspr::tileDark;
Sprite Rspr::flagNull;
Sprite Rspr::flagP[4];
Sprite Rspr::flagK[4];

Sprite Rspr::number[10];

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

Sprite Rspr::ownPoint;
Sprite Rspr::ownFlag[8];
Sprite Rspr::ownGaugeP[5];
Sprite Rspr::ownGaugeK[5];

Sprite Rspr::turnWaitPlay;
Sprite Rspr::turnWaitWait;

irrklang::ISoundSource* Rsfx::death;

#define RESOURCE_IMG_LOCATION "images\\"
#define RESOURCE_SFX_LOCATION "sounds\\"

// Macro to load multiple consecutive images at once
#define MULTILOAD(buf, res, num, filename) \
	for (int i = 0; i < num; i++) \
	{ \
		sprintf(buf, RESOURCE_IMG_LOCATION filename ".png", i); \
		Rspr::res[i].load(buf).dot(); \
	}

// Single image load macro
#define LOAD(res, filename) Rspr::res.load(RESOURCE_IMG_LOCATION filename ".png")

// Single SFX load macro
#define LOADSFX(res, filename) Rsfx::res = Audio::getEngine()->addSoundSourceFromFile(RESOURCE_SFX_LOCATION filename ".wav", irrklang::ESM_AUTO_DETECT, true)

void Resource::init() {
}

void Resource::initSfx() {
	LOADSFX(death, "death");
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
	LOAD(unitHeartP, "heart_p").dot();
	LOAD(unitHeartK, "heart_k").dot();

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
	MULTILOAD(buf, PHYSBlackholeP, 8, "blackhole_p_%d");
	MULTILOAD(buf, PHYSBlackholeK, 8, "blackhole_k_%d");
	MULTILOAD(buf, stun, 4, "stun%d");
	LOAD(CSEBlink, "blink_cse").dot().setOffset(20.0, 74.0);
	LOAD(MEAccidentPH, "me_accident_p_h").dot().setOffset(120.0, 40.0);
	LOAD(MEAccidentPV, "me_accident_p_v").dot().setOffset(20.0, 0.0);
	LOAD(MEAccidentKH, "me_accident_k_h").dot().setOffset(120.0, 40.0);
	LOAD(MEAccidentKV, "me_accident_k_v").dot().setOffset(20.0, 0.0);
	MULTILOAD(buf, attackCSE, 4, "attack_cse_%d");
	MULTILOAD(buf, sparkboom, 4, "sparkboom%d");
	LOAD(mushroom, "mushroom").dot();

	MULTILOAD(buf, deathP, 8, "death_p_%d");
	MULTILOAD(buf, deathK, 8, "death_k_%d");
	LOAD(hero, "hero").dot().setOffset(30.0, 50.0);

	LOAD(tileLight, "tile_light").dot();
	LOAD(tileDark, "tile_dark").dot();
	LOAD(flagNull, "flag_null").dot();
	MULTILOAD(buf, flagP, 4, "flag_p_%d");
	MULTILOAD(buf, flagK, 4, "flag_k_%d");

	MULTILOAD(buf, number, 10, "num_%d");

	LOAD(bg, "bg").dot();
	LOAD(intengrad, "intengrad").dot();

	LOAD(pointNorm, "point").dot();
	LOAD(pointP, "point_p").dot();
	LOAD(pointK, "point_k").dot();
	LOAD(pointBarP, "point_bar_p").dot().setOffset(256.0, 16.0);
	LOAD(pointBarK, "point_bar_k").dot().setOffset(0.0, 16.0);
	LOAD(pointBarFrame, "point_bar_frame").dot();

	LOAD(ownPoint, "own_point").dot();
	MULTILOAD(buf, ownFlag, 8, "own_flag_%d");
	MULTILOAD(buf, ownGaugeP, 5, "own_gauge_p_%d");
	MULTILOAD(buf, ownGaugeK, 5, "own_gauge_k_%d");

	LOAD(winPostech, "win_postech").dot();
	LOAD(winKaist, "win_kaist").dot();
	LOAD(winDraw, "win_draw").dot();

	LOAD(turnWaitPlay, "turnwait_play").dot();
	LOAD(turnWaitWait, "turnwait_wait").dot();
}