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


void askServerToGenerate();
void askServerToDefend();
void askServerToAttack(int targetId);
int	displayMainMenu(int color, int spacingLeft, int size, char * notifMessage);
int displayGameInfos(int color, int spacingLeft, int size, Player ** gameInfo, int playerCount);
char * statusToString(Status s);
char* getNextString(char * str);

#endif