//
// CNH 2016.04.30
// shader management
//

#include <GL/glew.h>

#include "InitShader.h"
#include "shader.h"
#include "debug.h"

GLuint Shader::shdDefault;
GLuint Shader::defBlend;
GLuint Shader::defModelView;
GLuint Shader::defProjection;
GLuint Shader::vboCanonical;
GLuint Shader::vbo4thPlane;
mat4 Shader::modelViewCurrent;
std::stack<mat4> Shader::modelViewStack;

void Shader::init() {
	shdDefault = glCreateProgram();
	loadShader(shdDefault, GL_VERTEX_SHADER, "default_v.glsl");
	loadShader(shdDefault, GL_FRAGMENT_SHADER, "default_f.glsl");
	linkProgram(shdDefault);

	glUniform1i(glGetUniformLocation(shdDefault, "tex"), 0);
	defBlend = glGetUniformLocation(shdDefault, "blend");
	defModelView = glGetUniformLocation(shdDefault, "modelview");
	defProjection = glGetUniformLocation(shdDefault, "projection");

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

	mat4 m;
	modelViewStack.push(m);
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