#include <stdio.h>
//parametres
/*	
	si x=3 texte, si x=4 fond
	x0=Noir
	x1=Rouge
	x2=Vert
	x3=Jaune
	x4=Bleu
	x5=Magenta
	x6=Cyan
	x7=Blanc
*/

void clear(){
	printf("\033[H\033[2J");
}

void setColor(int n){
	printf("\033[%dm",n);
}

void normal(){
	printf("\033[0;0m");
}

void bold(){
	printf("\033[01;37m");
}

void blue(){
	setColor(44);
	setColor(37);
}

void green(){
	setColor(42);
	setColor(37);
}

void yellow(){
	setColor(43);
	setColor(37);
}

void cyan(){
	setColor(46);
	setColor(37);
}

void dark(){
	setColor(40);
	setColor(31);
}

void red(){
	setColor(41);
	setColor(37);
}

void shadow(){
	setColor(40);
	setColor(30);
}

void white(){
	setColor(47);
	setColor(37);
}

void idk(){
	setColor(48);
	setColor(37);
}