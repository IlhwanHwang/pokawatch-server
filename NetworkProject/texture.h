//
// CNH 2016.04.30
// texture management unit
//

#pragma once

#include <GL/glut.h>

class Texture {
private:
	unsigned w, h;
	GLuint buf;

public:
	void load(const char* fn);
	void bind();
	unsigned getWidth() { return w; }
	unsigned getHeight() { return h; }
};