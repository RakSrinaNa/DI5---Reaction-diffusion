//
// Created by Thomas Couchoud on 2019-02-12.
//

#ifndef TURING_GENERATION_H
#define TURING_GENERATION_H

#include "individual.h"

typedef struct  _Generation{
	int populationSize;
	Individual ** individuals;
}Generation;

Generation * GenerationCreate();

void GenerationDestroy(Generation * generation);

void GenerationGenerate(Generation * generation);

int GenerationAppend(Generation * generation, Individual * individual);

#endif //TURING_GENERATION_H
