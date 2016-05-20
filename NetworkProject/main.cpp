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



SOCKET hServSock;
SOCKET hClntSock[6];
SOCKADDR_IN servAddr;
SOCKADDR_IN clntAddr[6];
int szClntAddr[6];
char messageToClient[];


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
	

	glutMainLoop();
}