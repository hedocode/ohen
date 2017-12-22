#ifndef _player
#define _player

#include "core.h"
	
typedef struct{
	int id;
	
	char * nickname;
	int ohen;
	int max_ohen;

	int shield;
	int attack_damage;
	int defense;

	int health;
	int max_health;

	int regen_ohen;
	int regen_health;

	int targetId;	

	bool vulnerable;
	Status state;
} Player;


Player createPlayer();
void generate(Player * player);
void attack(Player * attacker, Player * defender);
void defend(Player * defender);
void upgradeAttackDamage(Player * p);
void upgradeRegenOhen(Player * p);
void upgradeDefense(Player * p);
void upgradeHealthRegen(Player * p);
char * serializePlayer(Player p);
char * serializePlayers(Player * p, int playerCount);
char * serializeIntArray(int * intarr);
Player intArray2Player(int * arr);
int * player2IntArray(Player p);
#endif
