#ifndef NAHODA_H
#define NAHODA_H
#include <SDL/SDL.h>

float nahoda(float min, float max)
{
	return (float)rand()/RAND_MAX*(max-min)+min;
}

#endif // NAHODA_H
