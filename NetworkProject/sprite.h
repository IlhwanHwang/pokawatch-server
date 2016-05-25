//
// CNH 2016.05.20
// sprite class
//

#pragma once

#include <GL/glew.h>

// One png file corrensponds one Sprite class instance.
class Sprite {
private:
	float ofx, ofy;
	float w, h;
	unsigned uw, uh;
	GLuint buf;

public:
	// Sprite initializers are returning a self-reference
	// It is for code readibility, see 'resource.cpp' for example
	Sprite& load(const char* fn); // Load external png file
	Sprite& setOffset(float x, float y); // Set offset of sprite
	Sprite& dot(); // Specify dot style image, it simply magnifies image by 2.
	GLuint getBuf(int i) const { return buf; } // Get sprite's texture buffer location
	float getW() const { return w; }
	float getH() const { return h; }
	float getOfx() const { return ofx; }
	float getOfy() const { return ofy; }
};