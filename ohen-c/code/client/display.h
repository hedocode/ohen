#ifndef _display
#define _display
#include "color.h"

void putCursor(int x, int y);
void printBlank(int x);
void emptyBlankLine(int x);
void endLineShadow();
void simpleLine(int n, int color, int spacingLeft, int size);
void emptyColoredShadowedLine(int n, int color, int spacingLeft, int size);
int messageLine(int a , char * chaine, int color, int spacingLeft, int size);
int inputLine(char * chaine, int color, int blankColor, int blankSize, int spacingLeft, int size);
void endMenu(int n, int color, int spacingLeft, int size);
void initcolor(Color color);
void initBlankColor(Color color);
void progBar(int spacingLeft, char* message, int size, int part, int total, int colorFont, int colorBar);
void beginMenu(int color, int spacingLeft, int size);
void endMenuNotif(int color, char * message, int spacingLeft, int size);
void endLineShadow();
#endif