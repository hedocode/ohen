#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include "display.h"
#include "player.h"
#include "socket.h"
#include "clientThread.h"
#include "client.h"

int sock;

// Close the game properly
void closeGame(){
	askServerToDisconnect();
	close(sock);
}

//Send the char array str to ther server throught the socket given as global parameter.
void sendMessageToServer(char * str){
	if(send(sock, str, strlen(str), 0) < 0){
		perror("SEND ERROR : ");
		exit(errno);
	}
}

//Send a message to the server to make this client generating ohen.
void askServerToGenerate(){
	sendMessageToServer("generate");
}
	
// Send a message to the server to make this client defend himself.
void askServerToDefend(){
	sendMessageToServer("defend");
}

// Send a message to the server to make this client attack an other.
void askServerToAttack(int targetId){
	char cmd[64];
	sprintf(cmd,"%s %d","attack",targetId);
	sendMessageToServer(cmd);
}

// Send a message to the server to make this client disconnect.
void askServerToDisconnect(){
	sendMessageToServer("disconnect");
}

// Send a message to the server to make this client upgrade his ohen regen.
void askServerToUpgradeOhenRegen(){
	sendMessageToServer("inc_ohen_regen");
}

// Send a message to the server to make this client attack an other.
void askServerToUpgradeAttack(){
	sendMessageToServer("inc_attack");
}

// Send a message to the server to make this client defending.
void askServerToUpgradeDefense(){
	sendMessageToServer("inc_defence");
}

// Parsing a char array and getting the first Player data from it
char * getNextPlayer(char * str){
	char * buffer = malloc(sizeof(char[256]));
	int len = strlen(str);
	
	while(str[0] != ';'){
		sprintf(buffer,"%s%c",buffer,str[0]);
		for(int i = 0; i<len;i++){
			str[i] = str[i+1];
		}
		str[len] = '\0';
		len--;
	}
	for(int i = 0; i<len;i++){
			str[i] = str[i+1];
		}
		str[len] = '\0';
		len--;
	return buffer;
}

// Convert a char array into a Player struct
Player deserializePlayer(char * str){
	Player p;
	p.id 			= getNextNumber(str);
	p.nickname 		= getNextString(str);
	p.ohen 			= getNextNumber(str);
	p.max_ohen 		= getNextNumber(str);
	p.shield 		= getNextNumber(str);
	p.attack_damage = getNextNumber(str);
	p.defense 		= getNextNumber(str);
	p.health 		= getNextNumber(str);
	p.max_health 	= getNextNumber(str);
	p.regen_ohen 	= getNextNumber(str);
	p.regen_health 	= getNextNumber(str);
	p.dead 			= getNextNumber(str);
	p.targetId		= getNextNumber(str);
	p.state 		= getNextNumber(str);
	return p;
}

// Convert the char array that is received by the server into a player array.
Player * deserializePlayers(char * charr, int playerCount){
	Player * players = malloc(sizeof(Player)*playerCount);
	for(int i = 0; i < playerCount; i++){
		char * player = getNextPlayer(charr);
		players[i] = deserializePlayer(player);
	}
	return players;
}

// Client thread code that listen the game info changes from the server.
void * listenFromServer(void * args){
	int n = 0;
	ListenServerThreadArgs * arg = (ListenServerThreadArgs *) args;	
	if(arg == NULL){
		perror("Thread argument is not a pointer on a ListenThreadArgs struct"); 
		exit(3);
	}
	char * gameInfo = malloc(sizeof(char[256*arg->clientCount]));
	while(1){
		if((n = recv(arg->sock, gameInfo, sizeof(char[256*arg->clientCount]), 0)) < 0){
			perror("RECV GAMEINFO ERROR ");
			exit(errno);
		}
		if(n != 0){
			char * cpy = malloc(sizeof(char[256*arg->clientCount]));
			strcpy(cpy, gameInfo);
			char * word = getNextString(cpy) ;
			if(word[0]-'0' > 9 || word[0]-'0' < 0){
				if(strcmp(word,"GOSU") == 0){
					int winnerIndex = string2unsignedint(getNextString(cpy));
					fprintf(stderr,"%s won by supremacy !", arg->playersGameInfo[0][winnerIndex].nickname);
					scanf("%s",word);
					exit(1);
				}
				else if(strcmp(word, "GOKO") == 0){
					int winnerIndex = string2unsignedint(getNextString(cpy));
					fprintf(stderr,"%s won by K.O !", arg->playersGameInfo[0][winnerIndex].nickname);
					scanf("%s",word);
					exit(1);
				}
				else if(strcmp(word, "KO") == 0){
					*arg->dead = true;
				}
				else{
					//int len = strlen(word);
					//printf("strlen(word) = %d\n", len);
					//printf("%s \n%s\n", word, cpy);
				}
			}
			else
				arg->playersGameInfo[0] = deserializePlayers(gameInfo, arg->clientCount);
		}
	}
}

