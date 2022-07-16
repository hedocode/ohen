#include <stdio.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include "../lib/player.h"
#include "../lib/socket.h"
#include "serverThread.h"

//Send a message throught the socket sock.
void sendMessage(int sock, char * message){
	if(sock != -1){
		if(send(sock, message, strlen(message),0) < 0){
			perror("Send Message Error : ");
			printf("Message was : \"%s\"", message);
			exit(errno);
		}
	}
}

// Send a message to all the clients.
void sendMessageToClients(int * clientSockets, int clientCount, char * message){
	for(int i = 0; i < clientCount; i++){
		sendMessage(clientSockets[i], message);
	}
}

// Makes the player at the chosen index generating ohen.
void setPlayerGenerate(Player ** players, int playerIndex){
	players[0][playerIndex].state = GENERATE;
}

// Makes the player at the chosen index attacking a player at an other chosen index.
void setPlayerAttack(Player ** players, int playerIndex, int playerCount, char * target){
	if(strlen(target) == 1){	
		int targetId = string2unsignedint(target);
		if(targetId >= 0 && targetId < playerCount){
			players[0][playerIndex].state = ATTACK;
			players[0][playerIndex].targetId = targetId;
		}
	}
	else{
		for(int i = 0; i < playerCount; i++){
			if(strcmp(players[0][i].nickname, target) == 0){
				players[0][playerIndex].state = ATTACK;
				players[0][playerIndex].targetId = i;
			}
		}
	}
}

// Makes the player at the chosen index defending himself.
void setPlayerDefend(Player ** players, int playerIndex){
	players[0][playerIndex].state = DEFENSE;
}

// Play the turn
void playTurn(int * clientSockets, Player * playersGameInfo, int maxClients){
	for(int i = 0; i < maxClients; i++){
		fprintf(stderr,"Playing player %d turn\n",i);
		fprintf(stderr,"Player %d's state : %d\n",i,playersGameInfo[i].state);
		switch(playersGameInfo[i].state){
			case GENERATE:
				fprintf(stderr, "GENERATING\n");
				generate(&playersGameInfo[i]);
				break;
			case ATTACK:
				fprintf(stderr, "ATTACKING\n");
				attack(&playersGameInfo[i], &playersGameInfo[playersGameInfo[i].targetId]);
				if(playersGameInfo[playersGameInfo[i].targetId].health <= 0)
					sendMessage(clientSockets[playersGameInfo[i].targetId], "KO");
				break;
			case DEFENSE:
				fprintf(stderr, "DEFENDING\n");
				defend(&playersGameInfo[i]);
				break;
			default :
				fprintf(stderr,"UNDEFINED STATE ERROR");
				exit(0);
		}
	}
}

// Player listen thread code
void * listenPlayer(void * args){
	char buffer [512];
	char message [256];
	char * word;
	int n = 0;
	ListenClientThreadArgs * arg = (ListenClientThreadArgs *) args;
	if(arg == NULL){
		perror("Thread argument is not a pointer on a ListenThreadArgs struct"); 
		exit(3);
	}
	while(arg->playersGameInfo[0][arg->playerIndex].health > 0 && *arg->end == false){
		if((n = recv(arg->sock, buffer, sizeof buffer - 1, 0)) < 0){
				perror("RECV ERROR");
				exit(errno);
		}
		buffer[n] = '\0';
		word = getNextString(buffer);
		fprintf(stderr,"%s",word);
		if(strcmp(word,"generate") == 0){
			sprintf(message,"generate %d", arg->playerIndex);
			setPlayerGenerate(arg->playersGameInfo, arg->playerIndex);
			sendMessageToClients(arg->clientSockets, arg->playerCount, message);
		}
		else if(strcmp(word,"attack") == 0){
			setPlayerAttack(arg->playersGameInfo, arg->playerIndex, arg->playerCount, getNextString(buffer));
			sprintf(message,"attack %d", arg->playerIndex);
			sendMessageToClients(arg->clientSockets, arg->playerCount, message);
		}
		else if(strcmp(word,"defend") == 0){
			setPlayerDefend(arg->playersGameInfo, arg->playerIndex);
			sprintf(message,"defend %d", arg->playerIndex);
			sendMessageToClients(arg->clientSockets, arg->playerCount, message);
		}
		else if(strcmp(word, "inc_ohen_regen") == 0){
			bool done = upgradeRegenOhen(&arg->playersGameInfo[0][arg->playerIndex]);
			if(done == true){
				sprintf(message,"inc_ohen_regen %d", arg->playerIndex);
				sendMessageToClients(arg->clientSockets, arg->playerCount, message);
			}
		}
		else if(strcmp(word, "inc_attack") == 0){
			bool done = upgradeAttackDamage(&arg->playersGameInfo[0][arg->playerIndex]);
			if(done == true){
				sprintf(message,"inc_attack %d", arg->playerIndex);
				sendMessageToClients(arg->clientSockets, arg->playerCount, message);
			}
		}
		else if(strcmp(word, "inc_defence") == 0){
			bool done = upgradeDefense(&arg->playersGameInfo[0][arg->playerIndex]);
			if(done == true){
				sprintf(message,"inc_defence %d", arg->playerIndex);
				sendMessageToClients(arg->clientSockets, arg->playerCount, message);
			}
		}
		else if(strcmp(word,"disconnect") == 0){
			arg->clientSockets[arg->playerIndex] = -1;
			close(arg->sock);
			sprintf(message,"inc_defence %d", arg->playerIndex);
			sendMessageToClients(arg->clientSockets, arg->playerCount, message);
			return NULL;
		}
	}
	pthread_exit(NULL);
}

