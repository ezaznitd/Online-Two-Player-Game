/*
	Name: Ezaz Hossain Sardar
	Roll: 17CS8022
	
	***How to run***
		==>g++ TCP_Server.c
	It won't work for 
		==>gcc TCP_Server.c
		
		As this code is using some Inline function and Lambda expression.

	Assignment 3: Build a online Two player game using TCP/IP in C.
*/

//Online Two player Tic Tac Toe game in C

// Server Program:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<stdbool.h>

#define PORT 4444
int arr[5];
char board[20] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

int main(){

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024],buffer2[10000];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);
	if(listen(sockfd, 10) == 0)
		printf("[+]Listening....\n");
	else
		printf("[-]Error in binding.\n");
	int cnt=0,idx=0,conn=0;
	int newSocket2;
	newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
	if(newSocket < 0)
		exit(1);
	printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
	arr[conn]=newSocket;
	conn++;
	printf("Player 1 entered into the game.......\nTotal no of player = 1\nTotal no of game = 0\nwaiting for player 2...........\n");
	newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
	if(newSocket < 0)
		exit(1);
	printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
	arr[conn]=newSocket;
	printf("Player 2 entered into the game........\nTotal no of player = 2\nTotal no of game = 1\nLet's start the game.....\n");
	auto check_player_validity = [&](char ch)
	{
		if(ch>='1' and ch<='9')
		{
			for(int i=0;i<9;i++)
			{
				if(board[i]==ch)
				{
					board[i]='-1';
					return true;
				}
			}
			return false;
		}
		return false;
	};
	auto check_opponant_validity = [&](char ch)
	{
		if(ch=='p' or ch=='P')
			return true;
		return false;
	};
	while(1)
	{
		if(cnt==0)
		{
			//for 1st client;
			while(1)
			{
				char input[]="Enter a number: ";
				send(arr[cnt], input, strlen(input)+10, 0);
				bzero(input, sizeof(input));
				recv(arr[cnt], buffer, 1024, 0);
				printf("%d --> You have entered %s\n",arr[cnt],buffer);
				char in[10];
				in[0]='1';
				in[1]=buffer[0];
				send(arr[cnt], in, sizeof(in)+10, 0);
				bool valid=check_player_validity(buffer[0]);
				if(valid)
					break;
				else
					printf("You have entered wrong choice, enter correct choice:\n");
			}
			//for 2nd client;
			while(1)
			{
				char input2[]="Enter proceed/p/P to see opponant's movement: ";
				send(arr[cnt+1], input2, strlen(input2)+10, 0);
				bzero(input2, sizeof(input2));
				recv(arr[cnt+1], buffer2, 1024, 0);
				printf("%d --> You have entered %s\n",arr[cnt+1],buffer2);
				char in[10];
				in[0]='1';
				in[1]=buffer[0];
				send(arr[cnt+1], in, sizeof(in)+10, 0);
				bool valid=check_opponant_validity(buffer2[0]);
				if(valid)
				{
					cnt^=1;
					break;
				}
				else
					printf("You have entered wrong choice, enter correct choice:\n");
			}
		}
		else
		{
			//for 2nd client;
			while(1)
			{
				char input[]="Enter a number: ";
				send(arr[cnt], input, strlen(input)+10, 0);
				bzero(input, sizeof(input));
				recv(arr[cnt], buffer, 1024, 0);
				printf("%d --> You have entered %s\n",arr[cnt],buffer);
				char in[10];
				in[0]='2';
				in[1]=buffer[0];
				send(arr[cnt], in, sizeof(in)+10, 0);
				bool valid=check_player_validity(buffer[0]);
				if(valid)
					break;
				else
					printf("You have entered wrong choice, enter correct choice:\n");
			}
			//for 1st client;
			while(1)
			{
				char input2[]="Enter proceed/p/P to see opponant's movement: ";
				send(arr[cnt-1], input2, strlen(input2)+10, 0);
				bzero(input2, sizeof(input2));
				recv(arr[cnt-1], buffer2, 1024, 0);
				printf("%d --> You have entered %s\n",arr[cnt-1],buffer);
				char in[10];
				in[0]='2';
				in[1]=buffer[0];
				send(arr[cnt-1], in, sizeof(in)+10, 0);
				bool valid=check_opponant_validity(buffer2[0]);
				if(valid)
				{
					cnt^=1;
					break;
				}
				else
					printf("You have entered wrong choice, enter correct choice:\n");
			}
		}
		if(strcmp(buffer, ":exit") == 0)
		{
			printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
			break;
		}
		else
			printf("Client: %s\n", buffer);
	}
	close(newSocket);
	return 0;
}