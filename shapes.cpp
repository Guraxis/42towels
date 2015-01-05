#include "shapes.h"
#include "obrazek.h"
#include "math.h"

Kruznice::Kruznice()
{
	vx = 0;
	vy = 0;
}

Rectangle::Rectangle()
{
	vx = 0;
	vy = 0;
}

Bod::Bod()
{
	vx = 0;
	vy = 0;
}

Cara::Cara()
{
	vx1 = 0;
	vy1 = 0;
	vx2 = 0;
	vy2 = 0;
}

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

void rectangle(int x1, int y1, int x2, int y2)
{
	for(int i=0; i<y1-y2; i++)
	{
		cara(x1,y1+i,x2,y1+i);
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

