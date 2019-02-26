//
// Created by Thomas Couchoud on 2019-02-12.
//

#ifndef TURING_PARAMETERS_H
#define TURING_PARAMETERS_H

#include <stdlib.h>
#ifdef _WIN32
#include <stdint-gcc.h>
#endif

#define PARAMETERS_COUNT 7

typedef struct _Parameters
{
	double reaction_rate_a;
	double reaction_rate_i;
	int diffusion_speed_a;
	int diffusion_speed_i;
	double reduction_rate;
	double diffusion_rate;
	int palette_size;
	uint8_t * palette;
} Parameters;

Parameters * ParametersCreate(double reaction_rate_a, double reaction_rate_i, int diffusion_speed_a, int diffusion_speed_i, double reduction_rate, double diffusion_rate);

Parameters * ParametersCreateRandom();

void ParametersDestroy(Parameters * parameters);

Parameters * ParametersCopy(Parameters * parameters);

void ParametersWriteToFile(Parameters * parameters, char * filepath);

#endif //TURING_PARAMETERS_H
