//
// CNH 2016.04.30
// CSED353 Project
//

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <winsock2.h>
#include <windows.h>

#include "shader.h"
#include "timer.h"
#include "server.h"
#include "key.h"
#include "protocol.h"
#include "unit.h"
#include "network.h"
#include "game.h"

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
	Shader::init();
	//60fps, 0.5 second per turn
	Key::init();
	Network::init();
	Game::init();

	Resource::init();
	Key::init();
	Shader::init();
	Draw::init();
	Timer::init(16, 60);

	glutMainLoop();
}