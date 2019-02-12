//
// Created by Thomas Couchoud on 2019-02-12.
//

#include "headers/parameters.h"

Parameters * ParametersCreate(double reaction_rate_a, double reaction_rate_i, int diffusion_speed_a, int diffusion_speed_i, double reduction_rate, double diffusion_rate)
{
	Parameters * parameters = malloc(sizeof(Parameters));
	parameters->reaction_rate_a = reaction_rate_a;
	parameters->reaction_rate_i= reaction_rate_i;
	parameters->diffusion_speed_a = diffusion_speed_a;
	parameters->diffusion_speed_i = diffusion_speed_i;
	parameters->reduction_rate = reduction_rate;
	parameters->diffusion_rate = diffusion_rate;
	return parameters;
}
