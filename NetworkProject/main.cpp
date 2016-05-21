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



SOCKET hServSock;			// of server
SOCKET hClntSock[6];		// of server
SOCKET hSocket;				// of client
SOCKADDR_IN servAddr;		// of server/client
SOCKADDR_IN clntAddr[6];	// of server
int szClntAddr[6];			// of server
char messageToClient[3];		// of server
char messageFromClient[6][16]; // of server
char messageToServer[];		// of client
int mode;					//determine server(1) client(2) or nothing (0)
int characterSelection;		//character_selection_time;
char gameStart[2];				// start(1)



void main(int argc, char **argv)
{
	srand(time(NULL));
	printf("Hi");

	mode = 0;
	characterSelection = 0;

	messageToClient[0] = 'h';
	messageToClient[1] = 'i';
	messageToClient[2] = '\0';

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

	/*if (argc == 2) mode = 1;
	else if (argc == 3) mode = 2;
	else
	{
		printf("Server Usage : %s <port> \n", argv[0]);
		printf("Client Usage : %s <ip> <port>\n", argv[0]);
		exit(1);
	}*/

	glutMainLoop();
}