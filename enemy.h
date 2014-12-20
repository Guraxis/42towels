#ifndef ENEMY_H
#define ENEMY_H
#include "character.h"

class Enemy : public Character
{
public:
	double tempangle;
	bool die;
	int spawnrate;
	Enemy();
};

class EnemyBasic : public Enemy
{
public:
	EnemyBasic();

};

class EnemyTank : public Enemy
{
public:
	Rectangle body;
	EnemyTank();
};
#endif // ENEMY_H
