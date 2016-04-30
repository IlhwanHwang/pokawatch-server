//
// CNH 2016.04.30
// CSED353 Project
//

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "key.h"
#include "game.h"
#include "shader.h"
#include "model.h"
#include "resource.h"

void update(int cnt) {
	glutTimerFunc(Game::getFrameInterval(), update, cnt + 1);

	Game::update();
	Game::draw();
	Key::keyUpdate();
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(512, 512);
	
	//glutInitContextVersion(3, 2);
	//glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("CNH - Network Project");

	//glewExperimental = GL_TRUE;
	glewInit();

	Model::init();
	Resource::init();
	Shader::init();

	Game::init();
	
	glutReshapeFunc(Game::reshape);
	glutDisplayFunc(Game::draw);
	
	glutTimerFunc(Game::getFrameInterval(), update, 0);

	glutKeyboardFunc(Key::keyPressed);
	glutKeyboardUpFunc(Key::keyReleased);

	glutMainLoop();
}