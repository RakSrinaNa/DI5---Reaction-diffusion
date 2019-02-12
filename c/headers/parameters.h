//
// Created by Thomas Couchoud on 2019-02-12.
//

#ifndef TURING_PARAMETERS_H
#define TURING_PARAMETERS_H

#include <stdlib.h>

#define PARAMETERS_COUNT 6

typedef struct _Parameters
{
	double reaction_rate_a;
	double reaction_rate_i;
	int diffusion_speed_a;
	int diffusion_speed_i;
	double reduction_rate;
	double diffusion_rate;
} Parameters;

Parameters * ParametersCreate(double reaction_rate_a, double reaction_rate_i, int diffusion_speed_a, int diffusion_speed_i, double reduction_rate, double diffusion_rate);

Parameters * ParametersCreateRandom();

Parameters * ParametersCopy(Parameters * parameters);

#endif //TURING_PARAMETERS_H
