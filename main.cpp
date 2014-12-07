#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>
#include <list>
#include <sys/time.h>

#include "obrazek.h"

#define RESX 800
#define RESY 600


float nahoda(float min, float max)
{
	return (float)rand()/RAND_MAX*(max-min)+min;
}


class Rectangle
{
public:
	float x, y, w, h, vx, vy;
	Rectangle() {vx = 0; vy = 0;}
};

class Kruznice
{
public:
	float x, y, r, vx, vy;
	Kruznice() {vx = 0; vy = 0;}
};

class Bod
{
public:
	float x, y, vx, vy;
	Bod() {vx = 0; vy = 0;}
};

class Cara
{
public:
	float x1, y1, x2, y2, vx1, vy1, vx2, vy2;
	Cara() {vx1 = 0; vy1 = 0; vx2 = 0; vy2 = 0;}
};

class Character
{
public:
	Kruznice body;
	float angle;
	float speed;
	float health;
	bool moveup, movedown, moveleft, moveright;
	Character() {angle = 0; moveup = 0; movedown = 0; moveleft = 0; moveright = 0; speed = 2.5; health = 2;}
};

class Player : public Character
{
public:
	Cara hands;
	Player() {}
	virtual ~Player() {}
};

class Shot : public Cara
{
public:
	float speed;
	Shot() {speed = 3.5;}
};

void kruznice(int x, int y, int r)
{
	for(float alfa=0;alfa<2*M_PI;alfa+=0.01)
	{
		bod(x+cos(alfa)*r,y+sin(alfa)*r);
	}
}

void kruznice(Kruznice a)
{
	for(float alfa=0;alfa<2*M_PI;alfa+=0.01)
	{
		bod(a.x+cos(alfa)*a.r,a.y+sin(alfa)*a.r);
	}
}

void rectangle(Rectangle a)
{
	for(int i=0; i<a.h; i++)
	{
		cara(a.x,a.y+i,a.x+a.w,a.y+i);
	}
}

void hrectangle(Rectangle a)
{
	cara(a.x,a.y,a.x+a.w,a.y);
	cara(a.x+a.w,a.y+a.h);
	cara(a.x,a.y+a.h);
	cara(a.x,a.y);
}

void triangle(int x, int y, int w, int h)
{
	cara(x,y,x+w,y);
	cara(x+w/2,y-h);
	cara(x,y);
}

void triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	cara(x1,y1,x2,y2);
	cara(x3,y3);
	cara(x1,y1);
}

void hvezda(int x, int y, int r)
{
	bod(x+r,y);
	for(float alfa=0;alfa<10*M_PI;alfa+=2*M_PI/12*5)
	{
		cara(x+cos(alfa)*r,y+sin(alfa)*r);
	}
	cara(x+r,y);
}

void drawplayer(Player p)
{
	kruznice(p.body);
	cara(p.body.x, p.body.y, p.hands.x2, p.hands.y2);
}

