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
//#include "server.h"
#include "key.h"
#include "protocol.h"
#include "unit.h"
#include "network.h"
#include "game.h"
#include "draw.h"
#include "resource.h"
#include "gui.h"

void main(int argc, char **argv)
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(INFO_WINDOW_WIDTH, INFO_WINDOW_HEIGHT);

	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("CNH - Network Project");

	glewExperimental = GL_TRUE;
	glewInit();

	Draw::init();
	Key::init();
	Game::init();
	Shader::init();
	Network::init(NULL);
	Resource::init();

	Timer::init(FRAME_INTERVAL, FRAME_PER_TURN);

	glutMainLoop();
}