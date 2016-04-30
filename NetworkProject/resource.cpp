//
// CNH 2016.04.30
// resource management
//

#include "vec.h"
#include "resource.h"
#include "lodepng.h"

void Resource::Tex::init() {
	glActiveTexture(GL_TEXTURE0);
}

void Resource::init() {
	Tex::init();
}