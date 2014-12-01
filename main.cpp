#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>
#include <list>
#include <sys/time.h>

#include "obrazek.h"

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

int main(int argc, char** argv)
{
	int stage=1;
	int r=255, g=0, b=0;
	int timer=0;
	int t1, t2;
	Obrazovka* obrazovka = Obrazovka::instance();
	obrazovka->inicializuj(800, 600, 0, 0);

	Kruznice k;
	k.x = 500;
	k.y = 400;
	k.r = 30;
	k.vx = 0;
	k.vy = 0;


	while(1)
	{
		t1 = SDL_GetTicks();
		SDL_FillRect(obrazovka->screen, NULL, 0);

		if(stage==1){r--;g++;}
		if(stage==2){g--;b++;}
		if(stage==3){b--;r++;}

		if(b==0&&r==255){stage=1;}
		if(r==0&&g==255){stage=2;}
		if(g==0&&b==255){stage=3;}

		barva(255,0,0);
		kruznice(k.x, k.y, k.r);
		bod(k.x,k.y);
		barva(255,255,255);
		cara(20,20,780,20);
		cara(780,580);
		cara(20,580);
		cara(20,20);

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
			case SDL_MOUSEBUTTONDOWN:
				k.vx += ( event.button.x - k.x ) / 50;
				k.vy += ( event.button.y - k.y ) / 50;
				break;
			}
		}

		if(k.x<50){k.vx = -k.vx/4*3; k.x=50;}
		if(k.x>750){k.vx = -k.vx/4*3; k.x=750;}
		if(k.y<50){k.vy = -k.vy/4*3; k.y=50;}

		if(k.vx>0){k.vx -= 0.01;}
		if(k.vx<0){k.vx += 0.01;}
		if(k.y==0&&k.vx>0){k.vx -= 0.05;}
		if(k.y==0&&k.vx<0){k.vx += 0.05;}
		if(k.vx<0.001&&k.vx>0||k.vx>-0.001&&k.vx<0){k.vx=0;}

		if(k.y<550){k.vy+=0.2;}
		if(k.y>550){k.y = 550; k.vy = -k.vy/4*3;}

		if(k.y>548&&k.vy>0||k.y>545&&k.vy<0){timer++;}
		if(k.y<=548){timer=0;}
		if(timer==10){timer=0; k.vy=0; k.y=550;}

		k.x+=k.vx;
		k.y+=k.vy;

		t2 = SDL_GetTicks();
		if(t2-t1<=17)
		{
			SDL_Delay(17-(t2-t1));
		}
	}
}
