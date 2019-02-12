//
// Created by yateu on 10/02/2019.
//

#ifndef TURING_CELL_H
#define TURING_CELL_H

#include "parameters.h"

typedef struct _Cell{
	double a;
	double i;
} Cell;

void cellReact(Parameters * parameters, Cell * pCell);

void cellDiffuseA(Parameters * parameters, Cell * pCellSource, Cell * pCellDestination, int coefficient);

void cellDiffuseI(Parameters * parameters, Cell * pCellSource, Cell * pCellDestination, int coefficient);

void cellReduce(Parameters * parameters, Cell * pCell);

#endif //TURING_CELL_H
