#include "core.h"

typedef struct{
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
	status state;
} Player;

typedef Player Player;

Player createPlayer();
void generate(Player * player);
void attack(Player * attacker, Player * defender);
void defend(Player * defender);
void upgradeAttackDamage(Player * p);
void upgradeRegenOhen(Player * p);
void upgradeDefense(Player * p);
void upgradeHealthRegen(Player * p);
