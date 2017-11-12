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

void clear(void)
{
	printf("\033[H\033[2J");
}

void color(int n)
{
	printf("\033[%dm",n);
}

void normal(void)
{
	printf("\033[0;0m");
}

void bold(void)
{
	printf("\033[01;37m");
}

void blue(void)
{
	color(44);
	color(37);
}

void green(void)
{
	color(42);
	color(37);
}

void yellow(void)
{
	color(43);
	color(37);
}

void cyan(void)
{
	color(46);
	color(37);
}

void dark(void)
{
	color(40);
	color(31);
}

void red(void)
{
	color(41);
	color(37);
}

void shadow(void)
{
	color(40);
	color(30);
}