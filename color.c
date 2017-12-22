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

// Clear the console.
void clear(){
	printf("\033[H\033[2J");
}

// Change the prompt color background or text
void setColor(int n){
	printf("\033[%dm",n);
}

// Makes the font normal
void normal(){
	printf("\033[0;0m");
}

// Makes the font bold
void bold(){
	printf("\033[01;37m");
}

// Write white Text over a blue background
void blue(){
	setColor(44);
	setColor(37);
}

// Write white Text over a blue background
void green(){
	setColor(42);
	setColor(37);
}

// Write white Text over a yellow background
void yellow(){
	setColor(43);
	setColor(37);
}

// Write white Text over a cyan background
void cyan(){
	setColor(46);
	setColor(37);
}

// Write red Text over a black background
void dark(){
	setColor(40);
	setColor(31);
}

// Write white Text over a red background
void red(){
	setColor(41);
	setColor(37);
}

// Write white Text over a Black background
void shadow(){
	setColor(40);
	setColor(37);
}

// Write white Text over a white background
void white(){
	setColor(47);
	setColor(37);
}

// IDK
void idk(){
	setColor(47);
	setColor(37);
}

void pink(){
	setColor(45);
	setColor(37);
}