//
// CNH 2016.05.14
// draw module
//

#include "draw.h"
#include "shader.h"
#include "resource.h"
#include "debug.h"

Color Color::white(1.0, 1.0, 1.0);
Color Color::black(0.0, 0.0, 0.0);
Color Color::gray(0.5, 0.5, 0.5);
Color Color::lightgray(0.75, 0.75, 0.75);
Color Color::cyan(0.3, 1.0, 1.0);
Color Color::magenta(1.0, 0.7, 1.0);
Color Color::postech(1.0, 0.0, 0.2);
Color Color::kaist(0.0, 0.5, 1.0);

Color Color::merge(Color& c1, Color& c2, float x) {
	return Color(
		c1.r * (1.0 - x) + c2.r * x,
		c1.g * (1.0 - x) + c2.g * x,
		c1.b * (1.0 - x) + c2.b * x);
}

int Draw::w = INFO_WINDOW_WIDTH;
int Draw::h = INFO_WINDOW_HEIGHT;
std::priority_queue<DrawRequest> Draw::req;

void Draw::init() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, w, h);
}

DrawRequest::DrawRequest(const Sprite& spr, float depth, float x, float y, float sx, float sy, Color& c, float alpha) :
	depth(depth), x(x), y(y), c(c), alpha(alpha) {
	buf = spr.getBuf(0);
	w = spr.getW() * sx;
	h = spr.getH() * sy;
	ofx = spr.getOfx() * sx;
	ofy = spr.getOfy() * sy;
}

void DrawRequest::draw() const {
	Shader::draw(buf, 0, x - ofx, y - ofy, w, h, c.r, c.g, c.b, alpha);
}

void Draw::refresh() {
	if (w != glutGet(GLUT_WINDOW_WIDTH) || h != glutGet(GLUT_WINDOW_HEIGHT))
		glutReshapeWindow(w, h);
	//w = glutGet(GLUT_WINDOW_WIDTH);
	//h = glutGet(GLUT_WINDOW_HEIGHT);
	glViewport(0, 0, w, h);
}

void Draw::setsize(int _w, int _h) { 
	w = _w;
	h = _h;
	glutReshapeWindow(w, h);
	glutPostRedisplay();
}

void Draw::flush() {
	while (!req.empty()) {
		(const_cast<DrawRequest&>(req.top())).draw();
		req.pop();
	}
}

void Draw::naivefill(Sprite& spr) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	Shader::draw(spr.getBuf(0), 0, 0.0, 0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0, 1.0, 1.0, 1.0);
}

void Draw::number(int num, float x, float y) {
	const float ddx = 40.0;
	
	int numcopy = num;
	int ind = 0;
	
	while (numcopy > 0) {
		numcopy /= 10;
		ind++;
	}

	if (ind == 0)
		ind = 1;

	float dx = (ind - 1) / 2.0 * ddx;
	numcopy = num;

	for (int i = 0; i < ind; i++) {
		int digit = numcopy % 10;
		numcopy /= 10;
		draw(Rspr::number[digit], x + dx, y);
		dx -= ddx;
	}
}

void Draw::bignumber(int num, float x, float y) {
	const float ddx = 60.0;

	int numcopy = num;
	int ind = 0;

	while (numcopy > 0) {
		numcopy /= 10;
		ind++;
	}

	if (ind == 0)
		ind = 1;

	float dx = (ind - 1) / 2.0 * ddx;
	numcopy = num;

	for (int i = 0; i < ind; i++) {
		int digit = numcopy % 10;
		numcopy /= 10;
		draw(Rspr::bignumber[digit], x + dx, y);
		dx -= ddx;
	}
}