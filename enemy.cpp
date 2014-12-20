#include "enemy.h"

Enemy::Enemy()
{
	die = 0;
}

EnemyBasic::EnemyBasic()
{
	health = 3;
	speed = 2;
	spawnrate = 100;
	body.r = 15;
}

EnemyTank::EnemyTank()
{
	health = 5;
	speed = 1.5;
	spawnrate = 500;
	body.w = 30;
	body.h = body.w;
}
