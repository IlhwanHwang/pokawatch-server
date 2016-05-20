//
// CNH 2016.04.30
// shader management
//

#pragma once

#include <GL/glew.h>
#include <stack>
#include "mat.h"

class Shader {
private:
	static GLuint shdDefault;
	static GLuint defBlend;
	static GLuint defModelView;
	static GLuint defProjection;
	static GLuint vboCanonical;
	static GLuint vbo4thPlane;
	static mat4 modelViewCurrent;
	static std::stack<mat4> modelViewStack;

public:
	static void init();
	static void drawCanonical();
	static void draw4thPlane();
	static void useDefault() { glUseProgram(shdDefault); }
	static void setBlend(vec4& blend) { useDefault(); glUniform4fv(defBlend, 1, blend); }

	static void translate(vec3 pos) { modelViewCurrent *= Matrix::Translate(pos); }
	static void rotateX(float a) { modelViewCurrent *= Matrix::RotateX(a); }
	static void rotateY(float a) { modelViewCurrent *= Matrix::RotateY(a); }
	static void rotateZ(float a) { modelViewCurrent *= Matrix::RotateZ(a); }
	static void scale(vec3 sv) { modelViewCurrent *= Matrix::Scale(sv); }

	static void pop() {
		modelViewCurrent = modelViewStack.top();
		modelViewStack.pop();
	}
	static void push() { modelViewStack.push(modelViewCurrent); }
	static void apply() {
		glUseProgram(shdDefault);
		glUniformMatrix4fv(defModelView, 1, GL_TRUE, modelViewCurrent);
	}
	static void projection(mat4 p) {
		glUseProgram(shdDefault);
		glUniformMatrix4fv(defProjection, 1, GL_TRUE, p);
	}
};