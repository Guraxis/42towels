#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>
#include <list>
#include <sys/time.h>

#include "obrazek.h"
#include "character.h"
#include "shapes.h"
#include "enemy.h"
#include "player.h"
#include "shot.h"

#define RESX 800
#define RESY 600

Obrazovka* obrazovka = Obrazovka::instance();
int t1, t2;
Pismo numbers;

float nahoda(float min, float max)
{
	return (float)rand()/RAND_MAX*(max-min)+min;
}




int wallcollide(int x, int y, int r, Rectangle roof, Rectangle ground, Rectangle left, Rectangle right)
{
	if(y - r < roof.h || y + r > ground.y || x - r < left.w || x + r > right.x)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int enemycollide(int x, int y, int r, int ex, int ey, int er)
{
	if((abs(ex - x) * abs(ex - x)) + (abs(ey - y) * abs(ey - y)) < (er + r) * (er + r))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool dead(int x, int y)
{
	numbers.nacti("numbers.png", "0123456789");
	struct timeval tv;
	while(1)
	{
		t1 = SDL_GetTicks();
		SDL_FillRect(obrazovka->screen, NULL, 0);
		numbers.umisti(RESX/3, RESY/3);
		numbers.kresli("0123");
		obrazovka->aktualizuj();
		SDL_Event devent;
		while(SDL_PollEvent(&devent))
		{
			switch(devent.key.keysym.sym)
			{
			case SDLK_SPACE:
				return 0;
			case SDLK_ESCAPE:
				return 1;
			}
			if(devent.type == SDL_QUIT)
			{
				return 1;
			}
		}
		t2 = SDL_GetTicks();
		if(t2-t1<=17)
		{
			SDL_Delay(17-(t2-t1));
		}
	}
}

bool game()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
	int spawnx, spawny;
	int spawnlimit = 50;
	int spawncount = 0;
	int mousex, mousey;
	int shoot = 0;
	int delay = 20;
	int basicspawndelay = 0;
	int tankspawndelay = 0;
	int basicspawnrate = 100;
	int tankspawnrate = 500;
	int rate = 25;
	bool nolimit = 0;


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

	std::list<EnemyBasic> enemiesbasic;
	std::list<EnemyBasic>::iterator eb;
	std::list<EnemyBasic>::iterator eb1;

	std::list<EnemyTank> enemiestank;
	std::list<EnemyTank>::iterator et;
	std::list<EnemyTank>::iterator et1;

	std::list<Shot> shots;
	std::list<Shot>::iterator s;

	std::list<Animace> deaths;
	std::list<Animace>::iterator d;


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
					return 1;
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
				case SDLK_e:
					rate--;
					break;
				case SDLK_q:
					rate++;
					break;
				case SDLK_SPACE:
					nolimit = 1;
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
				case SDLK_SPACE:
					nolimit = 0;
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
				return 1;
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
			for(eb = enemiesbasic.begin(); eb != enemiesbasic.end(); eb++)
			{
				if(s -> stickb == eb)
				{
					s -> x1 += eb -> body.vx;
					s -> y1 += eb -> body.vy;
					s -> x2 += eb -> body.vx;
					s -> y2 += eb -> body.vy;
					s -> stuck = 1;
					break;
				}
			}
			for(et = enemiestank.begin(); et != enemiestank.end(); et++)
			{
				if(s -> stickt == et)
				{
					s -> x1 += et -> body.vx;
					s -> y1 += et -> body.vy;
					s -> x2 += et -> body.vx;
					s -> y2 += et -> body.vy;
					s -> stuck = 1;
					break;
				}
			}

			if(s -> stuck == 0)
			{
				s -> x1 += s -> vx1;
				s -> y1 += s -> vy1;
				s -> x2 += s -> vx2;
				s -> y2 += s -> vy2;
			}

			cara(s -> x1, s -> y1, s -> x2, s -> y2);

			if(wallcollide(s -> x2, s -> y2, 0, roof, ground, leftwall, rightwall))
			{
				std::list<Shot>::iterator s2 = s;
				s++;
				shots.erase(s2);
				s--;
			}
		}

		if(basicspawndelay > basicspawnrate && spawncount < spawnlimit)
		{
			while(1)
			{
				spawnx = nahoda(leftwall.w + 15, rightwall.x - 15);
				spawny = nahoda(roof.h + 15, ground.y - 15);
				if((abs(player.body.x - spawnx)*abs(player.body.x - spawnx)) + (abs(player.body.y - spawny)*abs(player.body.y - spawny)) > 300*300)
				{
					EnemyBasic* eb = new EnemyBasic();
					eb -> body.x = spawnx;
					eb -> body.y = spawny;
					enemiesbasic.push_back(*eb);
					basicspawndelay = 0;
					spawncount ++;
					break;
				}
			}
		}

		if(tankspawndelay > tankspawnrate && spawncount < spawnlimit)
		{
			while(1)
			{
				spawnx = nahoda(leftwall.w + 15, rightwall.x - 15);
				spawny = nahoda(roof.h + 15, ground.y - 15);
				if((abs(player.body.x - spawnx)*abs(player.body.x - spawnx)) + (abs(player.body.y - spawny)*abs(player.body.y - spawny)) > 300*300)
				{
					EnemyTank* et = new EnemyTank();
					et -> body.x = spawnx;
					et -> body.y = spawny;
					enemiestank.push_back(*et);
					tankspawndelay = 0;
					spawncount ++;
					break;
				}
			}
		}

		for(eb = enemiesbasic.begin(); eb != enemiesbasic.end(); eb++)
		{
			eb -> angle = atan2(player.body.y - eb -> body.y , player.body.x - eb -> body.x);
			eb -> body.vx = cos(eb -> angle) * eb -> speed;
			eb -> body.vy = sin(eb -> angle) * eb -> speed;
			for(eb1 = enemiesbasic.begin(); eb1 != enemiesbasic.end(); eb1++)
			{
				{
					if((abs(eb -> body.x - eb1 -> body.x)) * (abs(eb -> body.x - eb1 -> body.x))
							+ (abs(eb -> body.y - eb1 -> body.y)) * (abs(eb -> body.y - eb1 -> body.y))
							< (eb -> body.r + eb1 -> body.r) * (eb -> body.r + eb1 -> body.r)
							&& eb -> body.x != eb1 -> body.x && eb -> body.y != eb1 -> body.y)
					{
						eb -> tempangle = atan2(eb -> body.y - eb1 -> body.y, eb -> body.x - eb1 -> body.x);
						eb -> body.vx += cos(eb -> tempangle) * eb -> speed * 1.5;
						eb -> body.vy += sin(eb -> tempangle) * eb -> speed * 1.5;
					}
				}
			}
			for(et1 = enemiestank.begin(); et1 != enemiestank.end(); et1++)
			{
				{
					if((abs(eb -> body.x - (et1 -> body.x + et1 -> body.w/2))) * (abs(eb -> body.x - (et1 -> body.x + et1 -> body.w/2)))
							+ (abs(eb -> body.y - (et1 -> body.y + et1 -> body.w/2))) * (abs(eb -> body.y - (et1 -> body.y + et1 -> body.w/2)))
							< (eb -> body.r + et1 -> body.w/2) * (eb -> body.r + et1 -> body.w/2)
							&& eb -> body.x != et1 -> body.x + et1 -> body.w/2 && eb -> body.y != et1 -> body.y + et1 -> body.w/2)
					{
						eb -> tempangle = atan2(eb -> body.y - (et1 -> body.y + et1 -> body.w/2), eb -> body.x - (et1 -> body.x + et1 -> body.w/2));
						eb -> body.vx += cos(eb -> tempangle) * eb -> speed * 1.5;
						eb -> body.vy += sin(eb -> tempangle) * eb -> speed * 1.5;
					}
				}
			}
			eb -> body.x += eb -> body.vx;
			eb -> body.y += eb -> body.vy;
			while(eb -> body.x - eb -> body.r < leftwall.w)
			{
				eb -> body.x++;
			}
			while(eb -> body.x + eb -> body.r > rightwall.x)
			{
				eb -> body.x--;
			}
			while(eb -> body.y - eb -> body.r < roof.h)
			{
				eb -> body.y++;
			}
			while(eb -> body.y + eb -> body.r > ground.y)
			{
				eb -> body.y--;
			}

			for(s = shots.begin(); s != shots.end(); s++)
			{
				if(enemycollide(s -> x2, s -> y2, 0, eb -> body.x, eb -> body.y, eb -> body.r) && !s -> stuck)
				{
					s -> stickb = eb;
					eb -> health -= s -> damage;
					s -> damage = 0;
				}
			}

			if(enemycollide(eb -> body.x, eb -> body.y, eb -> body.r, player.body.x, player.body.y, player.body.r))
			{
				player.health--;
				eb -> die = 1;
			}

			switch(eb -> health)
			{
			case 3:
				barva(255,255,255);
				break;
			case 2:
				barva(255,100,100);
				break;
			case 1:
				barva(255,0,0);
				break;
			}

			if(eb -> health <= 0)
			{
				eb -> die = 1;
			}


			if(eb -> die)
			{
				Animace* death = new Animace();
				death -> nacti("death.png", 33, 33);
				death -> umisti(eb -> body.x - 17, eb -> body.y - 17);
				deaths.push_back(*death);

				for(s = shots.begin(); s != shots.end(); s++)
				{
					if(s -> stickb == eb)
					{
						std::list<Shot>::iterator s2 = s;
						s++;
						shots.erase(s2);
						s--;
					}
				}

				std::list<EnemyBasic>::iterator eb2 = eb;
				eb++;
				enemiesbasic.erase(eb2);
				eb--;

				spawncount--;
			}
			kruznice(eb -> body);
		}


		for(et = enemiestank.begin(); et != enemiestank.end(); et++)
		{
			et -> angle = atan2(player.body.y - et -> body.y + et -> body.w/2, player.body.x - et -> body.x + et -> body.w/2);
			et -> body.vx = cos(et -> angle) * et -> speed;
			et -> body.vy = sin(et -> angle) * et -> speed;
			for(et1 = enemiestank.begin(); et1 != enemiestank.end(); et1++)
			{
				{
					if((abs((et -> body.x + et -> body.w/2) - (et1 -> body.x + et1 -> body.w/2))) * (abs((et -> body.x + et -> body.w/2) - (et1 -> body.x + et1 -> body.w/2)))
							+ (abs((et -> body.y + et -> body.w/2) - (et1 -> body.y + et1 -> body.w/2))) * (abs((et -> body.y + et -> body.w/2) - (et1 -> body.y + et1 -> body.w/2)))
							< (et -> body.w/2 + et1 -> body.w/2) * (et -> body.w/2 + et1 -> body.w/2)
							&& et -> body.x + et -> body.w/2!= et1 -> body.x + et -> body.w/2 && et -> body.y + et -> body.w/2!= et1 -> body.y + et -> body.w/2)
					{
						et -> tempangle = atan2((et -> body.y + et -> body.w/2) - (et1 -> body.y + et1 -> body.w/2), (et -> body.x + et -> body.w/2) - (et1 -> body.x + et1 -> body.w/2));
						et -> body.vx += cos(et -> tempangle) * et -> speed * 2.5;
						et -> body.vy += sin(et -> tempangle) * et -> speed * 2.5;
					}
				}
			}
			for(eb1 = enemiesbasic.begin(); eb1 != enemiesbasic.end(); eb1++)
			{
				{
					if((abs((et -> body.x + et -> body.w/2) - eb1 -> body.x)) * (abs((et -> body.x + et -> body.w/2) - eb1 -> body.x))
							+ (abs((et -> body.y + et -> body.w/2) - eb1 -> body.y)) * (abs((et -> body.y + et -> body.w/2) - eb1 -> body.y))
							< (et -> body.w/2 + eb1 -> body.r) * (et -> body.w/2 + eb1 -> body.r)
							&& et -> body.x + et -> body.w/2 != eb1 -> body.x + et -> body.w/2 && et -> body.y + et -> body.w/2 != eb1 -> body.y + et -> body.w/2)
					{
						et -> tempangle = atan2((et -> body.y + et -> body.w/2) - eb1 -> body.y, (et -> body.x + et -> body.w/2) - eb1 -> body.x);
						et -> body.vx += cos(et -> tempangle) * et -> speed * 2.5;
						et -> body.vy += sin(et -> tempangle) * et -> speed * 2.5;
					}
				}
			}
			et -> body.x += et -> body.vx;
			et -> body.y += et -> body.vy;
			while(et -> body.x + et -> body.w/2- et -> body.w/2 < leftwall.w)
			{
				et -> body.x++;
			}
			while(et -> body.x + et -> body.w/2+ et -> body.w/2 > rightwall.x)
			{
				et -> body.x--;
			}
			while(et -> body.y + et -> body.w/2- et -> body.w/2 < roof.h)
			{
				et -> body.y++;
			}
			while(et -> body.y + et -> body.w/2+ et -> body.w/2 > ground.y)
			{
				et -> body.y--;
			}

			for(s = shots.begin(); s != shots.end(); s++)
			{
				if(enemycollide(s -> x2, s -> y2, 0, et -> body.x + et -> body.w/2, et -> body.y + et -> body.w/2, et -> body.w/2) && !s -> stuck)
				{
					s -> stickt = et;
					et -> health -= s -> damage;
					s -> damage = 0;
				}
			}

			if(enemycollide(et -> body.x , et -> body.y + et -> body.w/2, et -> body.w/2, player.body.x, player.body.y, player.body.r))
			{
				player.health--;
				et -> die = 1;
			}

			switch(et -> health)
			{
			case 5:
				barva(255,255,255);
				break;
			case 4:
				barva(255,150,150);
				break;
			case 3:
				barva(255,100,100);
				break;
			case 2:
				barva(255,50,50);
				break;
			case 1:
				barva(220,0,0);
				break;
			}

			if(et -> health <= 0)
			{
				et -> die = 1;
			}


			if(et -> die)
			{
				Animace* death = new Animace();
				death -> nacti("death.png", 33, 33);
				death -> umisti(et -> body.x + et -> body.w/2 - 17, et -> body.y + et -> body.w/2 - 17);
				deaths.push_back(*death);

				for(s = shots.begin(); s != shots.end(); s++)
				{
					if(s -> stickt == et)
					{
						std::list<Shot>::iterator s2 = s;
						s++;
						shots.erase(s2);
						s--;
					}
				}

				std::list<EnemyTank>::iterator et2 = et;
				et++;
				enemiestank.erase(et2);
				et--;

				spawncount--;
			}
			hrectangle(et -> body);
		}

		for(d = deaths.begin(); d != deaths.end(); d++)
		{
			d -> kresli();
			d -> aktualizuj();
			if(d -> frame == 0)
			{
				std::list<Animace>::iterator d2 = d;
				d++;
				deaths.erase(d2);
				d--;
			}
		}

		switch(player.health)
		{
		case 5:
			barva(255,255,255);
			break;
		case 4:
			barva(255,150,150);
			break;
		case 3:
			barva(255,100,100);
			break;
		case 2:
			barva(255,50,50);
			break;
		case 1:
			barva(220,0,0);
			break;
		case 0:
			if(dead(player.body.x, player.body.y))
			{
				return 1;
			}
			return 0;
		case -1:
			if(dead(player.body.x, player.body.y))
			{
				return 1;
			}
			return 0;
		case -2:
			if(dead(player.body.x, player.body.y))
			{
				return 1;
			}
			return 0;
		}

		player.drawplayer();
		barva(255,255,255);
		rectangle(ground);
		rectangle(roof);
		rectangle(leftwall);
		rectangle(rightwall);

		obrazovka->aktualizuj();

		delay++;
		basicspawndelay++;
		tankspawndelay++;

		t2 = SDL_GetTicks();
		if(t2-t1<=17 && nolimit == 0)
		{
			SDL_Delay(17-(t2-t1));
		}
	}
}

int main(int argc, char** argv)
{
	obrazovka->inicializuj(RESX, RESY, 0, 0);
	while(1)
	{
		if(game())
		{
			SDL_QUIT;
			return 0;
		}
	}
}
