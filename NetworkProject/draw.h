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
	DrawRequest(const Sprite& spr, int ind, float depth, float x, float y, float sx, float sy, float a, Color& c, float alpha);
	void draw();
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
	static void draw(Sprite& spr, int ind, float depth, float x, float y, float sx, float sy, float a, Color& c, float alpha) {
		//DrawRequest(spr, ind, depth, x, y, sx, sy, a, c, alpha).draw();
		req.push(DrawRequest(spr, ind, depth, x, y, sx, sy, a, c, alpha));
	}
	static void draw(Sprite& spr, float depth, float x, float y, float sx, float sy, float a, Color& c, float alpha) {
		draw(spr, 0, depth, x, y, sx, sy, a, c, alpha);
	}
	static void draw(Sprite& spr, float depth, float x, float y) {
		draw(spr, 0, depth, x, y, 1.0, 1.0, 0.0, Color::white, 1.0);
	}
	static void draw(Sprite& spr, float depth, float x, float y, float sx, float sy) {
		draw(spr, 0, depth, x, y, sx, sy, 0.0, Color::white, 1.0);
	}
	static void draw(Sprite& spr, float depth, float x, float y, float sx, float sy, float a) {
		draw(spr, 0, depth, x, y, sx, sy, a, Color::white, 1.0);
	}
	static void draw(Sprite& spr, float depth, float x, float y, float sx, float sy, float a, Color& c) {
		draw(spr, 0, depth, x, y, sx, sy, a, c, 1.0);
	}
	static void onmap(Sprite& spr, int ind, float depth, float x, float y, float sx, float sy, float a, Color& c, float alpha) {
		draw(spr, ind, depth - y, Gui::unitX(x), Gui::unitY(y), sx, sy, a, c, alpha);
	}
	static void onmap(Sprite& spr, float depth, float x, float y) {
		onmap(spr, 0, depth, x, y, 1.0, 1.0, 0.0, Color::white, 1.0);
	}
	static void onmap(Sprite& spr, int ind, float depth, float x, float y) {
		onmap(spr, ind, depth, x, y, 1.0, 1.0, 0.0, Color::white, 1.0);
	}
};