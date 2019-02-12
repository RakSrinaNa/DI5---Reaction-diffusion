//
// Created by Thomas Couchoud on 2019-02-12.
//

#ifndef TURING_INDIVIDUAL_H
#define TURING_INDIVIDUAL_H

#include "cell.h"
#include "gifenc.h"
#include "parameters.h"

typedef struct _Individual{
	Parameters * params;
	ge_GIF * gif;
	char * outputfile;
	Cell ** board;
} Individual;

Individual * IndividualCreate(Parameters * params);

void IndividualDestroy(Individual * individual);

void IndividualGenerate(Individual * individual);

void IndividualDraw(Individual * individual, uint8_t * image);

void IndividualReact(Individual * individual);

void IndividualDiffuse(Individual * individual);

void IndividualReduce(Individual * individual);

uint8_t * IndividualThresholding(Individual * individual);

#endif //TURING_INDIVIDUAL_H