// Display the connexion menu.
int menuConnexion(int color, int spacingLeft, int size, char * nickname, char * hostname, char * notifMessage){
	Coords c1;
	Coords c2;
	beginMenu(color, spacingLeft, size);
	messageLine(0, "CONNEXION :", color, spacingLeft, size);
	emptyColoredShadowedLine(1, color, spacingLeft, size);
	c1.x = inputLine("Nickname :", color, 7, size-10-spacingLeft-8, spacingLeft, size);
	c1.y = 10;
	emptyColoredShadowedLine(1, color, spacingLeft, size);
	c2.x = inputLine("Hostname :", color, 7, size-10-spacingLeft-8, spacingLeft, size);
	c2.y = c1.y+4;
	emptyColoredShadowedLine(1, color, spacingLeft, size);
	endMenu(3, color, spacingLeft, size);
	setColor(47);
	setColor(30);
	putCursor(c1.x, c1.y);
	scanf("%s", nickname);
	putCursor(c2.x, c2.y);
	scanf("%s", hostname);
	setColor(0);
	return MAIN;
}

// Display the infos on Players 
int displayGameInfos(int color, int spacingLeft, int size, Player ** gameInfo, int playerCount){
	char buffer [256];
	while(strcmp(buffer,"exit") != 0){
		beginMenu(color, spacingLeft, size);
		for(int i = 0; i < playerCount; i++){
			char message[64] = "";
			sprintf(message,"%d) %s's Ohen", i, gameInfo[0][i].nickname);
			Color c;
			if((Color)color != YELLOW)
				c = YELLOW;
			else
				c = BLUE;
			progBar(spacingLeft, message, size, gameInfo[0][i].ohen, gameInfo[0][i].max_ohen, color, c);
			sprintf(message,"%d) %s's Health", i, gameInfo[0][i].nickname);
			emptyColoredShadowedLine(1, color, spacingLeft, size);
			if((Color)color != GREEN)
				c = GREEN;
			else
				c = BLUE;
			progBar(spacingLeft, message, size, gameInfo[0][i].health, gameInfo[0][i].max_health, color, c);
			sprintf(message,"%d) %s's state : %s - GEN : %d | ATK : %d | DEF : %d", i, gameInfo[0][i].nickname, statusToString(gameInfo[0][i].state), gameInfo[0][i].regen_ohen, gameInfo[0][i].attack_damage, gameInfo[0][i].defense);
			messageLine(-1, message, color, spacingLeft, size);
		}
		endMenuNotif(color, "Type anything to update or exit", spacingLeft, size);
		scanf("%s",buffer);
	}
	return MAIN;
}
	
// Display the Menu where you can choose actions to do.
int displayActionChoice(int color, char * notifMessage, int spacingLeft, int size){
	char message[256];
	int a = 1;
	beginMenu(color, spacingLeft, size);
	a = messageLine(a, "Generate", color, spacingLeft, size);
	a = messageLine(a, "Defend", color, spacingLeft, size);
	a = messageLine(a, "Attack", color, spacingLeft, size);
	strlen(notifMessage) == 0 ? 
		endMenu(2, color, spacingLeft, size):
		endMenuNotif(color, notifMessage, spacingLeft, size);
	
	scanf("%s", message);
	if(strcmp(message,"1") == 0 || strcmp(message,"generate") == 0){
		askServerToGenerate();
		emptyString(notifMessage);
		return MAIN;
	}
	else if(strcmp(message,"2") == 0 || strcmp(message,"defend") == 0){
		askServerToDefend();
		emptyString(notifMessage);
		return MAIN;
	}
	else if(strcmp(message,"3") == 0 || strcmp(message,"attack") == 0){
		int targetId = 0;
		printf("Id of the target ? -> ");
		scanf("%d",&targetId);
		askServerToAttack(targetId);
		emptyString(notifMessage);
		return MAIN;
	}
	else if(strcmp(message,"exit") == 0){
		emptyString(notifMessage);
		return MAIN;
	}
	else{
		sprintf(notifMessage,"%s isn't a correct cmd.", message);
		return DISPLAY_ACTION_CHOICE;
	}
}

