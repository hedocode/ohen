#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "display.h"
#include "../lib/core.h"

// Put the console cursor at the x,y position.
void putCursor(int x, int y){ //printf(27 91 65)
	char cmd [30] = "";
	sprintf(cmd, "tput cup %d %d", y, x);
	system(cmd);
}

// Display a progress bar with the given parameters.
void progBar(int spacingLeft, char * message, int size, int part, int total, int colorFont, int colorBar){
	
	int pourcVie, rest;
	
	Color resBar = DARK;
	
	printBlank(spacingLeft);
	initcolor(colorFont);
	printBlank(2);
	sprintf(message, "%s (%d/%d) : ", message, part, total);
	printf("%s", message);
	printBlank(2);
	
	
	int barLenght = size-strlen(message)-spacingLeft-4;
	
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
	endLineShadow();
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

// Begin a menu
void beginMenu(int color, int spacingLeft, int size){
	clear();
	emptyBlankLine(4);
	simpleLine(1,color, spacingLeft, size);
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
		printBlank(size - strLen - spacingLeft - 3);
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
	
	endLineShadow();
	emptyColoredShadowedLine(1, color, spacingLeft, size);
	normal();
	return spacingLeft + 4 + strLen + 5;
}

// Print the last lines of a menu with a shadow under.
void endMenu(int n, int color, int spacingLeft, int size){
	for (int i = 0; i < n; i++){
		emptyColoredShadowedLine(1, color, spacingLeft, size);
	}
	printBlank(spacingLeft + 1);
	endLineShadow();
}

// Display the end of the menu with a given notification message
void endMenuNotif(int color, char * message, int spacingLeft, int size){
	int len = strlen(message);
	emptyColoredShadowedLine(1, color, spacingLeft, size);
	
	//Print first notifLine with menu shadow
	printBlank(spacingLeft+3);
	shadow();
	printBlank(size-spacingLeft-len-1);
	red();
	printBlank(len+2);
	endLineShadow();
	
	// Print second line with message
	printBlank(size-len+2);
	red();
	printf(" %s ",message);
	endLineShadow();
	
	//Print red notif line with shadow
	printBlank(size-len+2);
	red();
	printBlank(len+2);
	endLineShadow();
	
	// Print Shadow under the notification bar.
	printBlank(size-len+1);
	shadow();
	printBlank(size-len-1);
	shadow();
	printBlank(len-1);
	endLineShadow();
}

// Print a shadow and go to the next line.
void endLineShadow(){
	shadow();
	printBlank(1);
	setColor(0);
	printf("\n");
}

char * statusToString(Status s){
	switch(s){
		case GENERATE:
			return "GENERATE";
			break;
		case ATTACK:
			return "ATTACK";
			break;
		case DEFENSE:
			return "DEFENSE";
			break;
		default:
			return "UNKNOWN STATUS";
			break;
	}
}