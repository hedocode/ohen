#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "player.h"
#include "socket.h"

int main(){
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		perror("Invalid socket");
		exit(errno);
	}		

	SOCKADDR_IN sin = { 0 };
	char * hostname = malloc(sizeof(char *));
	printf("hostname : ");
	scanf("%s", hostname);

	struct hostent * hostinfo = gethostbyname(hostname);

	if(hostinfo == NULL){
		fprintf(stderr, "Unknown host \"%s\".\n", hostname);
		exit(EXIT_FAILURE);
	}

	sin.sin_addr = *(IN_ADDR *) hostinfo -> h_addr;
	sin.sin_port = htons(PORT);
	sin.sin_family = AF_INET;

	if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == -1){
		perror("Connexion error");
		exit(errno);
	}

	return 0;
}
