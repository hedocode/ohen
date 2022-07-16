#ifndef _color
#define _color
typedef enum{
	BLUE,
	GREEN,
	DARK,
	YELLOW,
	RED,
	PINK,
	WHITE,
	IDK
}color;

typedef color Color;

void clear();
void setColor(int n);
void normal();
void bold();

void blue();
void green();
void yellow();
void cyan();
void dark();
void red();
void shadow();
void pink();
void idk();
void white();
void initcolor(Color color);
void initBlankColor(Color color);
#endif