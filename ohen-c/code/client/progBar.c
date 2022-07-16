#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "display.h"

// Unit test for the progression bar display.
int main(int argc, char ** argv){
	if(argc != 6){
		printf("BAD ARGUMENTS ! \nFORMAT : ./progBar [spacingLeft] [size] [part] [total] [message]\n");
		exit(0);
	}
	int spacingLeft = atoi(argv[1]);
	int size 		= atoi(argv[2]);
	int part 		= atoi(argv[3]);
	int total		= atoi(argv[4]);
	char * message 	= argv[5];
	Color colorFont = RED;
    Color colorBar 	= YELLOW;
	
	progBar(spacingLeft, message, size, part, total, colorFont, colorBar);
	
}