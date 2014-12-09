#ifndef SHOT_H
#define SHOT_H
#include <list>
#include "shapes.h"
#include "enemy.h"

class Shot : public Cara
{
public:
	float speed;
	bool stuck;
	float damage;
	std::list<EnemyBasic>::iterator stickb;
	std::list<EnemyTank>::iterator stickt;
	Shot();
};

#endif // SHOT_H
