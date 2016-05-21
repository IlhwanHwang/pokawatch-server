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

Unit unitArray[UNIT_NUM_MAX] = {Unit(TEAM_POSTECH), Unit(TEAM_POSTECH), Unit(TEAM_POSTECH), Unit(TEAM_KAIST), Unit(TEAM_KAIST), Unit(TEAM_KAIST)};
Flag flagArray[FLAG_NUM_MAX] = {Flag(FLAG1_X, FLAG1_Y), Flag(FLAG2_X, FLAG2_Y) , Flag(FLAG3_X, FLAG3_Y) , Flag(FLAG4_X, FLAG4_Y) , Flag(FLAG5_X, FLAG5_Y) };
//Poison poisonArray[POISON_NUM_MAX];
//Petal petalArray[PETAL_NUM_MAX];
//Mushroom mushroomArray[MUSHROOM_NUM_MAX];

int score[2];
int turnleft;


void main(int argc, char **argv)
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(512, 512);

	glutCreateWindow("CNH - Network Project");

	glewInit();
	Shader::init();
	//60fps, 0.5 second per turn
	Timer::init(16, 30);
	Key::init();
	Network::init();

	glutMainLoop();
}