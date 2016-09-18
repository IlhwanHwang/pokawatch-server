//
// CNH 2016.05.14
// draw module
//

#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <queue>
#include <iostream>

#include "protocol.h"
#include "sprite.h"
#include "gui.h"

// Color information container.
struct Color {
	static Color white, black, lightgray, gray, cyan, magenta, postech, kaist;
	static Color merge(Color& c1, Color& c2, float x);
	float r, g, b;
	Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

// Draw request class. Each class instance corresponds one draw call
// Used for depth sort
class DrawRequest {
private:
	GLuint buf;
	float w, h, ofx, ofy;
	float x, y, sx, sy, a, alpha;
	Color c;
	float depth;

public:
	DrawRequest(const Sprite& spr, float depth, float x, float y, float sx, float sy, Color& c, float alpha);
	void draw() const;
	bool operator< (const DrawRequest& other) const { return depth > other.depth; } // Overriding for priority_queue
};

class Draw {
private:
	static int w, h;
	static std::priority_queue<DrawRequest> req;

public:
	static void init();
	static void flush(); // Flush all draw requests. In depth-sorted manner.
	static void refresh();
	static void setsize(int _w, int _h);
	// Simple drawing function. One can handle scale and blend.
	static void drawSB(Sprite& spr, float x, float y, float sx, float sy, Color& c, float alpha) {
		DrawRequest(spr, 0.0, x, y, sx, sy, c, alpha).draw();
	}
	// And it derivatives.
	static void drawS(Sprite& spr, float x, float y, float sx, float sy) {
		drawSB(spr, x, y, sx, sy, Color::white, 1.0);
	}
	static void drawB(Sprite& spr, float x, float y, Color& c, float alpha) {
		drawSB(spr, x, y, 1.0, 1.0, c, alpha);
	}
	static void draw(Sprite& spr, float x, float y) {
		drawSB(spr, x, y, 1.0, 1.0, Color::white, 1.0);
	}
	// Draw sprite on the map coordinate
	static void onmapSB(Sprite& spr, float x, float y, float z, float sx, float sy, Color& c, float alpha) {
		drawSB(spr, Gui::unitX(x), Gui::unitY(y + z), sx, sy, c, alpha);
	}
	static void onmapB(Sprite& spr, float x, float y, float z, Color& c, float alpha) {
		drawSB(spr, Gui::unitX(x), Gui::unitY(y + z), 1.0, 1.0, c, alpha);
	}
	static void onmap(Sprite& spr, float x, float y, float z) {
		onmapSB(spr, x, y, z, 1.0, 1.0, Color::white, 1.0);
	}

	// Drawing with depth sort. Drawing request is pushed into queue
	static void qdrawSB(Sprite& spr, float depth, float x, float y, float sx, float sy, Color& c, float alpha) {
		req.push(DrawRequest(spr, depth, x, y, sx, sy, c, alpha));
	}
	static void qdrawB(Sprite& spr, float depth, float x, float y, Color& c, float alpha) {
		qdrawSB(spr, depth, x, y, 1.0, 1.0, c, alpha);
	}
	static void qdraw(Sprite& spr, float depth, float x, float y) {
		qdrawSB(spr, depth, x, y, 1.0, 1.0, Color::white, 1.0);
	}
	static void qonmapSB(Sprite& spr, float depth, float x, float y, float z, float sx, float sy, Color& c, float alpha) {
		qdrawSB(spr, depth - y, Gui::unitX(x), Gui::unitY(y + z), sx, sy, c, alpha);
	}
	static void qonmapB(Sprite& spr, float depth, float x, float y, float z, Color& c, float alpha) {
		qonmapSB(spr, depth, x, y, z, 1.0, 1.0, c, alpha);
	}
	static void qonmapS(Sprite& spr, float depth, float x, float y, float z, float sx, float sy) {
		qdrawSB(spr, depth - y, Gui::unitX(x), Gui::unitY(y + z), sx, sy, Color::white, 1.0);
	}
	static void qonmap(Sprite& spr, float depth, float x, float y, float z) {
		qonmapSB(spr, depth, x, y, z, 1.0, 1.0, Color::white, 1.0);
	}

	// Very simple image fetch
	static void naivefill(Sprite& spr);

	// Draw digits
	static void number(int num, float x, float y);
};