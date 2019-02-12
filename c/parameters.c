//
// Created by Thomas Couchoud on 2019-02-12.
//

#include <math.h>
#include <string.h>
#include "headers/parameters.h"
#include "headers/utils.h"
#include "headers/constants.h"

Parameters * ParametersCreate(double reaction_rate_a, double reaction_rate_i, int diffusion_speed_a, int diffusion_speed_i, double reduction_rate, double diffusion_rate)
{
	Parameters * parameters = malloc(sizeof(Parameters));
	parameters->reaction_rate_a = reaction_rate_a;
	parameters->reaction_rate_i = reaction_rate_i;
	parameters->diffusion_speed_a = diffusion_speed_a;
	parameters->diffusion_speed_i = diffusion_speed_i;
	parameters->reduction_rate = reduction_rate;
	parameters->diffusion_rate = diffusion_rate;
	parameters->palette_size = (int) pow(2, PALETTE_DEPTH) * 3;
	parameters->palette = malloc(sizeof(uint8_t) * parameters->palette_size);
	return parameters;
}

Parameters * ParametersCreateRandom()
{
	Parameters * parameters = ParametersCreate(getRandom(0, 1), getRandom(0, 1), (int) getRandom(0, 25), (int) getRandom(0, 25), getRandom(0, 1), getRandom(0, 1));
	for(int colorIndex = 0; colorIndex < parameters->palette_size; colorIndex++)
	{
		parameters->palette[colorIndex] = (uint8_t) getRandom(0, 256);
	}
	return parameters;
}

void ParametersDestroy(Parameters * parameters)
{
	free(parameters->palette);
	free(parameters);
}

Parameters * ParametersCopy(Parameters * parameters)
{
	Parameters * copy = malloc(sizeof(Parameters));
	copy->reaction_rate_a = parameters->reaction_rate_a;
	copy->reaction_rate_i = parameters->reaction_rate_i;
	copy->diffusion_speed_a = parameters->diffusion_speed_a;
	copy->diffusion_speed_i = parameters->diffusion_speed_i;
	copy->reduction_rate = parameters->reduction_rate;
	copy->diffusion_rate = parameters->diffusion_rate;
	copy->palette_size = parameters->palette_size;
	memcpy(copy->palette, parameters->palette, sizeof(uint8_t) * parameters->palette_size);
	return copy;
}
