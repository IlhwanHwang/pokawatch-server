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
#include "draw.h"
#include "resource.h"

void main(int argc, char **argv)
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("CNH - Network Project");

	glewExperimental = GL_TRUE;
	glewInit();

	Resource::init();
	Key::init();
	Shader::init();
	Draw::init();
	Timer::init(16, 60);

	glutMainLoop();
}