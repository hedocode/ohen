#ifndef _color
#define _color
#include "color.h"
#endif

void putCursor(int x, int y);
void printBlank(int x);
void emptyBlankLine(int x);
void endLineShadow();
void simpleLine(int n, int color, int spacingLeft, int size);
void emptyColoredShadowedLine(int n, int color, int spacingLeft, int size);
int messageLine(int a , char * chaine, int color, int spacingLeft, int size);
int inputLine(char * chaine, int color, int blankColor, int blankSize, int spacingLeft, int size);
void lastLine(int n, int color, int spacingLeft, int size);
void initcolor(Color color);
void initBlankColor(Color color);
void progBar(int spacingLeft, char* message, int barLenght, int size, int part, int total, int colorFont, int colorBar);