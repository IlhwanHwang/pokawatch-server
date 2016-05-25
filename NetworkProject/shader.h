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
	static GLuint vboCanonical; // rectangle (-1.0, -1.0, 1.0, 1.0)
	static GLuint vbo4thPlane; // rectangle (0.0, 0.0, 1.0, 1.0)

public:
	static void init();
	static void drawCanonical();
	static void draw4thPlane();
	static void useDefault() { glUseProgram(shdDefault); }
	static void draw(GLuint buf, int flip, float x, float y, float w, float h, float r, float g, float b, float a);
};