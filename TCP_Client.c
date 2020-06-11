/*
    Name: Ezaz Hossain Sardar
    Roll: 17CS8022
    
    ***How to run***
        ==>g++ TCP_Client.c
    It won't work for 
        ==>gcc TCP_Client.c
        
        As this code is using some Inline function and Lambda expression.

    Assignment 3: Build a online Two player game using TCP/IP in C.
*/

//Online Two player Tic Tac Toe game in C

// Client program:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444
char square[10] = { 'o', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

int main()
{
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0)
    {
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
    {
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");
	//Tic Tac Toe game start here;
    auto checkwin = [&]()
    {
        if (square[1] == square[2] && square[2] == square[3])
            return 1;
        else if (square[4] == square[5] && square[5] == square[6])
            return 1;
        else if (square[7] == square[8] && square[8] == square[9])
            return 1;
        else if (square[1] == square[4] && square[4] == square[7])
            return 1;
        else if (square[2] == square[5] && square[5] == square[8])
            return 1;
        else if (square[3] == square[6] && square[6] == square[9])
            return 1;
        else if (square[1] == square[5] && square[5] == square[9])
            return 1;
        else if (square[3] == square[5] && square[5] == square[7])
            return 1;
        else if (square[1] != '1' && square[2] != '2' && square[3] != '3' &&
            square[4] != '4' && square[5] != '5' && square[6] != '6' && square[7] 
            != '7' && square[8] != '8' && square[9] != '9')
            return 0;
        else
            return  - 1;
    };
    auto board = [&]()
    {
        system("cls");
        printf("\n\n\tTic Tac Toe\n\n");
        printf("Player 1 (X)  -  Player 2 (O)\n\n\n");
        printf("     |     |     \n");
        printf("  %c  |  %c  |  %c \n", square[1], square[2], square[3]);
        printf("_____|_____|_____\n");
        printf("     |     |     \n");
        printf("  %c  |  %c  |  %c \n", square[4], square[5], square[6]);
        printf("_____|_____|_____\n");
        printf("     |     |     \n");
        printf("  %c  |  %c  |  %c \n", square[7], square[8], square[9]);
        printf("     |     |     \n\n");
    };
	int i, choice;
	char player='1';
    char mark;
    int is_valid=1;
	do
    {
        board();
        if(recv(clientSocket, buffer, 1024, 0) < 0)
			printf("[-]Error in receiving data.\n");
		else
			printf("Server: \t%s\n", buffer);
		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer)+10, 0);
		if(recv(clientSocket, buffer, 1024, 0) < 0)
			printf("[-]Error in receiving data.\n");
		else{
			player=buffer[0];
			choice=buffer[1];
		}
        mark = (player == '1') ? 'X' : 'O';
        if (choice == '1' && square[1] == '1')
            square[1] = mark;
        else if (choice == '2' && square[2] == '2')
            square[2] = mark;
        else if (choice == '3' && square[3] == '3')
            square[3] = mark;
        else if (choice == '4' && square[4] == '4')
            square[4] = mark;
        else if (choice == '5' && square[5] == '5')
            square[5] = mark;
        else if (choice == '6' && square[6] == '6')
            square[6] = mark;
        else if (choice == '7' && square[7] == '7')
            square[7] = mark;
        else if (choice == '8' && square[8] == '8')
            square[8] = mark;
        else if (choice == '9' && square[9] == '9')
            square[9] = mark;
        else
        {
            printf("Invalid move ");
            getchar();
        }
        i = checkwin();
    }
    while (i ==  - 1);
    board();
    if (i == 1)
        printf("==>\aPlayer %d win\n", (player-'0'));
    else
        printf("==>\aGame draw");
    getchar();
	return 0;
}