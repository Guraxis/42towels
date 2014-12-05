#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>
#include <list>
#include <sys/time.h>

#include "obrazek.h"

#define RESX 800
#define RESY 600

bool pixelstate[RESX][RESY];

void resetpixelstate()
{
	for(int i=0;i<RESX;i++)
	{
		for(int j=0;j<RESY;j++)
		{
			pixelstate[i][j] = 0;
		}
	}
}

float nahoda(float min, float max)
{
	return (float)rand()/RAND_MAX*(max-min)+min;
}

void kruznice(int x, int y, int r)
{
	for(float alfa=0;alfa<2*M_PI;alfa+=0.01)
	{
		bod(x+cos(alfa)*r,y+sin(alfa)*r);
	}
}

class Rectangle
{
public:
	float x, y, w, h, vx, vy;
};

class Kruznice
{
public:
	float x, y, r, vx, vy;
};

class Bod
{
public:
	float x, y, vx, vy;
};

void setpixelstate(int x,int y)
{
	pixelstate[x][y] == 1;
}

bool getpixelstate(int x,int y)
{
	return pixelstate[x][y];
}

void rectangle(Rectangle a)
{
	for(int i=0; i<a.h; i++)
	{
		cara(a.x,a.y+i,a.x+a.w,a.y+i);
	}
}

void crectangle(Rectangle a)
{
	for(int i=0; i<a.h; i++)
	{
		cara(a.x,a.y+i,a.x+a.w,a.y+i);
		for(int j=0; j<a.w; j++)
		{
			setpixelstate(j,i);
		}
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

int getheight(int x, int y)
{
	for(int i = y; i < 2000; i++)
	{
		if(getpixelstate(x,i)==1)
		{
			return(i-y);
		}
	}
}

int main(int argc, char** argv)
{
	int t1, t2;
	Obrazovka* obrazovka = Obrazovka::instance();
	obrazovka->inicializuj(RESX, RESY, 0, 0);
	Rectangle ground, roof, leftwall, rightwall;
	ground.x = -400;
	ground.y = RESY-20;
	ground.h = 300;
	ground.w = 1800;
	Rectangle player;
	player.w = 20;
	player.h = 30;
	player.x = RESX/2 - player.w/2;
	player.y = RESY/2 - player.h/2;
	player.vy = 0.5;

	while(1)
	{
		t1 = SDL_GetTicks();
		SDL_FillRect(obrazovka->screen, NULL, 0);
		resetpixelstate();



		barva(255,255,255);
		crectangle(ground);
		rectangle(player);

		if(getheight(player.x,player.y)>0)
		{
			ground.y -= player.vy;
		}
		else
		{
			//player.vy = 0;
		}
		player.vy += 0.1;

		obrazovka->aktualizuj();

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
				}
				break;
			case SDL_QUIT:
				SDL_Quit();
				return 0;
			}
		}
		t2 = SDL_GetTicks();
		if(t2-t1<=17)
		{
			SDL_Delay(17-(t2-t1));
		}
	}
}
