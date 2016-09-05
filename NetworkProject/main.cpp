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
//#include "server.h"
#include "key.h"
#include "protocol.h"
#include "unit.h"
#include "network.h"
#include "game.h"
#include "draw.h"
#include "resource.h"
#include "gui.h"
#include "audio.h"

#include <irrKlang.h>

void main(int argc, char **argv)
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGHT);

	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("2016 PKWar AI <Pokawatch>");

	glewExperimental = GL_TRUE;
	glewInit();

	Draw::init();
	Key::init();
	Game::init();
	Shader::init();
	if (argc == 1)
		Network::init("2222");
	else
		Network::init(argv[1]);
	Resource::init();
	Audio::init();

	Timer::init(1, 30);

	glutMainLoop();
}