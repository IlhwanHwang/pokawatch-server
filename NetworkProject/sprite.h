//
// CNH 2016.05.20
// sprite class
//

#pragma once

#include <GL/glew.h>
#include <vector>

class Sprite {
private:
	float ofx, ofy;
	float w, h;
	unsigned uw, uh;
	GLuint buf;


public:
	Sprite& load(const char* fn, int num);
	Sprite& load(const char* fn);
	Sprite& setOffset(float x, float y);
	GLuint getBuf() const { return buf; }
	float getW() const { return w; }
	float getH() const { return h; }
	float getOfx() const { return ofx; }
	float getOfy() const { return ofy; }
};