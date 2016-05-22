#pragma comment(lib, "ws2_32.lib")

#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <cstdio>
#include <string>
#include "server.h"
#include "protocol.h"
/*
using namespace std;



Server::Server() {}

void Server::ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void Server::communicate(int cnt)
{
	printf("와우 오졌따리");
}

void Server::makeServerSocket()
{
	string portString = PORT_STRING;
	char * port = (char*)portString.c_str();
	
	WSADATA wsaData;

	// Load Winsock 2.2 DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error!");

	// 서버 소켓 생성
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET) ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(port));

	// 소켓에 주소 할당
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) ErrorHandling("bind() error");

	// 연결 요청 대기 상태
	if (listen(hServSock, UNIT_NUM_MAX) == SOCKET_ERROR) ErrorHandling("listen() error");

	printf("listen");
}

void Server::acceptClient()
{
	// 연결 요청 수락
	for (int i = 0; i < UNIT_NUM_MAX; i++) szClntAddr[i] = sizeof(clntAddr[i]);
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		hClntSock[i] = accept(hServSock, (SOCKADDR*)&clntAddr[i], &szClntAddr[i]);
		if (hClntSock[i] == INVALID_SOCKET) ErrorHandling("accept() error");
	}
}

void Server::sendToClient(char messageToClient[])
{
	for (int i = 0; i < UNIT_NUM_MAX; i++)	send(hClntSock[i], messageToClient, sizeof(messageToClient), 0);
}

void Server::recieveFromClient()
{
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		int strLen = recv(hClntSock[i], messageFromClient[i], MESSAGE_TO_SERVER_SIZE, 0);
		messageFromClient[i][strLen] = '\0';
	}
}

void Server::closeServerConnection()
{
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		closesocket(hClntSock[i]);
	}
	WSACleanup();
}




/*
int main(int argc, char **argv)
{
WSADATA wsaData;
SOCKET hServSock;
SOCKET hClntSock;
SOCKET hClntSock2;
SOCKADDR_IN servAddr;
SOCKADDR_IN clntAddr;
SOCKADDR_IN clntAddr2;
int szClntAddr;
int szClntAddr2;

char message[] = "Hello World!\n";

if (argc != 2)
{
printf("Usage : %s <port> \n", argv[0]);
exit(1);
}

// Load Winsock 2.2 DLL
if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
{
ErrorHandling("WSAStartup() error!");
}

// 서버 소켓 생성
hServSock = socket(PF_INET, SOCK_STREAM, 0);
if (hServSock == INVALID_SOCKET)
{
ErrorHandling("socket() error");
}

memset(&servAddr, 0, sizeof(servAddr));
servAddr.sin_family = AF_INET;
servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
servAddr.sin_port = htons(atoi(argv[1]));

// 소켓에 주소 할당
if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
{
ErrorHandling("bind() error");
}

// 연결 요청 대기 상태
if (listen(hServSock, 5) == SOCKET_ERROR)
{
ErrorHandling("listen() error");
}

// 연결 요청 수락
szClntAddr = sizeof(clntAddr);
szClntAddr2 = sizeof(clntAddr2);
hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
hClntSock2 = accept(hServSock, (SOCKADDR*)&clntAddr2, &szClntAddr2);

if ((hClntSock == INVALID_SOCKET) || hClntSock2 == INVALID_SOCKET)
{
ErrorHandling("accept() error");
}

char *from_client = new char[1024];
char *from_client2 = new char[1024];

// 데이터 전송
send(hClntSock, message, sizeof(message), 0);
send(hClntSock2, message, sizeof(message), 0);

int cnt = 5;
while (cnt--)
{
Sleep(500);

int strLen = recv(hClntSock, from_client, 1024, 0);
int strLen2 = recv(hClntSock2, from_client2, 1024, 0);

from_client[strLen] = '\0';
from_client2[strLen2] = '\0';
printf("this is %s from client0\n", from_client);
printf("this is %s from client1\n\n", from_client2);
}



// 연결 종료
closesocket(hClntSock);
WSACleanup();

return 0;
}
*/
