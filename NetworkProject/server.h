#pragma once
/*
#include <WinSock2.h>
#include "protocol.h"

class Server {
private:
	static SOCKET hServSock;												// of server
	static SOCKET hClntSock[UNIT_NUM_MAX];									// of server
	static SOCKET hSocket;													// of client
	static SOCKADDR_IN servAddr;											// of server/client
	static SOCKADDR_IN clntAddr[UNIT_NUM_MAX];								// of server
	static int szClntAddr[UNIT_NUM_MAX];									// of server
	static char messageToClient[MESSAGE_T0_CLIENT_SIZE];					// of server
	static char messageFromClient[UNIT_NUM_MAX][MESSAGE_TO_SERVER_SIZE];	// of server
	static char messageToServer[MESSAGE_TO_SERVER_SIZE];					// of client
	static int mode;														//determine server/ client/ nothing
	static int characterSelection;
	static char gameStart[2];												// not(N) start(G)
	static int command;

public:
	Server();
	static void ErrorHandling(char *message);
	static void communicate(int cnt);
	static void makeServerSocket();
	static void acceptClient();
	static void sendToClient(char messageToClient[]);
	static void recieveFromClient();
	static void closeServerConnection();
	static int getMode() { return mode; }
	static int getCommand() { return command; }
	static char *getGameStart() { return gameStart; }
};
*/