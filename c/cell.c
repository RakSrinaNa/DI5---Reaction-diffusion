//
// Created by yateu on 10/02/2019.
//

#include <math.h>
#include "headers/cell.h"
#include "headers/constants.h"

void cellDiffuseA(Cell * pCellSource, Cell * pCellDestination, int coefficient)
{
	pCellDestination->a += diffusion_rate * pCellSource->a / coefficient;
}

void cellDiffuseI(Cell * pCellSource, Cell * pCellDestination, int coefficient)
{
	pCellDestination->i += diffusion_rate * pCellSource->i / coefficient;
}

void cellReact(Cell * pCell)
{
	double previousI = pCell->i;
	double aPow2 = pow(pCell->a, 2);
	pCell->i += reaction_rate_i * aPow2;
	pCell->a += (reaction_rate_a * aPow2) / previousI;
}

void cellReduce(Cell * pCell)
{
	pCell->i *= 1 - reduction_rate;
	pCell->a *= 1 - reduction_rate;
}
