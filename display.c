#ifndef _stdio
#define _stdio
#include <stdio.h>
#endif
#include <string.h>
#include <stdlib.h>
#include "display.h"

// Put the console cursor at the x,y position.
void putCursor(int x, int y){
	char cmd [30] = "";
	sprintf(cmd, "tput cup %d %d", y, x);
	system(cmd);
}

// Display a progress bar with the given parameters.
void progBar(int spacingLeft, char * message, int barLenght, int size, int part, int total, int colorFont, int colorBar){
	
	int pourcVie, rest;
	
	Color resBar = DARK;
	
	printBlank(spacingLeft);
	initcolor(colorFont);
	printBlank(2);
	sprintf(message, "%s (%d/%d) : ", message, part, total);
	printf("%s", message);
	printBlank(2);
	
	pourcVie = (part / (float)total)*barLenght;
	initcolor(colorBar);
	printBlank(pourcVie);
	
	rest = barLenght - pourcVie;
	initcolor(resBar);	
	printBlank(rest);
	
	initcolor(colorFont);
	printBlank(2);
	printBlank(size-2-barLenght-4-strlen(message)-spacingLeft);

	printBlank(2);
	setColor(0);
	printf("\n");
}


// Print x blank space. 
void printBlank(int x){
	for(int i = 0; i < x; i++){
		printf(" ");
	}
}

// Print \n x times.
void emptyBlankLine(int x){
	while(x-- > 0)
		printf("\n");
}

// Print a shadow at the end of the line and go to the next.
void endLineShadow(){
	shadow();
	printf(" ");
	setColor(0);
	printf("\n");
}

// Print n lines with the chosen color and no shadow
void simpleLine(int n, int color, int spacingLeft, int size){
	while(n-- > 0){
		printBlank(spacingLeft);
		initcolor(color);
		printBlank(size);
		setColor(0);
		printf("\n");
	}
}

//Print n empty, colored, and shadowed line with the chosen color.
void emptyColoredShadowedLine(int n, int color, int spacingLeft, int size){
	for (int i = 0; i < n; i++){
		printBlank(spacingLeft);
		initcolor(color);
		printBlank(size);
		endLineShadow();
	}
}

// Print a shadowed line with the given message, color, spacing and size.
int messageLine(int a , char * chaine, int color, int spacingLeft, int size){
	int strLen = strlen(chaine);
	emptyColoredShadowedLine(1,color, spacingLeft, size);
	printBlank(spacingLeft);
	bold();
	initcolor(color);
	if(a > 0){
		printBlank(spacingLeft);
		printf("%d) %s",a,chaine);
		printBlank(size - strLen - spacingLeft - 2 - strlen(a + ""));
	}
	else{
		printBlank(spacingLeft);
		printf("%s",chaine);
		printBlank(size - strLen - spacingLeft);
	}
	endLineShadow();
	emptyColoredShadowedLine(1,color, spacingLeft, size);
	normal();
	return ++a;
}

// Print a line with the given message, followed by a blank space of size blankSize, with the given size. 
// Return the x position of the cursor.
int inputLine(char * chaine, int color, int blankColor, int blankSize, int spacingLeft, int size){
	int strLen = strlen(chaine);
	emptyColoredShadowedLine(1, color, spacingLeft, size);
	printBlank(spacingLeft);
	bold();
	initcolor(color);
	printBlank(4);
	printf("%s",chaine);
	printBlank(4);
	
	initBlankColor(blankColor);
	printBlank(blankSize);
	
	setColor(0);
	initcolor(color);
	printBlank(size - blankSize - 8- strLen);
	
	shadow();
	printBlank(1);
	setColor(0);
	printf("\n");
	emptyColoredShadowedLine(1, color, spacingLeft, size);
	normal();
	return spacingLeft + 4 + strLen + 4;
}

// Print the last lines of a menu with a shadow under.
void lastLine(int n, int color, int spacingLeft, int size){
	for (int i = 0; i < n; i++){
		emptyColoredShadowedLine(1, color, spacingLeft, size);
	}
	printBlank(spacingLeft + 1);
	shadow();
	printBlank(size);
	setColor(0);
	printf("\n");
}

// Activate the chosen color.
void initcolor(Color color)
{
	switch(color){
		case BLUE : 
			blue();
			break;
		case GREEN : 
			green();
			break;
		case DARK : 
			dark();
			break;
		case YELLOW : 
			yellow();
			break;
		case RED :
			red();
			break;
		case PINK :
			pink();
			break;
		case IDK :
			idk();
			break;
	}
}

void initBlankColor(Color color){
	switch(color){
		case BLUE : 
			setColor(47);
			break;
		case GREEN : 
			setColor(47);
			break;
		case DARK : 
			setColor(47);
			break;
		case YELLOW : 
			setColor(47);
			break;
		case RED :
			setColor(47);
			break;
		case PINK :
			setColor(47);
			break;
		default :
			setColor(47);
			break;
	}
}