// Display the Intro Menu that allows you to learn how to play and customize your client.
int displayIntroMenu(int color, int spacingLeft, int size, char * notifMessage){
	char message[256] = "";
	int a = 1;
	beginMenu(color, spacingLeft, size);
	a = messageLine(a, "Connect to a server", color, spacingLeft, size);
	a = messageLine(a, "Customize", color, spacingLeft, size);
	a = messageLine(a, "Tutorial", color, spacingLeft, size);
	
	strlen(notifMessage) == 0 ? 
		endMenu(2, color, spacingLeft, size):
		endMenuNotif(color, notifMessage, spacingLeft, size);
	
	scanf("%s", message);
	if(strcmp(message,"1") == 0 || strcmp(message,"connect") == 0){
		emptyString(notifMessage);
		return CONNEXION;
	}
	else if(strcmp(message,"2") == 0 || strcmp(message,"custom") == 0){
		emptyString(notifMessage);
		return CUSTOMIZATION;
	}
	else if(strcmp(message,"3") == 0 || strcmp(message,"upgrade") == 0){
		emptyString(notifMessage);
		return TUTORIAL;
	}
	else if(strcmp(message,"exit") == 0){
		emptyString(notifMessage);
		askServerToDisconnect();
		close(sock);
		exit(0);
	}
	else{
		char notifMessage[256];
		sprintf(notifMessage,"%s isn't a correct cmd.", message);
		return INTRO;
	}
}

int tutorial(int color, int spacingLeft, int size, char * notifMessage){
	char message[256] = "";
	beginMenu(color, spacingLeft, size);
	int a = -1;
	messageLine(a, "Each player have ohen and life.", color, spacingLeft, size);
	messageLine(a, "If someone fill his ohen bar, we win.", color, spacingLeft, size);
	messageLine(a, "If someone life falls to zero, he dies.", color, spacingLeft, size);
	messageLine(a, "The last person alive win.", color, spacingLeft, size);
	messageLine(a, "You can choose between generating, attacking, and defending.", color, spacingLeft, size);
	messageLine(a, "You can also buy some upgrades.", color, spacingLeft, size);
	strlen(notifMessage) == 0 ? 
		endMenu(2, color, spacingLeft, size):
		endMenuNotif(color, notifMessage, spacingLeft, size);
	
	scanf("%s", message);
	return INTRO;
}

int displayMainMenu(int color, int spacingLeft, int size, char * notifMessage){
	char message[256] = "";
	int a = 1;
	beginMenu(color, spacingLeft, size);
	a = messageLine(a, "See Player stats", color, spacingLeft, size);
	a = messageLine(a, "Choose Action", color, spacingLeft, size);
	a = messageLine(a, "Upgrades", color, spacingLeft, size);
	a = messageLine(a, "Custom Menu", color, spacingLeft, size);

	strlen(notifMessage) == 0 ? 
		endMenu(2, color, spacingLeft, size):
		endMenuNotif(color, notifMessage, spacingLeft, size);
	
	scanf("%s", message);
	if(strcmp(message,"1") == 0 || strcmp(message,"see") == 0){
		emptyString(notifMessage);
		return DISPLAY_GAME_INFO;
	}
	else if(strcmp(message,"2") == 0 || strcmp(message,"choose") == 0){
		emptyString(notifMessage);
		return DISPLAY_ACTION_CHOICE;
	}
	else if(strcmp(message,"3") == 0 || strcmp(message,"upgrade") == 0){
		emptyString(notifMessage);
		return DISPLAY_UPGRADES;
	}
	else if(strcmp(message,"4") == 0 || strcmp(message,"custom") == 0){
		emptyString(notifMessage);
		return CUSTOMIZATION;
	}
	else if(strcmp(message,"exit") == 0){
		emptyString(notifMessage);
		askServerToDisconnect();
		close(sock);
		exit(0);
	}
	else{
		sprintf(notifMessage,"%s isn't a correct cmd.", message);
		return MAIN;
	}
}

