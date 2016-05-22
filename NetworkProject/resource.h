//
// CNH 2016.05.22
// resources
//

#pragma once

#include "sprite.h"

class Rspr {
public:
	static Sprite temp;
	static Sprite tileLight;
	static Sprite tileDark;
	static Sprite flagNull;
	static Sprite flagFlag;
};

class Resource {
public:
	static void init();
};