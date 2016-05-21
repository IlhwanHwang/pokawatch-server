#pragma once
#include "protocol.h"

void ErrorHandling(char *message);
void communicate(int cnt);
void makeServerSocket(SOCKET * hServSock, SOCKADDR_IN *servAddr);
void acceptClient(SOCKET * hServSock, SOCKET  hClntSock[], SOCKADDR_IN clntAddr[], int szClntAddr[]);
void sendToClient(char messageToClient[], SOCKET  hClntSock[]);
void recieveFromClient(SOCKET hClntSock[], char messageFromClient[6][16]);
void closeServerConnection(SOCKET  hClntSock[]);
