//
// Created by yateu on 10/02/2019.
//

#ifndef TURING_CELL_H
#define TURING_CELL_H

typedef struct _Cell{
	double a;
	double i;
} Cell;

void cellReact(Cell * pCell);

void cellDiffuseA(Cell * pCellSource, Cell * pCellDestination, int coefficient);

void cellDiffuseI(Cell * pCellSource, Cell * pCellDestination, int coefficient);

void cellReduce(Cell * pCell);

#endif //TURING_CELL_H