int displayUpgradesMenu(int color, int spacingLeft, int size, char * notifMessage, Player player){
	char message[256] = "";
	int a = -1;
	beginMenu(color, spacingLeft, size);
	sprintf(message, "You have %d ohen.", player.ohen);
	messageLine(a, message, color, spacingLeft, size);
	a = 1;
	sprintf(message, "Increase Ohen Regen - %d ohen", player.regen_ohen*10);
	a = messageLine(a, message, color, spacingLeft, size);
	sprintf(message, "Increase Attack - %d ohen", player.attack_damage*9);
	a = messageLine(a, message, color, spacingLeft, size);
	sprintf(message, "Increase Defence - %d ohen", player.defense*8);
	a = messageLine(a, message, color, spacingLeft, size);
	strlen(notifMessage) == 0 ? 
		endMenu(2, color, spacingLeft, size):
		endMenuNotif(color, notifMessage, spacingLeft, size);
	scanf("%s", message);
	if(strcmp(message,"1") == 0 || strcmp(message,"ohenregen") == 0){
		askServerToUpgradeOhenRegen();
		emptyString(notifMessage);
		return MAIN;
	}
	else if(strcmp(message,"2") == 0 || strcmp(message,"attack") == 0){
		askServerToUpgradeAttack();
		emptyString(notifMessage);
		return MAIN;
	}
	else if(strcmp(message,"3") == 0 || strcmp(message,"defence") == 0){
		askServerToUpgradeDefense();
		emptyString(notifMessage);
		return MAIN;
	}
	else if(strcmp(message,"exit") == 0 || strcmp(message,"return") == 0){
		emptyString(notifMessage);
		return MAIN;
	}
	else{
		sprintf(notifMessage, "Unknown command : %s", message);
		return DISPLAY_UPGRADES;
	}
}

int persoMenu(Color * color, int spacingLeft, int * size, char * notifMessage, Menu menu){
	char message[256] = "";
	int a = 1;
	beginMenu(*color, spacingLeft, *size);
	a = messageLine(a, "Change Color", *color, spacingLeft, *size);
	a = messageLine(a, "Change Window Width", *color, spacingLeft, *size);
	strlen(notifMessage) == 0 ? 
		endMenu(2, *color, spacingLeft, *size):
		endMenuNotif(*color, notifMessage, spacingLeft, *size);
	scanf("%s", message);
	if(strcmp(message, "color") == 0 || strcmp(message, "1") == 0){
		printf("Wanted color : ");
		scanf("%s", message);
		int arg = string2unsignedint(message);
		if(arg != -1){
			*color = arg;
			emptyString(notifMessage);
			return menu;
		}
		else if(strcmp(message,"red") == 0){
			*color = RED;
			emptyString(notifMessage);
			return menu;
		}
		else if(strcmp(message,"blue") == 0){
			*color = BLUE;
			emptyString(notifMessage);
			return menu;
		}
		else if(strcmp(message,"yellow") == 0){
			*color = YELLOW;
			emptyString(notifMessage);
			return menu;
		}
		else if(strcmp(message,"green") == 0){
			*color = GREEN;
			emptyString(notifMessage);
			return menu;
		}
		else if(strcmp(message,"pink") == 0){
			*color = PINK;
			emptyString(notifMessage);
			return menu;
		}
		else{
			sprintf(notifMessage, "Second arg isn't a number : %s", message);
			return menu;
		}
	}
	else if(strcmp(message,"size") == 0 || strcmp(message,"2") == 0){
		printf("Wanted size : ");
		scanf("%s", message);
		int arg = string2unsignedint(message);
		if(arg != -1 && arg >= 70){
			*size = arg;
			emptyString(notifMessage);
			return menu;
		}
		else{
			sprintf(notifMessage, "Second arg is invalid : %s", message);
			return menu;
		}
	}
	else{
		sprintf(notifMessage, "Invalid CMD : %s", message);
		return CUSTOMIZATION;
	}
	return menu;
}

