//
// CNH 2016.05.14
// draw module
//

#include "draw.h"
#include "shader.h"

Color Color::white(1.0, 1.0, 1.0);
Color Color::black(0.0, 0.0, 0.0);

Color Color::merge(Color& c1, Color& c2, float x) {
	return Color(
		c1.r * (1.0 - x) + c2.r * x,
		c1.g * (1.0 - x) + c2.g * x,
		c1.b * (1.0 - x) + c2.b * x);
}

int Draw::w = WINDOW_WIDTH;
int Draw::h = WINDOW_HEIGHT;
std::priority_queue<DrawRequest> Draw::req;

void Draw::init() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_ALPHA_TEST);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glutReshapeFunc(reshape);
}

DrawRequest::DrawRequest(const Sprite& spr, int ind, float depth, float x, float y, Color& c, float alpha) :
	depth(depth), x(x), y(y), c(c), alpha(alpha) {
	buf = spr.getBuf(ind);
	w = spr.getW();
	h = spr.getH();
	ofx = spr.getOfx();
	ofy = spr.getOfy();
}

void DrawRequest::draw() const {
	Shader::draw(buf, 0, x - ofx, y - ofy, w, h, c.r, c.g, c.b, alpha);
}

void Draw::reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void Draw::flush() {
	while (!req.empty()) {
		(const_cast<DrawRequest&>(req.top())).draw();
		req.pop();
	}
}