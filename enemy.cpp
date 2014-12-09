#include "enemy.h"

Enemy::Enemy()
{
	die = 0;
}

EnemyBasic::EnemyBasic()
{
	health = 3;
	speed = 2.5;
	spawnrate = 100;
}

EnemyTank::EnemyTank()
{
	health = 5;
	speed = 1.5;
	spawnrate = 500;
}
