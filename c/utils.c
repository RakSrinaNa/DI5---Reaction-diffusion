//
// Created by Thomas Couchoud on 2019-02-12.
//

#include <stdlib.h>
#include <sys/time.h>
#include "headers/utils.h"

double getRandom(double min, double max)
{
	double rnd = (double) rand() / (double) RAND_MAX;
	return min + rnd * (max - min);
}

int mod(int a, int b)
{
	int r = a % b;
	return r < 0 ? r + b : r;
}

long getMicrotime()
{
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}
