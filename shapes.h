#ifndef SHAPES_H
#define SHAPES_H

class Kruznice
{
public:
	float x, y, r, vx, vy;
	Kruznice();
};

class Rectangle
{
public:
	float x, y, w, h, vx, vy;
	Rectangle();
};

class Bod
{
public:
	float x, y, vx, vy;
	Bod();
};

class Cara
{
public:
	float x1, y1, x2, y2, vx1, vy1, vx2, vy2;
	Cara();
};

void kruznice(int x, int y, int r);
void kruznice(Kruznice a);
void rectangle(Rectangle a);
void hrectangle(Rectangle a);
void triangle(int x, int y, int w, int h);
void triangle(int x1, int y1, int x2, int y2, int x3, int y3);
void hvezda(int x, int y, int r);

#endif // SHAPES_H