void displayGameOver(int color, int spacingLeft, int size, char * message){
	char bin [32];
	beginMenu(color, spacingLeft, size);
	bold();
	messageLine(-1, "G A M E  O V E R", color, spacingLeft, size);
	messageLine(-1, message, color, spacingLeft, size);
	normal();
	endMenu(2, color, spacingLeft, size);
	scanf("%s", bin);
}

int main(){
	signal(SIGINT, closeGame);
	pthread_t listenServer;
	Color color = RED;
	int spacingLeft = 4;
	int size = 80;
	char buffer [512];
	char notifMessage [256] = "";
	int nextMenuId = INTRO;
	bool dead = false;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		perror("Invalid socket");
		exit(errno);
	}

	SOCKADDR_IN address = { 0 };
	char hostname [32];
	char nickname [32];
	
	Player * playersGameInfo;
	int clientCount = 0;
	bool start = false;
	
	// Print the connexion menu.
	while(!start){
		switch(nextMenuId){
			case INTRO:
				nextMenuId = displayIntroMenu(color, spacingLeft, size, notifMessage);
				break;
			case CUSTOMIZATION:
				nextMenuId = persoMenu(&color, spacingLeft, &size, notifMessage, nextMenuId);
				break;
			case CONNEXION:
				nextMenuId = menuConnexion(color, spacingLeft, size, nickname, hostname, notifMessage);
				break;
			case TUTORIAL:
				nextMenuId = tutorial(color, spacingLeft, size, notifMessage);
				break;
			default:
				start = true;
				break;
		}
	}

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
	sendMessageToServer(nickname);
	
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
	while(clientCount == 0){
		int r = 0;
		if((r = recv(sock, &clientCount, sizeof clientCount, 0)) < 0){
			perror("RECV CLIENT COUNT ERROR ");
			exit(errno);
		}
		clientCount -= 48;
		if(r != 0)
			printf("Number of players on the server : %d \n", clientCount);
		
	}
	
	playersGameInfo = malloc(sizeof(Player) * clientCount);
	
	// Creation du thread qui va recevoir les infos du serveur.
	ListenServerThreadArgs args;
	args.playersGameInfo = &playersGameInfo;
	args.sock = sock;
	args.clientCount = clientCount;
	args.dead = &dead;
	if(pthread_create(&listenServer, NULL, listenFromServer, &args) == -1) {
		perror("pthread_create");
		return -1;
    }
	Player me;
	while(!dead){
		switch(nextMenuId){
			case MAIN:
				nextMenuId = displayMainMenu(color, spacingLeft, size, notifMessage);
				break;
			case DISPLAY_GAME_INFO:
				nextMenuId = displayGameInfos(color, spacingLeft, size, &playersGameInfo, clientCount);
				break;
			case DISPLAY_ACTION_CHOICE:
				nextMenuId = displayActionChoice(color, notifMessage, spacingLeft, size);
				break;
			case DISPLAY_UPGRADES:
				for(int i = 0; i < clientCount; i++){
					if(strcmp(playersGameInfo[i].nickname, nickname) == 0){
						me = playersGameInfo[i];
					}
				}
				nextMenuId = displayUpgradesMenu(color, spacingLeft, size, notifMessage, me);
				break;
			case CUSTOMIZATION:
				nextMenuId = persoMenu(&color, spacingLeft, &size, notifMessage, nextMenuId);
				break;
			case TUTORIAL:
				nextMenuId = tutorial(color, spacingLeft, size, notifMessage);
				break;
			case INTRO:
				nextMenuId = displayIntroMenu(color, spacingLeft, size, notifMessage);
				break;
			default:
				nextMenuId = displayMainMenu(color, spacingLeft, size, notifMessage);
		}
	}
	displayGameOver(color, spacingLeft, size, "YOU GOT KILLED");
	askServerToDisconnect();
	close(sock);
	return 0;
}