int wallcollide(int x, int y, Rectangle roof, Rectangle ground, Rectangle left, Rectangle right)
{
	if(y < roof.h || y > ground.y || x < left.w || x > right.x)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int enemycollide(int x, int y, int ex, int ey, int er)
{
	if((abs(ex - x) * abs(ex - x)) + (abs(ey - y) * abs(ey - y)) < er * er)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int main(int argc, char** argv)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
	int spawnx, spawny;
	int spawnlimit = 35;
	int spawncount = 0;
	int t1, t2;
	int mousex, mousey;
	int shoot = 0;
	int delay = 20;
	int rate = 20;
	int spawndelay = 0;
	int tempangle = 0;

	Obrazovka* obrazovka = Obrazovka::instance();
	obrazovka->inicializuj(RESX, RESY, 0, 0);

	Rectangle ground, roof, leftwall, rightwall;
	ground.x = 0;
	ground.y = RESY-20;
	ground.h = 20;
	ground.w = RESX;
	roof.x = 0;
	roof.y = 0;
	roof.h = 20;
	roof.w = RESX;
	leftwall.x = 0;
	leftwall.y = 0;
	leftwall.h = RESY;
	leftwall.w = 20;
	rightwall.x = RESX-20;
	rightwall.y = 0;
	rightwall.h = RESY;
	rightwall.w = 20;

	Player player;
	player.body.r = 20;
	player.body.x = RESX/2;
	player.body.y = RESY/2;
	player.hands.x1 = player.body.x;
	player.hands.y1 = player.body.y;
	player.hands.x2 = player.body.x+25;
	player.hands.y2 = player.body.y;

	std::list<Character> enemies;
	std::list<Character>::iterator e;
	std::list<Character>::iterator e1;

	std::list<Shot> shots;
	std::list<Shot>::iterator s;


	while(1)
	{
		t1 = SDL_GetTicks();
		SDL_FillRect(obrazovka->screen, NULL, 0);

		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					SDL_Quit();
					return 0;
				case SDLK_w:
					player.moveup = 1;
					break;
				case SDLK_a:
					player.moveleft = 1;
					break;
				case SDLK_s:
					player.movedown = 1;
					break;
				case SDLK_d:
					player.moveright = 1;
					break;
				}
				break;
			case SDL_KEYUP:
				switch(event.key.keysym.sym)
				{
				case SDLK_w:
					player.moveup = 0;
					break;
				case SDLK_a:
					player.moveleft = 0;
					break;
				case SDLK_s:
					player.movedown = 0;
					break;
				case SDLK_d:
					player.moveright = 0;
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				mousex = event.motion.x;
				mousey = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				shoot = 1;
				break;
			case SDL_MOUSEBUTTONUP:
				shoot = 0;
				break;
			case SDL_QUIT:
				SDL_Quit();
				return 0;
			}
		}

		player.angle = atan2(mousey - player.body.y , mousex - player.body.x);

		player.hands.x2 = player.body.x + cos(player.angle) * (player.body.r + 5);
		player.hands.y2 = player.body.y + sin(player.angle) * (player.body.r + 5);

		if(player.moveup && player.body.y > roof.h + player.body.r)
		{
			player.body.y -= player.speed;
		}
		if(player.movedown && player.body.y < ground.y - player.body.r)
		{
			player.body.y += player.speed;
		}
		if(player.moveleft && player.body.x > leftwall.w + player.body.r)
		{
			player.body.x -= player.speed;
		}
		if(player.moveright && player.body.x < rightwall.x - player.body.r)
		{
			player.body.x += player.speed;
		}

		if(shoot && delay > rate)
		{
			Shot* shot = new Shot();
			shot -> x1 = player.body.x;
			shot -> y1 = player.body.y;
			shot -> x2 = player.hands.x2;
			shot -> y2 = player.hands.y2;
			shot -> vx1 = cos(player.angle) * shot -> speed;
			shot -> vy1 = sin(player.angle) * shot -> speed;
			shot -> vx2 = shot -> vx1;
			shot -> vy2 = shot -> vy1;
			shots.push_back(*shot);
			delay = 0;
		}

		for(s = shots.begin(); s != shots.end(); s++)
		{
			s -> x1 += s -> vx1;
			s -> y1 += s -> vy1;
			s -> x2 += s -> vx2;
			s -> y2 += s -> vy2;
			cara(s -> x1, s -> y1, s -> x2, s -> y2);

			if(wallcollide(s -> x2, s -> y2, roof, ground, leftwall, rightwall))
			{
				std::list<Shot>::iterator s2 = s;
				s++;
				shots.erase(s2);
				s--;
			}
		}

		if(spawndelay > 50 && spawncount < spawnlimit)
		{
			while(1)
			{
				spawnx = nahoda(leftwall.w + 15, rightwall.x - 15);
				spawny = nahoda(roof.h + 15, ground.y - 15);
				if((abs(player.body.x - spawnx)*abs(player.body.x - spawnx)) + (abs(player.body.y - spawny)*abs(player.body.y - spawny)) > 300*300)
				{
					Character* enemy = new Character();
					enemy -> body.x = spawnx;
					enemy -> body.y = spawny;
					enemy -> body.r = 15;
					enemy -> speed = 1;
					enemies.push_back(*enemy);
					spawndelay = 0;
					spawncount ++;
					break;
				}
			}
		}

		for(e = enemies.begin(); e != enemies.end(); e++)
		{
			e -> angle = atan2(player.body.y - e -> body.y , player.body.x - e -> body.x);
			e -> body.vx = cos(e -> angle) * e -> speed;
			e -> body.vy = sin(e -> angle) * e -> speed;
			e -> body.x += e -> body.vx;
			e -> body.y += e -> body.vy;
			kruznice(e -> body.x, e -> body.y, e -> body.r);

			for(e1 = enemies.begin(); e1 != enemies.end(); e1++)
			{
				if(e -> body.x == e1 -> body.x && e -> body.y == e1 -> body.y){}
				else
				{
					if((abs(e -> body.x - e1 -> body.x)) * (abs(e -> body.x - e1 -> body.x))
							+ (abs(e -> body.y - e1 -> body.y)) * (abs(e -> body.y - e1 -> body.y))
							< (e -> body.r + e1 -> body.r) * (e -> body.r + e1 -> body.r))
					{
						tempangle = atan2(e -> body.y - e1 -> body.y, e -> body.x - e1 -> body.x);
						e -> body.x += cos(tempangle) * e -> speed ;
						e -> body.y += sin(tempangle) * e -> speed ;
					}
				}
			}

			for(s = shots.begin(); s != shots.end(); s++)
			{
				if(enemycollide(s -> x2, s -> y2, e -> body.x, e -> body.y, e -> body.r))
				{
					std::list<Shot>::iterator s2 = s;
					s++;
					shots.erase(s2);
					s--;

					e -> health--;
				}
			}

			if(e -> health == 0)
			{
				std::list<Character>::iterator e2 = e;
				e++;
				enemies.erase(e2);
				e--;
				spawncount--;
			}
		}

		barva(255,255,255);
		rectangle(ground);
		rectangle(roof);
		rectangle(leftwall);
		rectangle(rightwall);
		drawplayer(player);

		obrazovka->aktualizuj();

		delay++;
		spawndelay++;

		t2 = SDL_GetTicks();
		if(t2-t1<=17)
		{
			SDL_Delay(17-(t2-t1));
		}
	}
}
