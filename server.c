#include <stdio.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "player.h"
#include "socket.h"

void playTurn(Player * playersGameInfo, int maxClients){
	for(int i = 0; i < maxClients-1; i++){
		Player * current = playersGameInfo[i]; 
		switch(playersGameInfo[i]->state){
			case GENERATE:
				generate(current);
			case ATTACK:
				Player * target = playersGameInfo[current->targetId];
				attack(current, target);
				break;
			case DEFENSE:
				defend(current);
			default :
				fprint(stderr,"UNDEFINED STATE");
				exit(0);
		}
	}
}

void initPlayersGameInfo(Player * playersGameInfo, int maxClients){
	playersGameInfo = malloc(sizeof(Player)*maxClients);
	for(int i = 0; i < maxClients-1; i++){
		playersGameInfo[i] = createPlayer();
	}
}

void askServerInfo(char * name, char * password, int * maxClients){
	printf("Server name : ");
	scanf("%s",name);
	printf("Password : ");
	system("stty -echo");
	scanf("%s",password);
	system("stty echo");
	printf("\nPlayers : ");
	scanf("%d",maxClients);
}

int main(){
	char name[50];
	char password[50];
	int maxClients;
	
	askServerInfo(name, password, &maxClients);

	SOCKADDR_IN socketAddress = { 0 };

	SOCKADDR_IN clientAddress [8];
	int clientSockets [8];	
	char clientName [8][30];
	Player * playersGameInfo;

	int adressLenght, sock;
	int clientCount = 0;
	char buffer [512];

	socketAddress.sin_port = htons(PORT);
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	if((sock = socket(AF_INET, SOCK_STREAM,0)) == -1){
		printf("SOCKET CREATION ERROR !\n");
		exit(0);
	}
	bind(sock,(SOCKADDR *) &socketAddress, sizeof(socketAddress));
	if(listen(sock, 5) == SOCKET_ERROR){
		perror("ERROR LISTEN");
		exit(errno);
	}
	adressLenght = sizeof(clientAddress);
	printf("\n");
	while(clientCount != maxClients)
	{
		int n = 0;
		fprintf(stderr,"Waiting for connexions ...\n");
		clientSockets[clientCount] = accept(sock,(SOCKADDR *) &clientAddress[clientCount], &adressLenght);
		if(clientSockets[clientCount] == INVALID_SOCKET){
			perror("ACCEPT ERROR : ");
			exit(errno);
		}
		if((n = recv(clientSockets[clientCount], buffer, sizeof buffer - 1, 0)) < 0){
			perror("RECV ERROR");
			exit(errno);
		}
		buffer[n] = '\0';
		strcpy(clientName[clientCount],buffer);
			
		printf("%s joined the server %s !\n", clientName[clientCount], name);
		clientCount ++;
		printf("%d/%d players connected\n\n", clientCount, maxClients);
		
		char * mess = " joined the server ";
		mess = strcat(buffer,  mess);	
		sprintf(mess,"%s !\n %d / %d players connected\n \n", mess, clientCount, maxClients);
		for(int i = 0; i < clientcount-1; i++){
                    	if(send(clientsockets[i], mess, strlen(mess),0) < 0){
				perror("send error to all: ");
				exit(errno);
			}
               	}
		char mes [256]  = "Welcome ";
		strcat(mes, clientName[clientCount-1]);
		sprintf(mes,"%s to the server %s !\n", mes, name);
		if(send(clientSockets[clientCount-1], mes, strlen(mes),0) < 0){
			perror("SEND ERROR TO NEWCOMER :");
			exit(errno);
		}
	}
	for(int i = 0; i < maxClients-1; i++){
		if(send(clientsockets[i], "READY", strlen("READY"),0) < 0){
			perror("SEND \"READY\" TO EVERYONE: ");
			exit(errno);
		}
	}
	initPlayersGameInfo(playersGameInfo, maxClient);
	while(1){
		wait(1);
		playTurn(playersGameInfo, maxClient);
		for(int i = 0; i < clientCount-1; i++){
			if(send(clientSockets[i], playersGameInfo, sizeof(Player),0) < 0){
				perror("SEND GAMEINFO ERROR : ");
				exit(errno);
			}
		}
	}
	for(int i = 0; i < clientCount-1; i++){
		close(clientSockets[i]);
	}

	return 0;
}
