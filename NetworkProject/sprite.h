//
// CNH 2016.05.20
// sprite class
//

#pragma once

#include "texture.h"

struct Color {
	static Color white;
	float r, g, b;
	Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

class Sprite {
private:
	Texture t;
	float ofx, ofy;
	float w, h;

public:
	void load(const char* fn);
	void setOffset(float x, float y);
	
	void drawGeneral(float x, float y, float sx, float sy, float a, Color& c, float alpha);
	void draw(float x, float y) { 
		drawGeneral(x, y, 1.0, 1.0, 0.0, Color::white, 1.0); 
	}
	void drawScaled(float x, float y, float sx, float sy) {
		drawGeneral(x, y, sx, sy, 0.0, Color::white, 1.0);
	}
	void drawRotated(float x, float y, float a) {
		drawGeneral(x, y, 1.0, 1.0, a, Color::white, 1.0);
	}
	void drawScaledRotated(float x, float y, float sx, float sy, float a) {
		drawGeneral(x, y, sx, sy, a, Color::white, 1.0);
	}
	void drawColored(float x, float y, Color& c) {
		drawGeneral(x, y, 1.0, 1.0, 0.0, c, 1.0);
	}
	void drawAlphaed(float x, float y, float alpha) {
		drawGeneral(x, y, 1.0, 1.0, 0.0, Color::white, alpha);
	}
	void drawBlended(float x, float y, Color& c, float alpha) {
		drawGeneral(x, y, 1.0, 1.0, 0.0, c, alpha);
	}
};