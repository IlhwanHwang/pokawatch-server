//
// CNH 2016.05.19
// network module
//

#pragma once

#include <WinSock2.h>
#include "protocol.h"
#include <string>
class Network {
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

	static void init()
	{
		mode = MODE_NOTHING;
		characterSelection = 0;
		gameStart[0] = 'N';
		gameStart[1] = '\0';
	}
	static void ErrorHandling(char *message);
	static void communicate(int cnt);
	static void makeServerSocket();
	static void acceptClient();
	static void sendToClient(char messageToClient[]);
	static void recieveFromClient();
	static void closeServerConnection();
	
	static void makeClientSocket();
	static void connectToServer(){}
	static void getProtocolDataFromServer();
	static void closeClientConnection();
	static void sendToServer(char message[]);
	static void recieveGameStart();

	static int getMode() { return mode; }
	static int getCommand() { return command; }
	static int getCharacterSelection() { return characterSelection; }
	static char *getGameStart() { return gameStart; }

	static void setMode(int x) { mode = x; }
	static void setCommand(int x) { command = x; }
	static void setGameStart(int index, char x) { gameStart[index] = x; }
	static void setCharacterSelection(int x) { characterSelection = x; }

	static void update();
	static void turn();

	static protocol_command getCommand(int index) {return (protocol_command)atoi(messageFromClient[index]); }

};