//
// CNH 2016.05.14
// draw module
//

#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "protocol.h"

class Draw {
private:


public:
	static void init();
	static void update();
	static void input(protocol_data *data);
};