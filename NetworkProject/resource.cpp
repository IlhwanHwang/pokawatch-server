//
// CNH 2016.05.22
// resources
//

#include "resource.h"

Sprite Rspr::temp;
Sprite Rspr::tileLight;
Sprite Rspr::tileDark;

void Resource::init() {
	Rspr::temp.load("cse_temp.png").setOffset(20.0, 80.0);
	Rspr::tileLight.load("tile_light.png");
	Rspr::tileDark.load("tile_dark.png");
}