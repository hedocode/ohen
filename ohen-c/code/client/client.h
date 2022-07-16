#ifndef _client
#define _client

typedef enum{
	MAIN,
	DISPLAY_GAME_INFO,
	DISPLAY_ACTION_CHOICE,
	DISPLAY_UPGRADES,
	CUSTOMIZATION,
	TUTORIAL,
	INTRO,
	CONNEXION
} Menu;

void sendMessageToServer(char * str);
void askServerToGenerate();
void askServerToDefend();
void askServerToAttack(int targetId);
void askServerToDisconnect();
void askServerToUpgradeOhenRegen();
void askServerToUpgradeAttack();
void askServerToUpgradeDefense();
char * getNextPlayer(char * str);
Player deserializePlayer(char * str);
Player * deserializePlayers(char * charr, int playerCount);
void * listenFromServer(void * args);
int menuConnexion(int color, int spacingLeft, int size, char * nickname, char * hostname, char * notifMessage);
int	displayMainMenu(int color, int spacingLeft, int size, char * notifMessage);
int displayGameInfos(int color, int spacingLeft, int size, Player ** gameInfo, int playerCount);
char * statusToString(Status s);
char* getNextString(char * str);

#endif