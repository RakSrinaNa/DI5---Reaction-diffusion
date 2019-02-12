//
// Created by yateu on 10/02/2019.
//

#include <math.h>
#include "headers/cell.h"
#include "headers/constants.h"

void cellDiffuseA(Parameters * parameters, Cell * pCellSource, Cell * pCellDestination, int coefficient)
{
	pCellDestination->a += parameters->diffusion_rate * pCellSource->a / coefficient;
}

void cellDiffuseI(Parameters * parameters, Cell * pCellSource, Cell * pCellDestination, int coefficient)
{
	pCellDestination->i += parameters->diffusion_rate * pCellSource->i / coefficient;
}

void cellReact(Parameters * parameters, Cell * pCell)
{
	double previousI = pCell->i;
	double aPow2 = pow(pCell->a, 2);
	pCell->i += parameters->reaction_rate_i * aPow2;
	pCell->a += (parameters->reaction_rate_a * aPow2) / previousI;
}

void cellReduce(Parameters * parameters, Cell * pCell)
{
	pCell->i *= 1 - parameters->reduction_rate;
	pCell->a *= 1 - parameters->reduction_rate;
}
