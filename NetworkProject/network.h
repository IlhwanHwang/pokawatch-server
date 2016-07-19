//
// CNH 2016.05.19
// network module
//

#pragma once

#include <WinSock2.h>
#include "protocol.h"
#include <string>
class Network {																// EVERY THING RELATED TO NETWORK OPERATION IS IN HERE
private:
	static SOCKET hServSock;												// Server socket variable of server/client side
	static SOCKET hClntSock[UNIT_NUM_MAX];									// Client socket variable of server side
	static SOCKET hSocket;													// Client socket variable of client side
	static SOCKADDR_IN servAddr;											// Server address variable of server/client side
	static SOCKADDR_IN clntAddr[UNIT_NUM_MAX];								// Client address variable of server side
	static int szClntAddr[UNIT_NUM_MAX];									// Client address size variable of server side
	static char messageToClient[MESSAGE_T0_CLIENT_SIZE];					// Message buffer of server side
	static char messageFromClient[UNIT_NUM_MAX][MESSAGE_TO_SERVER_SIZE];	// Message buffer of server side
	static char messageToServer[MESSAGE_TO_SERVER_SIZE];					// Message buffer of client side
	static int mode;														// determine server/ client/ nothing
	static int characterSelection;											// Information of selection of charactor(dep)
	static char gameStart[3];												// game started? [0] : not(N) start(G), [1] : team_information
	static int command[(UNIT_NUM_MAX)/2];														// selected command of client side
	static char *serverIpArg;
	static int team;														// 0 : TEAM_NULL, 1: TEAM_POSTECH, 2: TEAM_KAIST

public:

	static void init(char * argv)								// initiate network state
	{
		mode = MODE_NOTHING;
		characterSelection = 0;
		gameStart[0] = 'N';
		gameStart[1] = '0' + TEAM_NULL;
		gameStart[2] = '\0';
		serverIpArg = argv;
	}
	static void ErrorHandling(char *message);		// Error handling

	static void makeServerSocket();					// Server socket making routine
	static void acceptClient();						// Server accept client
	static void sendToClient(char *messageToClient);// Server message sending routine
	static void recieveFromClient();				// Server message recieving routine
	static void closeServerConnection();			// Server closing routine
	
	static void makeClientSocket();					// Client socket making routine
	static void connectToServer(){}					// Client connet to server 
	static void getProtocolDataFromServer();		// Client message recieving routine
	static void recieveGameStart();					// Client message recieving for game start
	static void sendToServer(char message[]);		// Client message sending routine
	static void closeClientConnection();			// Client closing routine
	
	static void update();							// Per frame routine for network
	static void turn();								// Per turn routine for network

	//getter and setter
	static protocol_command getCommandEnum(int index) {return (protocol_command)atoi(messageFromClient[index]); } 
	static int getMode() { return mode; }
	static int getCommand(int i) { return command[i]; }
	static int getCharacterSelection() { return characterSelection; }
	static char *getGameStart() { return gameStart; }
	static int getTeam() { return team - '0'; }

	static void setMode(int x) { mode = x; }
	static void setCommand(int i, int x) { command[i] = x; }
	static void setGameStart(int index, char x) { gameStart[index] = x; }
	static void setCharacterSelection(int x) { characterSelection = x; }
	static void setTeam(char x) { team = x; }
	
};