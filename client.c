#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include "display.h"
#include "player.h"
#include "socket.h"
#include "clientThread.h"

int string2unsignedint(char * stringNumber) {
  int number = 0;
  int i = 0;

  int stringLength = strlen(stringNumber);

  while (i < stringLength) {
    int digit = stringNumber[i++] - '0';
    if (digit < 0 || digit > 9) {
      printf("Invalid character '%c' on the position '%d'\n", stringNumber[i - 1],(i - 1));
      return -1;
    }
    number *= 10;
    number += digit;
  }
}

int getNextNumber(char * str){
	char buffer[8];
	while(str[0]!= '/'){
		sprintf(buffer,"%c",str[0]);
		str++;
	}
	str++;
	return string2unsignedint(buffer);
}

char * getNextPlayer(char * str){
	char * buffer = malloc(sizeof(char[256]));
	while(str[0] != '-'){
		sprintf(buffer,"%c",str[0]);
		str++;
	}
	str++;
	return buffer;
}

Player deserializePlayer(char * str){
	Player p;
	p.id 			= getNextNumber(str);
	p.ohen 			= getNextNumber(str);
	p.max_ohen 		= getNextNumber(str);
	p.shield 		= getNextNumber(str);
	p.attack_damage = getNextNumber(str);
	p.defense 		= getNextNumber(str);
	p.health 		= getNextNumber(str);
	p.max_health 	= getNextNumber(str);
	p.regen_ohen 	= getNextNumber(str);
	p.regen_health 	= getNextNumber(str);
	p.vulnerable 	= getNextNumber(str);
	p.state 		= getNextNumber(str);
	return p;
}

Player * deserializePlayers(char * charr, int playerCount){
	Player * players = malloc(sizeof(Player)*playerCount);
	for(int i = 0; i < playerCount; i++){
		char * player = getNextPlayer(charr);
		players[i] = deserializePlayer(player);
	}
	return players;
}


void displayPlayersInfo(Player ** infos, int playerNumber){
	for(int i = 0; i < playerNumber; i++){
		printf("Player %d's health : %d\n", i, infos[i]->health);
		printf("Player %d's max_health : %d\n", i, infos[i]->max_health);
		printf("Player %d's ohen : %d\n", i, infos[i]->ohen);
		printf("Player %d's max_ohen : %d\n", i, infos[i]->max_ohen);
	}
}

void * listenFromServer(void * args){
	while(1){
		int n = 0;
		ListenThreadArgs * arg = (ListenThreadArgs *) args;
		if(arg == NULL){
			perror("Thread argument is not a pointer on a ListenThreadArgs struct"); 
			exit(3);
		}
		char gameInfo [256*arg->clientCount];
		if((n = recv(arg->sock, gameInfo, sizeof gameInfo, 0)) < 0){
			perror("RECV GAMEINFO ERROR ");
			exit(errno);
		}
		if(n != 0){
			arg->playersGameInfo = deserializePlayers(gameInfo, arg->clientCount);
		}
	}
}

// Display the connexion menu.
void menuConnexion(int color, int spacingLeft, int size, char * nickname, char * hostname){
	clear();
	Coords c1;
	Coords c2;
	emptyBlankLine(4);
	simpleLine(1,color, spacingLeft, size);
	messageLine(0, "CONNEXION :", color, spacingLeft, size);
	emptyColoredShadowedLine(1, color, spacingLeft, size);
	c1.x = inputLine("Nickname :", color, 7, 30, spacingLeft, size);
	c1.y = 8;
	emptyColoredShadowedLine(1, color, spacingLeft, size);
	c2.x = inputLine("Hostname :", color, 7, 30, spacingLeft, size);
	c2.y = c1.y+4;
	emptyColoredShadowedLine(1, color, spacingLeft, size);
	lastLine(3, color, spacingLeft, size);
	setColor(47); //TOREMOVE
	setColor(30); //TOREMOVE
	putCursor(c1.x, c1.y);
	scanf("%s", nickname);
	putCursor(c2.x, c2.y);
	scanf("%s", hostname);
}

int main(){
	pthread_t listenServer;
	Color color = RED;
	char buffer [512];
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		perror("Invalid socket");
		exit(errno);
	}

	SOCKADDR_IN address = { 0 };
	char hostname [32];
	char nickname [32];
	
	Player * playersGameInfo;
	int clientCount = 0;

	// Print the connexion menu.
	menuConnexion(color, 4, 64, nickname, hostname);

	struct hostent * hostinfo = gethostbyname(hostname);

	if(hostinfo == NULL){
		fprintf(stderr, "Unknown host \"%s\".\n", hostname);
		exit(EXIT_FAILURE);
	}

	address.sin_addr = *(IN_ADDR *) hostinfo -> h_addr;
	address.sin_port = htons(PORT);
	address.sin_family = AF_INET;

	printf("Connexion ...\n");
	if(connect(sock,(SOCKADDR *) &address, sizeof(SOCKADDR)) == -1){
		perror("Connexion error");
		exit(errno);
	}
	printf("Connexion réussie !\n");
	if(send(sock, nickname, strlen(nickname), 0) < 0){
		perror("SEND ERROR : ");
		exit(errno);
	}
	// Attente du lancement de la partie.
	while(strcmp(buffer, "READY") != 0){
		int n = 0;
		if((n=recv(sock, buffer, sizeof buffer -1, 0)) < 0){
			perror("RECV WELCOME MESSAGE ERROR ");
			exit(errno);
		}
		buffer[n] = '\0';
		if(n != 0)
			printf("<Message from server>\n %s \n", buffer);
	}
	
	//Reception du nombre de joueur par le serveur.
	int r = 0;
	if((r = recv(sock, &clientCount, sizeof clientCount, 0)) < 0){
		perror("RECV CLIENT COUNT ERROR ");
		exit(errno);
	}
	clientCount -= 48;
	if(r != 0)
		printf("<Message from server>\n %d \n", clientCount);
	
	
	// Creation du thread qui va recevoir les infos du serveur.
	ListenThreadArgs args;
	args.playersGameInfo = playersGameInfo;
	args.sock = sock;
	args.clientCount = clientCount;
	if(pthread_create(&listenServer, NULL, listenFromServer, &args) == -1) {
		perror("pthread_create");
		return -1;
    }
	while(1){
		
	}
	
	return 0;
}
