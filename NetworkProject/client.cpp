#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <winsock2.h>

#include <conio.h>
#include "client.h"
#include "server.h"

using namespace std;

void makeClientSocket(SOCKET * hSocket, SOCKADDR_IN *servAddr)
{
	string servIpString = "119.202.87.81";
	string portNumString = "2222";
	char * servIp = (char*)servIpString.c_str();
	char * portNum = (char*)portNumString.c_str();

	WSADATA wsaData;

	// Load WinSocket 2.2 DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup(), error");
	}

	// 서버 접속을 위한 소켓 생성
	*hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (*hSocket == INVALID_SOCKET)
	{
		ErrorHandling("hSocketet(), error");
	}

	memset(servAddr, 0, sizeof(*servAddr));
	servAddr->sin_family = AF_INET;
	servAddr->sin_addr.s_addr = inet_addr(servIp);
	servAddr->sin_port = htons(atoi(portNum));
}

void connectToServer(SOCKADDR_IN *servAddr, SOCKET * hSocket)
{
	// 서버로 연결 요청
	if (connect(*hSocket, (SOCKADDR*)servAddr, sizeof(*servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("Connect() error");
	}
}

void getProtocolDataFromServer(SOCKET * hSocket, char message[])
{
	int strLen;
	// 데이터 수신
	strLen = recv(*hSocket, message, sizeof(message) - 1, 0);
	if (strLen == -1)
	{
		ErrorHandling("read() error");
	}
	message[strLen] = '\0';
}

void closeClientConnection(SOCKET * hSocket)
{
	// 연결 종료
	closesocket(*hSocket);
	WSACleanup();
}

void sendToServer(SOCKET * hSocket, char message[])
{
	send(*hSocket, message, sizeof(message), 0);
}





/*==================================================
Error Message print
==================================================*/
/*void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

*/
/*==================================================
Entry Point
==================================================*/
/*int main(int argc, char **argv)
{
WSADATA wsaData;
SOCKET hSocket;
char message[30];
int strLen;
SOCKADDR_IN servAddr;

if (argc != 3)
{
printf("Usage : %s <ip> <port>\n", argv[0]);
exit(1);
}




// 서버로 연결 요청
if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
{
ErrorHandling("Connect() error");
}

// 데이터 수신
strLen = recv(hSocket, message, sizeof(message) - 1, 0);
if (strLen == -1)
{
ErrorHandling("read() error");
}
message[strLen] = 0;
printf("Message from server : %s \n", message);
char message2[] = "1";


int cnt = 5;
while (cnt--)
{
Sleep(1000);
/*	printf("입력해주세요 %d: ", cnt);
int sub_cnt = 500;
printf("%d", sub_cnt);
//char* c;
bool c;
if(!(sub_cnt--)|| (c = scanf("%s", message2)))
{

printf("%d", c);
} */

/*int sub = 300;
while (sub) {
char input;

if (_kbhit()) {
/* Use _getch to throw key away. */
/*printf("\nKey struck was '%c'\n", input = _getch());
message2[0] = input;
send(hSocket, message2, sizeof(message2), 0);
}
else {
sub--;
printf("%d", sub);
}
}
message2[0] = '9';
send(hSocket, message2, sizeof(message2), 0);
}



// 연결 종료
closesocket(hSocket);
WSACleanup();

return 0;
}*/
