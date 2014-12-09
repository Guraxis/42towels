#include "player.h"
#include "obrazek.h"

Player::Player()
{
	moveup = 0;
	movedown = 0;
	moveleft = 0;
	moveright = 0;
	speed = 3;
	health = 5;
}

void Player::drawplayer()
{
	kruznice(body);
	cara(body.x, body.y, hands.x2, hands.y2);
}
