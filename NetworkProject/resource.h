//
// CNH 2016.04.30
// resource management
//

#pragma once

#include <GL/glut.h>
#include "vec.h"
#include "model.h"
#include "texture.h"

class Resource {
public:
	class Tex {
	public:
		static void init();
	};

	static void init();
};