#ifndef CHARACTER_H
#define CHARACTER_H
#include "shapes.h"

class Character
{
public:
	Kruznice body;
	float angle;
	float speed;
	int health;
	Character();
};

#endif // CHARACTER_H
