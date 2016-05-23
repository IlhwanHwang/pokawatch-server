//
// CNH 2016.05.22
// resources
//

#include "resource.h"

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

Sprite Rspr::petal;
Sprite Rspr::poison;

Sprite Rspr::tileLight;
Sprite Rspr::tileDark;
Sprite Rspr::flagNull;
Sprite Rspr::flagFlag;

void Resource::init() {
	Rspr::temp.load("cse_temp.png").setOffset(20.0, 80.0);
	Rspr::error.load("error.png");

	Rspr::unitCSE.load("cse_temp.png").setOffset(20.0, 80.0);
	Rspr::unitCHEM.load("chem_temp.png").setOffset(20.0, 80.0);
	Rspr::unitME.load("me_temp.png").setOffset(20.0, 80.0);
	Rspr::unitLIFE.load("life_temp.png").setOffset(20.0, 80.0);
	Rspr::unitPHYS.load("phys_temp.png").setOffset(20.0, 80.0);
	Rspr::unitHeart.load("heart.png");

	Rspr::faceCSE.load("face_cse.png");
	Rspr::faceCHEM.load("face_chem.png");
	Rspr::faceME.load("face_me.png");
	Rspr::faceLIFE.load("face_life.png");
	Rspr::facePHYS.load("face_phys.png");
	Rspr::faceDEAD.load("face_dead.png");

	Rspr::petal.load("petal.png").setOffset(20.0, 40.0);
	Rspr::poison.load("poison.png");

	Rspr::tileLight.load("tile_light.png");
	Rspr::tileDark.load("tile_dark.png");
	Rspr::flagNull.load("flag_null.png");
	Rspr::flagFlag.load("flag_flag.png");
}