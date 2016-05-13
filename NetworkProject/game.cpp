//
// BetterCarryingEachOther 2016.03.11
// game manage module
//

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <cstdlib>
#include <ctime>

#include "game.h"
#include "shader.h"
#include "key.h"

using namespace std;

int Game::frameInterval = 16;

void Game::init() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	srand(time(NULL));
	setup();
}

void Game::setup() {
}

void Game::clear() {
}

void Game::reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void Game::draw() {
	glClearDepth(1.0);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader::useDefault();
	glActiveTexture(GL_TEXTURE0);
	
	glutSwapBuffers();
}

void Game::update() {
	Key::keyUpdate();
}

int Game::getFrameInterval() {
	return frameInterval;
}