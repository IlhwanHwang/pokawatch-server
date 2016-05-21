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


SOCKET hServSock;												// of server
SOCKET hClntSock[UNIT_NUM_MAX];									// of server
SOCKET hSocket;													// of client
SOCKADDR_IN servAddr;											// of server/client
SOCKADDR_IN clntAddr[UNIT_NUM_MAX];								// of server
int szClntAddr[UNIT_NUM_MAX];									// of server
char messageToClient[MESSAGE_T0_CLIENT_SIZE];					// of server
char messageFromClient[UNIT_NUM_MAX][MESSAGE_TO_SERVER_SIZE];	// of server
char messageToServer[MESSAGE_TO_SERVER_SIZE];					// of client
int mode;														//determine server/ client/ nothing
int characterSelection;											
char gameStart[2];												// not(N) start(G)

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

	mode = MODE_NOTHING;
	characterSelection = 0;
	gameStart[0] = 'N';
	gameStart[1] = '\0';

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(512, 512);

	glutCreateWindow("CNH - Network Project");

	glewInit();
	Shader::init();
	//60fps, 0.5 second per turn
	Timer::init(16, 30);
	Key::init();

	glutMainLoop();
}