#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "core.h"

// Convert a char array into an unsigned int.
int string2unsignedint(char * stringNumber) {
	int number = 0;
	int i = 0;
	
	int stringLength = strlen(stringNumber);

	while (i < stringLength) {
		int digit = stringNumber[i++] - '0';
		if (digit < 0 || digit > 9) {
		  printf("Invalid character '%c' on the position '%d'\n", stringNumber[i - 1],(i - 1));
		  return -1;
		}
		number *= 10;
		number += digit;
	}
	return number;
}

// Parsing a char array and getting the next number from it.
int getNextNumber(char * str){
	return string2unsignedint(getNextString(str));
}

// Parsing a char array and getting the next number from it.
char * getNextString(char * str){
	char * buffer = malloc(sizeof(char)*8);
	int len = strlen(str);
	while(str[0]!= '/' && str[0] != 0 && str[0] != 32){
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
	return buffer;
}