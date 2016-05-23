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

	static Sprite petal;
	static Sprite poison;

	static Sprite tileLight;
	static Sprite tileDark;

	static Sprite flagNull;
	static Sprite flagFlag;
};

class Resource {
public:
	static void init();
};