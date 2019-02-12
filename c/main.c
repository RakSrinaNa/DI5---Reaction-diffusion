//
// Created by yateu on 10/02/2019.
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "headers/utils.h"
#include "headers/main.h"
#include "headers/gifenc.h"
#include "headers/constants.h"
#include "headers/individual.h"
#include "headers/generation.h"

#ifdef _WIN32
#include <dir.h>
#else
#include <sys/stat.h>

#endif

int main(int argv, char ** argc)
{
	printf("Starting...\n");
	
	srand(time(NULL));

#ifdef _WIN32
	mkdir("output");
#else
	mkdir("output", 0777);
#endif
	
	Generation * generation = GenerationCreate();
	for(int i = 0; i < 9; i++)
	{
		Parameters * parameters = ParametersCreate(getRandom(0, 1), getRandom(0, 1), (int) getRandom(0, 25), (int) getRandom(0, 25), getRandom(0, 1), getRandom(0, 1));
		Individual * individual = IndividualCreate(parameters);
		GenerationAppend(generation, individual);
	}
	
	GenerationGenerate(generation);
	GenerationDestroy(generation);
	
	return 0;
}
