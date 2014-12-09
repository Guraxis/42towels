#ifndef PLAYER_H
#define PLAYER_H
#include "character.h"
#include "shapes.h"

class Player : public Character
{
public:
	Cara hands;
	bool moveup, movedown, moveleft, moveright;
	void drawplayer();
	Player();
};

#endif // PLAYER_H
