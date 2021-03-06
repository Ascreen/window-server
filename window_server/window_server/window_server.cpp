// window_server.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"


#pragma comment(lib,"ws2_32")
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define PORT 20000
#define LENGTH 5000


//void ErrorHandling(const char* message);

int main(int argc, char** argv)
{
	WSADATA wsaData;	 //	초기화 할때 쓰려고 선언했습니다.	//
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	char revbuf[LENGTH]; // Receiver buffer
	int success = 0;
	int szClntAddr;

	if (argc != 2) {	//	포트정보가 입력되었는지 여부 검사	//
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	//	윈속 2.2사용 초기화	//
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)	//	Error check	//
		exit(1);
	else
		printf("[Server] Winsock2.2 Configuring is successfully\n");

	//	소켓을 생성합니다.	//
	hServSock = socket(AF_INET, SOCK_STREAM, 0);	//	TCP socket	//
	if (hServSock == INVALID_SOCKET)
		exit(1);
	else
		printf("[Server] Creating socket successfully\n");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;			//	TCP를 사용합니다.	//
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	//	모든 IP를 받아줍니다.	//
	servAddr.sin_port = htons(atoi(argv[1]));	//	포트 정보	//
												//  host to network short

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)	//	소켓에 주소를 부여합니다.	//
		exit(1);
	else
		printf("[Server] Binded tcp port %d in addr host successfully.\n", PORT);

	if (listen(hServSock, 10) == SOCKET_ERROR)	//	소켓대기상태로 만듭니다, 동시에 10명까지 가능합니다.	//
		exit(1);
	else
		printf("[Server] Listening the port %d successfully.\n", PORT);

	while (success == 0)
	{
		szClntAddr = sizeof(SOCKADDR_IN);

		/* Wait a connection, and obtain a new socket file despriptor for single connection */
		if ((hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr)) == SOCKET_ERROR)
		{
			fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
			exit(1);
		}
		else
			printf("[Server] Server has got connected from Clinet.\n");// inet_ntoa(addr_remote.sin_addr));

																	   /*	Receive File from Client(show console)	*/
																	   //int fr_block_sz = 0;
																	   //while ((fr_block_sz = recv(hClntSock, revbuf, LENGTH, 0)) > 0)
																	   //{
																	   //	printf(revbuf);
																	   //}
																	   /*	Receive File from Client(save text)		*/
		const char* fr_one = "C:/Users/ksy/Desktop/one.txt";
		const char* fr_two = "C:/Users/ksy/Desktop/two.txt";
		const char* fr_three = "C:/Users/ksy/Desktop/three.txt";
		const char* fr_four = "C:/Users/ksy/Desktop/four.txt";

		FILE *fr;
		
		
		
		
		/*
		
		if (fr1 == NULL)
			printf("File %s Cannot be opened file on server.\n", fr1);
		else if(fr2 == NULL)
			printf("File %s Cannot be opened file on server.\n", fr2);
		else if(fr3==NULL)
			printf("File %s Cannot be opened file on server.\n", fr3);
		else if(fr4==NULL)
			printf("File %s Cannot be opened file on server.\n", fr4);
		*/
		
		//else{
			memset(revbuf, LENGTH, '0');
			int fr_block_sz = 0;
			while ((fr_block_sz = recv(hClntSock, revbuf, LENGTH, 0)) > 0)
			{
				if (!strncmp(&revbuf[0],"1",1)) {
					fr = fopen(fr_one, "w");
				}
				else if (!strncmp(&revbuf[0], "2",1)) {
					fr = fopen(fr_two, "w");
				}
				else if (!strncmp(&revbuf[0], "3",1)) {
					fr = fopen(fr_three, "w");
				}
				else {
					fr = fopen(fr_four, "w");
				}
				int write_sz1 = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
				if (write_sz1 < fr_block_sz)
				{
					exit(1);
				}
				
				memset(revbuf, LENGTH, '0');
				if (fr_block_sz == 0 || fr_block_sz != 1024)
				{
					break;
				}
			}
		//}
		fclose(fr);
	}
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}