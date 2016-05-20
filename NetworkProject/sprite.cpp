//
// CNH 2016.05.20
// sprite class
//

#include "shader.h"
#include "sprite.h"

Color Color::white(1.0, 1.0, 1.0);

void Sprite::load(const char* fn) {
	t.load(fn);
	w = (float)t.getWidth();
	h = (float)t.getHeight();
	ofx = w * 0.5;
	ofy = h * 0.5;
}

void Sprite::setOffset(float ofx, float ofy) {
	this->ofx = ofx;
	this->ofy = ofy;
}

void Sprite::drawGeneral(float x, float y, float sx, float sy, float a, Color& c, float alpha) {
	Shader::push();
		Shader::translate(vec3(x, y, 0.0));
		Shader::rotateZ(a);
		Shader::scale(vec3(sx, sy, 1.0));
		Shader::translate(vec3(-ofx, -ofy, 0.0));
		Shader::scale(vec3(w, h, 1.0));
		Shader::apply();
		glActiveTexture(GL_TEXTURE0);
		t.bind();
		Shader::setBlend(vec4(c.r, c.g, c.b, alpha));
		Shader::draw4thPlane();
	Shader::pop();
}