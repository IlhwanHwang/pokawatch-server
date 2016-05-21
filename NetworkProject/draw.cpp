//
// CNH 2016.05.14
// draw module
//

#include "draw.h"
#include "shader.h"

Color Color::white(1.0, 1.0, 1.0);

int Draw::w = WINDOW_WIDTH;
int Draw::h = WINDOW_HEIGHT;
std::priority_queue<DrawRequest> Draw::req;

void Draw::init() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glutReshapeFunc(reshape);
}

DrawRequest::DrawRequest(const Sprite& spr, float depth, float x, float y, float sx, float sy, float a, Color& c, float alpha) :
	depth(depth), x(x), y(y), sx(sx), sy(sy), a(a), c(c), alpha(alpha) {
	buf = spr.getBuf();
	w = spr.getW();
	h = spr.getH();
	ofx = spr.getOfx();
	ofy = spr.getOfy();
}

void DrawRequest::draw() {
	Shader::push();
		/*
		Shader::translate(vec3(x, y, 0.0));
		Shader::rotateZ(a);
		Shader::scale(vec3(sx, sy, 1.0));
		Shader::translate(vec3(-ofx, -ofy, 0.0));
		Shader::scale(vec3(w, h, 1.0));
		Shader::apply();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE, buf);
		Shader::setBlend(vec4(c.r, c.g, c.b, alpha));
		*/
		Shader::draw4thPlane();
	Shader::pop();
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