#include <stdlib.h>
#include <stdio.h>
#include "player.h"

// Player "Constructor" Initialize values at default.
Player createPlayer(int id, char * nickname){
	Player p;
	p.id 			= id;
	p.nickname      = nickname;
	p.ohen 			= 0;
	p.max_ohen 		= 1000;
	p.shield 		= 0;
	p.attack_damage	= 1;
	p.defense 		= 1;
	p.health 		= 50;
	p.max_health 	= 100;
	p.regen_ohen 	= 1;
	p.regen_health 	= 1;
	p.vulnerable 	= true;
	p.state 		= GENERATE;
	return p;
}

// Convert the Player p structure into an array of integers and returns it.
int * player2CharArray(Player p){
	int * arr = malloc(sizeof(int)*11);
	arr[0] = p.id;
	arr[1] = p.ohen;
	arr[2] = p.max_ohen;
	arr[3] = p.shield;
	arr[4] = p.attack_damage;
	arr[5] = p.defense;
	arr[6] = p.health;
	arr[7] = p.max_health;
	arr[8] = p.regen_ohen;
	arr[9] = p.regen_health;
	arr[10] = p.vulnerable;
	arr[11] = p.targetId;
	arr[12] = p.state;
	return arr;
}

// Convert the int array arr into a Player p structure and returns it.
Player intArray2Player(int * arr){
	Player p;
	p.id 			= arr[0];
	p.ohen 			= arr[1];
	p.max_ohen 		= arr[2];
	p.shield 		= arr[3];
	p.attack_damage = arr[4];
	p.defense 		= arr[5];
	p.health 		= arr[6];
	p.max_health 	= arr[7];
	p.regen_ohen 	= arr[8];
	p.regen_health 	= arr[9];
	p.vulnerable 	= arr[10];
	p.targetId 		= arr[11];
	p.state 		= arr[12];
	return p;
}

// Convert an int array intarr, into a char array chararr and returns it.
char * serializeIntArray(int * intarr){
	char * chararr = malloc(sizeof(char[256])); // PROBLEME ?
	for(int i = 0; i < 13; i++){
		sprintf(chararr,"%s%d/", chararr,intarr[i]);
	}
	return chararr;
}

// Convert a Player into a char array of char array that can be sent over a socket.
char * serializePlayer(Player p){
	char * charr = malloc(sizeof(char)*512);
	sprintf(charr,"%s%d/", charr, p.id);
	sprintf(charr,"%s%s/", charr, p.nickname);
	sprintf(charr,"%s%d/", charr, p.ohen);
	sprintf(charr,"%s%d/", charr, p.max_ohen);
	sprintf(charr,"%s%d/", charr, p.shield);
	sprintf(charr,"%s%d/", charr, p.attack_damage);
	sprintf(charr,"%s%d/", charr, p.defense);
	sprintf(charr,"%s%d/", charr, p.health);
	sprintf(charr,"%s%d/", charr, p.max_health);
	sprintf(charr,"%s%d/", charr, p.regen_ohen);
	sprintf(charr,"%s%d/", charr, p.regen_health);
	sprintf(charr,"%s%d/", charr, p.vulnerable);
	sprintf(charr,"%s%d/", charr, p.targetId);
	sprintf(charr,"%s%d/", charr, p.state);
	return charr;
}

// Convert an array of Players into an Array of array of char.
char * serializePlayers(Player * p, int playerCount){
	char * players = malloc(sizeof(char[256*playerCount]));
	for(int i = 0; i < playerCount; i++){
		sprintf(players,"%s%s;", players,serializePlayer(p[i]));
	}
	return players;
}

// Makes a Player p generating some ohen depending on his ohen regeneration.
void generate(Player * player){
	if(player->ohen + player->regen_ohen <= player->max_ohen)
		player->ohen += player->regen_ohen;
	else
		player->ohen = player->max_ohen;
}

// Makes a Player "attacker" attack a Player "defender"
void attack(Player * attacker, Player * defender){
	if(attacker->ohen >= attacker->attack_damage){
		attacker->ohen -= attacker->attack_damage;
	}
	else{
		attacker->state = GENERATE;
		return;
	}
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
					if(defender->health >= damages )
						defender->health -= damages;
					else
						defender->health = 0;
				}
			}
			else{
				if(defender->health >= damages )
					defender->health -= damages;
				else
					defender->health = 0;
			}
		}
		else{
			if(attacker -> health >= (defender->defense - attacker->attack_damage)/2)
				attacker->health -= (defender->defense - attacker->attack_damage)/2;
			else
				attacker->health = 0;
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
			if(defender->health >= damages )
				defender->health -= damages;
			else
				defender->health = 0;
		}
	}
}

// Makes the defender regen twice.
void defend(Player * defender){
	if(defender->health + defender->regen_health <= defender->max_health){
		defender->health += defender->regen_health;
	}
}

// Upgrade the Attack Damage of the Player p
void upgradeAttackDamage(Player * p){
	int price = 8*p->attack_damage;
	if(p->ohen >= price){
		p->ohen -= price;
		p->attack_damage++;
	}
}

// Upgrade the Ohen Regeneration of the Player p
void upgradeRegenOhen(Player * p){
	int price = 10*p->regen_ohen;
	printf("Ohen : %d\n", p->ohen);
	printf("Price : %d\n", price);
	if(p->ohen >= price){
		p->ohen -= price;
		p->regen_ohen++;
	}
}

// Upgrade the Defense of the Player p.
void upgradeDefense(Player * p){
	int price = 9*p->defense;
	if(p->ohen >= price){
		p->ohen -= price;
		p->defense++;
	}
}

//Upgrade Health regeneration of the Player p.
void upgradeHealthRegen(Player * p){
	int price = 10*p->regen_health;
	if(p->ohen >= price){
		p->ohen -= price;
		p->regen_health++;
	}
}
