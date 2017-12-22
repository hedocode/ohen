void askServerToGenerate();
void askServerToDefend();
void askServerToAttack(int targetId);
int displayMainMenu(int color, int spacingLeft, int size, char * notifMessage);
int displayGameInfos(int color, int spacingLeft, int size, Player ** gameInfo, int playerCount);
char * statusToString(Status s);
char* getNextString(char * str);