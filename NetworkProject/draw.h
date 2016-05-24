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

struct Color {
	static Color white, black, gray;
	static Color merge(Color& c1, Color& c2, float x);
	float r, g, b;
	Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

class DrawRequest {
private:
	GLuint buf;
	float w, h, ofx, ofy;
	float x, y, sx, sy, a, alpha;
	Color c;
	float depth;

public:
	DrawRequest(const Sprite& spr, int ind, float depth, float x, float y, float sx, float sy, Color& c, float alpha);
	void draw() const;
	bool operator< (const DrawRequest& other) const { return depth > other.depth; }
};

class Draw {
private:
	static int w, h;
	static std::priority_queue<DrawRequest> req;

public:
	static void init();
	static void reshape(int, int);
	static void flush();
	static void drawISB(Sprite& spr, int ind, float x, float y, float sx, float sy, Color& c, float alpha) {
		DrawRequest(spr, ind, 0.0, x, y, sx, sy, c, alpha).draw();
	}
	static void drawI(Sprite& spr, int ind, float x, float y) {
		drawISB(spr, ind, x, y, 1.0, 1.0, Color::white, 1.0);
	}
	static void drawS(Sprite& spr, float x, float y, float sx, float sy) {
		drawISB(spr, 0, x, y, sx, sy, Color::white, 1.0);
	}
	static void drawB(Sprite& spr, float x, float y, Color& c, float alpha) {
		drawISB(spr, 0, x, y, 1.0, 1.0, c, alpha);
	}
	static void draw(Sprite& spr, float x, float y) {
		drawISB(spr, 0, x, y, 1.0, 1.0, Color::white, 1.0);
	}
	static void onmapISB(Sprite& spr, int ind, float x, float y, float sx, float sy, Color& c, float alpha) {
		drawISB(spr, ind, Gui::unitX(x), Gui::unitY(y), sx, sy, c, alpha);
	}
	static void onmapI(Sprite& spr, int ind, float x, float y) {
		onmapISB(spr, ind, x, y, 1.0, 1.0, Color::white, 1.0);
	}
	static void onmap(Sprite& spr, float x, float y) {
		onmapISB(spr, 0, x, y, 1.0, 1.0, Color::white, 1.0);
	}

	static void qdrawISB(Sprite& spr, int ind, float depth, float x, float y, float sx, float sy, Color& c, float alpha) {
		req.push(DrawRequest(spr, ind, depth, x, y, sx, sy, c, alpha));
	}
	static void qdrawI(Sprite& spr, int ind, float depth, float x, float y) {
		qdrawISB(spr, ind, depth, x, y, 1.0, 1.0, Color::white, 1.0);
	}
	static void qdraw(Sprite& spr, float depth, float x, float y) {
		qdrawISB(spr, 0, depth, x, y, 1.0, 1.0, Color::white, 1.0);
	}
	static void qonmapISB(Sprite& spr, int ind, float depth, float x, float y, float sx, float sy, Color& c, float alpha) {
		qdrawISB(spr, ind, depth - y, Gui::unitX(x), Gui::unitY(y), sx, sy, c, alpha);
	}
	static void qonmapI(Sprite& spr, int ind, float depth, float x, float y) {
		qonmapISB(spr, ind, depth, x, y, 1.0, 1.0, Color::white, 1.0);
	}
	static void qonmapB(Sprite& spr, float depth, float x, float y, Color& c, float alpha) {
		qonmapISB(spr, 0, depth, x, y, 1.0, 1.0, c, alpha);
	}
	static void qonmap(Sprite& spr, float depth, float x, float y) {
		qonmapISB(spr, 0, depth, x, y, 1.0, 1.0, Color::white, 1.0);
	}

	static void number(int num, float x, float y);
};