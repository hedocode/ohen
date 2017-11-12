#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"
#include "player.h"
#include "socket.h"

//Gitea
//Jitea

void menuConnexion(int color, int spacingLeft, int size, char * nickname, char * hostname){
	clear();
	Coords c1;
	Coords c2;
	emptyBlankLine(4);
	simpleLine(1,color, spacingLeft, size);
	messageLine(0, "CONNEXION :", color, spacingLeft, size);
	emptyColoredShadowedLine(1, color, spacingLeft, size);
	c1.x = inputLine("Nickname :", color, 7, 30, spacingLeft, size);
	c1.y = 4+1+1+1+3;
	emptyColoredShadowedLine(1, color, spacingLeft, size);
	c2.x = inputLine("Hostname :", color, 7, 30, spacingLeft, size);
	c2.y = c1.y+2+2;
	emptyColoredShadowedLine(1, color, spacingLeft, size);
	lastLine(3, color, spacingLeft, size);
	putCursor(c1.x, c1.y);
	scanf("%s", nickname);
	putCursor(c2.x, c2.y);
	scanf("%s", hostname);
}

int main(){
	int color = 1;
	char buffer [512];
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		perror("Invalid socket");
		exit(errno);
	}

	SOCKADDR_IN address = { 0 };
	char hostname [32];
	char nickname [32];
	

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
	while(strcmp(buffer, "READY") != 0){
		int n = 0;
		if((n=recv(sock, buffer, sizeof buffer -1, 0)) < 0){
			perror("RECV WELCOME MESSAGE ");
			exit(errno);
		}
		buffer[n] = '\0';
		if(n != 0)
			printf("<Message from server>\n %s \n", buffer);
	}
	
	return 0;
}
