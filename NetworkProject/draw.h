//
// CNH 2016.05.14
// draw module
//

#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <queue>

#include "protocol.h"
#include "sprite.h"

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
	DrawRequest(const Sprite& spr, float depth, float x, float y, float sx, float sy, float a, Color& c, float alpha);
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
	static void draw(Sprite& spr, float depth, float x, float y, float sx, float sy, float a, Color& c, float alpha) {
		req.push(DrawRequest(spr, depth, x, y, sx, sy, a, c, alpha));
	}
	static void draw(Sprite& spr, float depth, float x, float y) {
		req.push(DrawRequest(spr, depth, x, y, 1.0, 1.0, 0.0, Color::white, 1.0));
	}
	static void draw(Sprite& spr, float depth, float x, float y, float sx, float sy) {
		req.push(DrawRequest(spr, depth, x, y, sx, sy, 0.0, Color::white, 1.0));
	}
	static void draw(Sprite& spr, float depth, float x, float y, float sx, float sy, float a) {
		req.push(DrawRequest(spr, depth, x, y, sx, sy, a, Color::white, 1.0));
	}
	static void draw(Sprite& spr, float depth, float x, float y, float sx, float sy, float a, Color& c) {
		req.push(DrawRequest(spr, depth, x, y, sx, sy, a, c, 1.0));
	}
};