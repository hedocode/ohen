#include <stdio.h>
#include "color.h"

// Parametres
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
	Examples : 
		-34 changera la couleur du texte en Bleu.
		-47 changera la couleur de fonc en Blanc.
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

// Write black Text over a yellow background
void yellow(){
	setColor(43);
	setColor(30);
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

// Write white Text over a pink background
void pink(){
	setColor(45);
	setColor(37);
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
		case WHITE:
			white();
			break;
		default:
			white();
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