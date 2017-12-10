#include <stdio.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include "player.h"
#include "socket.h"

// Play the turn
void playTurn(Player ** playersGameInfo, int maxClients){
	for(int i = 0; i < maxClients; i++){
		fprintf(stderr,"Playing player %d turn\n",i);
		Player * current = playersGameInfo[i];
		fprintf(stderr,"Player %d's Max Ohen %d\n",i,current[i].max_ohen);
		switch(current->state){
			case GENERATE:
				generate(current);
			case ATTACK:
				attack(current, playersGameInfo[current->targetId]);
				break;
			case DEFENSE:
				defend(current);
			default :
				fprintf(stderr,"UNDEFINED STATE ERROR");
				exit(0);
		}
		fprintf(stderr,"%d\n",i);
	}
}

// Player listen thread code
void listenPlayer(void * args){
	
}

// Initialize the PlayersGameInfo array.
void initPlayersGameInfo(Player ** playersGameInfo, int maxClients){
	playersGameInfo = malloc(sizeof(Player **));
	*playersGameInfo = malloc(sizeof(Player) * maxClients);
	for(int i = 0; i < maxClients; i++){
		playersGameInfo[i] = malloc(sizeof(Player));
		playersGameInfo[i] = createPlayer(i);
	}
}

// User data input
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
	// LOOP Waiting for all the clients to connect.
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
		
		//Print the event on the server.
		printf("%s joined the server %s !\n", clientName[clientCount], name);
		clientCount ++;
		printf("%d/%d players connected\n\n", clientCount, maxClients);
		
		//Send message to all the old clients.
		char * mess = " joined the server ";
		mess = strcat(buffer,  mess);	
		sprintf(mess,"%s !\n %d / %d players connected\n \n", mess, clientCount, maxClients);
		for(int i = 0; i < clientCount-1; i++){
                    	if(send(clientSockets[i], mess, strlen(mess),0) < 0){
				perror("send error to all: ");
				exit(errno);
			}
        }
		//SEND A WELCOME MESSAGE TO THE NEWCOMER
		char mes [256]  = "Welcome ";
		strcat(mes, clientName[clientCount-1]);
		sprintf(mes,"%s to the server %s !\n", mes, name);
		if(send(clientSockets[clientCount-1], mes, strlen(mes),0) < 0){
			perror("SEND ERROR TO NEWCOMER :");
			exit(errno);
		}
	}
	
	sleep(1);
	
	// Send Ready to all the Clients
	for(int i = 0; i < maxClients; i++){
		if(send(clientSockets[i], "READY", strlen("READY"),0) < 0){
			perror("SEND \"READY\" TO EVERYONE: ");
			exit(errno);
		}
		char cc = clientCount + '0';
		if(send(clientSockets[i], &cc, sizeof(char), 0) < 0){
			perror("SEND PLAYER COUNT TO EVERYONE: ");
			exit(errno);
		}
	}
	fprintf(stderr,"I LOVE IT\n");
	initPlayersGameInfo(&playersGameInfo, maxClients);
	printf("%s's Max Ohen : %d\n", clientName[0],playersGameInfo[0].max_ohen);
	fprintf(stderr,"END ?\n");
	while(1){
		sleep(1);
		fprintf(stderr,"I LOVE IT\n");
		playTurn(&playersGameInfo, maxClients);
		fprintf(stderr,"END ?\n");
		char * gameInfo = serializePlayers(&playersGameInfo, clientCount);
		//Send serialized infos to the players
		for(int i = 0; i < clientCount; i++){
			if(send(clientSockets[i], gameInfo, sizeof gameInfo,0) < 0){
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
