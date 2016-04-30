//
// CNH 2016.04.30
// game manage module
//

#pragma once

#include <GL/glew.h>

class Game {
private:
	static int frameInterval;

public:
	static void init();
	static void setup();
	static void clear();
	static void draw();
	static void update();
	static void reshape(int w, int h);

	static int getFrameInterval();
};