//
// CNH 2016.04.30
// shader management
//

#pragma once

#include <GL/glew.h>

class Shader {
private:
	static GLuint shdDefault;
	static GLuint defBlend;
	static GLuint defPosx;
	static GLuint defPosy;
	static GLuint defWidth;
	static GLuint defHeight;
	static GLuint defFlip;
	static GLuint vboCanonical;
	static GLuint vbo4thPlane;

public:
	static void init();
	static void drawCanonical();
	static void draw4thPlane();
	static void useDefault() { glUseProgram(shdDefault); }
	static void draw(GLuint buf, int flip, float x, float y, float w, float h, float r, float g, float b, float a);
};