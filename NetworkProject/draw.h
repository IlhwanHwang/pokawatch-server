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
	static Color white;
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
	DrawRequest(const Sprite& spr, int ind, float depth, float x, float y, Color& c, float alpha);
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
	static void draw(Sprite& spr, int ind, float x, float y, Color& c, float alpha) {
		DrawRequest(spr, ind, 0.0, x, y, c, alpha).draw();
	}
	static void draw(Sprite& spr, int ind, float x, float y) {
		draw(spr, ind, x, y, Color::white, 1.0);
	}
	static void draw(Sprite& spr, float x, float y) {
		draw(spr, 0, x, y, Color::white, 1.0);
	}
	static void onmap(Sprite& spr, int ind, float x, float y, Color& c, float alpha) {
		draw(spr, ind, Gui::unitX(x), Gui::unitY(y), c, alpha);
	}
	static void onmap(Sprite& spr, int ind, float x, float y) {
		onmap(spr, ind, x, y, Color::white, 1.0);
	}
	static void onmap(Sprite& spr, float x, float y) {
		onmap(spr, 0, x, y, Color::white, 1.0);
	}

	static void qdraw(Sprite& spr, int ind, float depth, float x, float y, Color& c, float alpha) {
		req.push(DrawRequest(spr, ind, depth, x, y, c, alpha));
	}
	static void qdraw(Sprite& spr, int ind, float depth, float x, float y) {
		qdraw(spr, ind, depth, x, y, Color::white, 1.0);
	}
	static void qdraw(Sprite& spr, float depth, float x, float y) {
		qdraw(spr, 0, depth, x, y, Color::white, 1.0);
	}
	static void qonmap(Sprite& spr, int ind, float depth, float x, float y, Color& c, float alpha) {
		qdraw(spr, ind, depth - y, Gui::unitX(x), Gui::unitY(y), c, alpha);
	}
	static void qonmap(Sprite& spr, int ind, float depth, float x, float y) {
		qonmap(spr, ind, depth, x, y, Color::white, 1.0);
	}
	static void qonmap(Sprite& spr, float depth, float x, float y) {
		qonmap(spr, 0, depth, x, y, Color::white, 1.0);
	}
};