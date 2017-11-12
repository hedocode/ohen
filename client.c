#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "socket.h"

int main(){
	char buffer [512];
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		perror("Invalid socket");
		exit(errno);
	}

	SOCKADDR_IN address = { 0 };
	char hostname [32];
	char nickname [32];
	printf("Nickname : ");
	scanf("%s", nickname);
	printf("Hostname : ");
	scanf("%s", hostname);

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
