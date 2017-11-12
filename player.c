#include <stdlib.h>
#include "player.h"

Player createPlayer(){
	Player p;
	p.ohen = 0;
	p.max_ohen = 1000;
	p.shield = 0;
	p.attack_damage = 1;
	p.defense = 1;
	p.health = 100;
	p.max_health = 100;
	p.regen_ohen = 10;
	p.regen_health = 1;
	p.vulnerable = true;
	p.state = GENERATE;
	return p;
}

void generate(Player * player){
	if(player->ohen + player->regen_ohen <= player->max_ohen)
		player->ohen += player->regen_ohen;
}

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

void defend(Player * defender){
	defender->health += defender->regen_health;
}

void upgradeAttackDamage(Player * p){
	int price = 50*p->attack_damage;
	if(p->ohen >= price){
		p->ohen -= price;
		p->attack_damage++;
	}
}

void upgradeRegenOhen(Player * p){
	int price = 20*p->regen_ohen;
	if(p->ohen >= price){
		p->ohen -= price;
		p->regen_ohen++;
	}
}

void upgradeDefense(Player * p){
	int price = 60*p->defense;
	if(p->ohen >= price){
		p->ohen -= price;
		p->defense++;
	}
}

void upgradeHealthRegen(Player * p){
	int price = 20*p->regen_health;
	if(p->ohen >= price){
		p->ohen -= price;
		p->regen_health++;
	}
}
