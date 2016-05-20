//
// CNH 2016.04.30
// CSED353 Project
//

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <cstdlib>
#include <ctime>

#include "shader.h"
#include "timer.h"
#include "key.h"

void main(int argc, char **argv)
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(512, 512);

	glutCreateWindow("CNH - Network Project");

	glewInit();

	Key::init();
	Shader::init();
	//60fps, 0.5 second per turn
	Timer::init(16, 30);

	glutMainLoop();
}