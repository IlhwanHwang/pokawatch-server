//
// CNH 2016.04.30
// shader management
//

#include <GL/glew.h>

#include "InitShader.h"
#include "shader.h"
#include "debug.h"
#include "protocol.h"

GLuint Shader::shdDefault;
GLuint Shader::defBlend;
GLuint Shader::defPosx;
GLuint Shader::defPosy;
GLuint Shader::defWidth;
GLuint Shader::defHeight;
GLuint Shader::defFlip;
GLuint Shader::vboCanonical;
GLuint Shader::vbo4thPlane;

void Shader::init() {
	shdDefault = glCreateProgram();
	loadShader(shdDefault, GL_VERTEX_SHADER, "shader\\sprite_v.glsl");
	loadShader(shdDefault, GL_FRAGMENT_SHADER, "shader\\sprite_f.glsl");
	linkProgram(shdDefault);

	glUniform1i(glGetUniformLocation(shdDefault, "tex"), 0);
	defBlend = glGetUniformLocation(shdDefault, "blend");
	defPosx = glGetUniformLocation(shdDefault, "posx");
	defPosy = glGetUniformLocation(shdDefault, "posy");
	defWidth = glGetUniformLocation(shdDefault, "width");
	defHeight = glGetUniformLocation(shdDefault, "height");
	defFlip = glGetUniformLocation(shdDefault, "flip");

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float data[20] = {
		-1.0, -1.0, 0.0, 0.0, 1.0,
		1.0, -1.0, 0.0, 1.0, 1.0,
		-1.0, 1.0, 0.0, 0.0, 0.0,
		1.0, 1.0, 0.0, 1.0, 0.0
	};
	glGenBuffers(1, &vboCanonical);
	glBindBuffer(GL_ARRAY_BUFFER, vboCanonical);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, (void*)data, GL_STATIC_DRAW);

	float data2[20] = {
		0.0, 0.0, 0.0, 0.0, 1.0,
		1.0, 0.0, 0.0, 1.0, 1.0,
		0.0, 1.0, 0.0, 0.0, 0.0,
		1.0, 1.0, 0.0, 1.0, 0.0
	};
	glGenBuffers(1, &vbo4thPlane);
	glBindBuffer(GL_ARRAY_BUFFER, vbo4thPlane);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, (void*)data2, GL_STATIC_DRAW);
}

void Shader::drawCanonical() {
	glBindBuffer(GL_ARRAY_BUFFER, vboCanonical);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Shader::draw4thPlane() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo4thPlane);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Shader::draw
(GLuint buf, int flip, float x, float y, float w, float h, float r, float g, float b, float a) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buf);

	x /= WINDOW_WIDTH * 0.5;
	y /= WINDOW_HEIGHT * 0.5;
	w /= WINDOW_WIDTH * 0.5;
	h /= WINDOW_HEIGHT * 0.5;

	x -= 1.0;
	y -= 1.0;

	useDefault();
	glUniform1f(defPosx, x);
	glUniform1f(defPosy, y);
	glUniform1f(defWidth, w);
	glUniform1f(defHeight, h);
	glUniform1i(defFlip, flip);
	glUniform4f(defBlend, r, g, b, a);

	glBindBuffer(GL_ARRAY_BUFFER, vbo4thPlane);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}