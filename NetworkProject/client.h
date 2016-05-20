#pragma once

void makeClientSocket(SOCKET * hSocket, char ** argv, SOCKADDR_IN *servAddr);
void connectToServer(SOCKADDR_IN *servAddr, SOCKET * hSocket);
void getProtocolDataFromServer(SOCKET * hSocket, char message[]);
void closeClientConnection(SOCKET * hSocket);
void sendToServer(SOCKET * hSocket, char message[]);
