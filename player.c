#include <stdlib.h>
#include <stdio.h>
#include "player.h"

// Player "Constructor" Initialize values at default.
Player * createPlayer(int id){
	Player * p = malloc(sizeof(Player *));
	p->id 			= id;
	p->ohen 		= 0;
	p->max_ohen 	= 1000;
	p->shield 		= 0;
	p->attack_damage= 1;
	p->defense 		= 1;
	p->health 		= 100;
	p->max_health 	= 100;
	p->regen_ohen 	= 10;
	p->regen_health = 1;
	p->vulnerable 	= true;
	p->state 		= GENERATE;
	return p;
}

// Convert the Player p structure into an array of integers and returns it.
int * player2IntArray(Player p){
	int * arr = malloc(sizeof(int)*11);
	arr[0] = p.ohen;
	arr[1] = p.max_ohen;
	arr[2] = p.shield;
	arr[3] = p.attack_damage;
	arr[4] = p.defense;
	arr[5] = p.health;
	arr[6] = p.max_health;
	arr[7] = p.regen_ohen;
	arr[8] = p.regen_health;
	arr[9] = p.vulnerable;
	arr[10] = p.state;
	return arr;
}

// Convert the int array arr into a Player p structure and returns it.
Player intArray2Player(int * arr){
	Player p;
	p.ohen 			= arr[0];
	p.max_ohen 		= arr[1];
	p.shield 		= arr[2];
	p.attack_damage = arr[3];
	p.defense 		= arr[4];
	p.health 		= arr[5];
	p.max_health 	= arr[6];
	p.regen_ohen 	= arr[7];
	p.regen_health 	= arr[8];
	p.vulnerable 	= arr[9];
	p.state 		= arr[10];
	return p;
}

// Convert an int array intarr, into a char array chararr and returns it.
char * serializeIntArray(int * intarr){
	char * chararr = malloc(sizeof(char[256]));
	for(int i = 0; i < 11; i++){
		sprintf(chararr,"%s%d/", chararr,intarr[i]);
	}
	return chararr;
}

// Convert a Player into a char array of char array that can be sent over a socket.
char * serializePlayer(Player p){
	return serializeIntArray(player2IntArray(p));
}

// Convert an array of Players into an Array of array of char.
char * serializePlayers(Player ** p, int playerCount){
	char * players = malloc(sizeof(char[256*playerCount]));
	for(int i = 0; i < playerCount; i++){
		sprintf(players,"%s%s/", players,serializePlayer(*p[i]));
	}
	return players;
}

// Makes a Player p generating some ohen depending on his ohen regeneration.
void generate(Player * player){
	if(player->ohen + player->regen_ohen <= player->max_ohen)
		player->ohen += player->regen_ohen;
}

// Makes a Player "attacker" attack a Player "defender"
void attack(Player * attacker, Player * defender){
	int damages;
	if(defender->state == DEFENSE){
		if(defender->defense < attacker->attack_damage){
			damages = attacker->attack_damage-defender->defense;
			if(defender->shield > 0){
				if(defender->shield > damages){
					defender->shield -= damages;
				}
				else{
					defender->shield = 0;
					damages -= defender->shield;
					defender->health -= damages;
				}
			}
			else{
				defender->health -= damages;
			}
		}
		else{
			attacker->health -= (defender->defense - attacker->attack_damage)/2;
		}
	}
	else{
		damages = attacker->attack_damage;
		if(defender->shield > 0){
			if(defender->shield > damages)
				defender->shield -= damages;
			else{
				defender->shield = 0;
				damages -= defender->shield;
				defender->health -= damages;
			}
		}
		else{
			defender->health -= attacker->attack_damage;
		}
	}
}

// Makes the defender regen twice.
void defend(Player * defender){
	defender->health += defender->regen_health;
}

// Upgrade the Attack Damage of the Player p
void upgradeAttackDamage(Player * p){
	int price = 50*p->attack_damage;
	if(p->ohen >= price){
		p->ohen -= price;
		p->attack_damage++;
	}
}

// Upgrade the Ohen Regeneration of the Player p
void upgradeRegenOhen(Player * p){
	int price = 20*p->regen_ohen;
	if(p->ohen >= price){
		p->ohen -= price;
		p->regen_ohen++;
	}
}

// Upgrade the Defense of the Player p.
void upgradeDefense(Player * p){
	int price = 60*p->defense;
	if(p->ohen >= price){
		p->ohen -= price;
		p->defense++;
	}
}

//Upgrade Health regeneration of the Player p.
void upgradeHealthRegen(Player * p){
	int price = 20*p->regen_health;
	if(p->ohen >= price){
		p->ohen -= price;
		p->regen_health++;
	}
}