// Initialize the PlayersGameInfo array.
void initPlayersGameInfo(Player * playersGameInfo, int maxClients, char names[8][30]){
	for(int i = 0; i < maxClients; i++){
		playersGameInfo[i] = createPlayer(i, names[i]);
		fprintf(stderr, "Player %d's state : %d", i, playersGameInfo[i].state);
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

// Server main code.
int main(int argc, char ** argv){
	pthread_t * listenClient;
	char * name = malloc(sizeof(char[50]));
	char password[50];
	int maxClients;

	bool end;
	int aliveCount;
	int lastAliveIndex;
	
	if(argc == 3){
		name = argv[1];
		maxClients = atoi(argv[2]);
	}
	else{
		askServerInfo(name, password, &maxClients);
	}
	
	SOCKADDR_IN socketAddress = { 0 };

	SOCKADDR_IN clientAddress [8];
	int clientSockets [8];
	char clientName [8][30];
	Player * playersGameInfo;

	unsigned int adressLenght; 
	int sock;
	int clientCount = 0;
	char buffer [512];
	char message [256];
	
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
	
	while(1){	
		clientCount = 0;
		end = false;

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
				perror("RECV ERROR 2");
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
				sendMessage(clientSockets[i], mess);
			}
			//SEND A WELCOME MESSAGE TO THE NEWCOMER
			sprintf(message, "Welcome %s to the server %s !\n", clientName[clientCount-1], name);
			sendMessage(clientSockets[clientCount-1], message);
		}
		
		sleep(1);
		
		// Send Ready to all the Clients
		sendMessageToClients(clientSockets, clientCount, "READY");
		
		sleep(1);
		
		//Sending client count to all the clients
		for(int i = 0; i < maxClients; i++){
			char cc = clientCount + '0';
			if(send(clientSockets[i], &cc, sizeof(char), 0) < 0){
				perror("SEND PLAYER COUNT TO EVERYONE: ");
				exit(errno);
			}
		}
		
		listenClient = malloc(sizeof(pthread_t)*clientCount);
		playersGameInfo = malloc(sizeof(Player) * maxClients);
		initPlayersGameInfo(playersGameInfo, maxClients, clientName);
		
		ListenClientThreadArgs * args = malloc(sizeof(ListenClientThreadArgs)*clientCount);
		// Creation des threads qui vont recevoir les infos des clients.
		for(int i = 0; i < clientCount; i++){
			args[i].clientSockets = clientSockets;
			args[i].playersGameInfo = &playersGameInfo;
			args[i].playerCount = clientCount;
			args[i].sock = clientSockets[i];
			args[i].playerIndex = i;
			args[i].end = &end;
			if(pthread_create(&listenClient[i], NULL, listenPlayer, &args[i]) == -1) {
				perror("pthread_create");
				return -1;
			}
		}
		
		//Game loop
		while(!end){
			playTurn(clientSockets, playersGameInfo, maxClients);
			aliveCount = 0;
			//Win Verification
			for(int i = 0; i < clientCount; i++){
				if(playersGameInfo[i].ohen == playersGameInfo[i].max_ohen){
					sprintf(message, "%s won by suprematy !\n", clientName[i]);
					printf("%s", message);
					sprintf(message,"GOSU %d", i);
					sendMessageToClients(clientSockets, clientCount, message);
					end = true;
				}
				if(playersGameInfo[i].health != 0){
					aliveCount ++;
					lastAliveIndex = i;
				}
			}		
			if(aliveCount == 1){
				sprintf(message, "%s won by K.O", clientName[lastAliveIndex]);
				printf("%s",message); 
				sprintf(message, "GOKO %d", lastAliveIndex);
				sendMessageToClients(clientSockets, clientCount, message);
				end = true;
			}
			char * gameInfo = serializePlayers(playersGameInfo, clientCount);
			fprintf(stderr,"%s\n",gameInfo);
			
			//Send serialized infos to the players
			for(int i = 0; i < clientCount; i++){
				if(clientSockets[i] != -1){				
					if(send(clientSockets[i], gameInfo, sizeof(char[256*clientCount]),0) < 0){
						perror("SEND GAMEINFO ERROR : ");
						exit(errno);
					}
				}
			}
			bool verif = 1;
			for(int i = 0; i < clientCount; i++){
				if(clientSockets[i] != -1){
					verif = 0;
				}
			}
			if(verif == 1){
				end = verif;
				printf("EVERYONE DISCONNECTED\n");
			}
			sleep(1);
		}
		
		//TODO : Freeing all the mallocs -> playersGameInfo
		free(listenClient);
		free(playersGameInfo);
		
		// Closing the sockets
		for(int i = 0; i < clientCount-1; i++){
			if(clientSockets[i] != 0)
				close(clientSockets[i]);
		}
		sleep(1);
	}
	return 0;
}
