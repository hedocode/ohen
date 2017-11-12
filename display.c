#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "display.h"

// Pur the console cursor at the x,y position.
void putCursor(int x, int y){
	char cmd [30] = "";
	sprintf(cmd, "tput cup %d %d", y, x);
	system(cmd);
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
	setColor(blankColor);
	setColor(45); //TOREMOBE
	setColor(35); //TOREMOVE
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
void initcolor(int color)
{
	if (color == 1) blue();
	if (color == 2) green();
	if (color == 3) dark();
	if (color == 4) yellow();
}