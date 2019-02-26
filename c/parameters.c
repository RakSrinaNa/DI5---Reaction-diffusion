//
// Created by Thomas Couchoud on 2019-02-12.
//

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "headers/parameters.h"
#include "headers/utils.h"
#include "headers/constants.h"

#ifdef _WIN32
#include <stdint-gcc.h>
#endif

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
	copy->palette = malloc(sizeof(uint8_t) * parameters->palette_size);
	memcpy(copy->palette, parameters->palette, sizeof(uint8_t) * parameters->palette_size);
	return copy;
}

void ParametersWriteToFile(Parameters * parameters, char * filepath)
{
	FILE * file = fopen(filepath, "w");
	fprintf(file, "{\n");
	fprintf(file, "\t\"reaction_rate_a\": %f,\n", parameters->reaction_rate_a);
	fprintf(file, "\t\"reaction_rate_i\": %f,\n", parameters->reaction_rate_i);
	fprintf(file, "\t\"diffusion_speed_a\": %d,\n", parameters->diffusion_speed_a);
	fprintf(file, "\t\"diffusion_speed_i\": %d,\n", parameters->diffusion_speed_i);
	fprintf(file, "\t\"reduction_rate\": %f,\n", parameters->reduction_rate);
	fprintf(file, "\t\"diffusion_rate\": %f,\n", parameters->diffusion_rate);
	fprintf(file, "\t\"colors\": {\n");
	int paletteCount = parameters->palette_size / 3;
	for(int paletteIndex = 0; paletteIndex < paletteCount; paletteIndex++)
	{
		fprintf(file, "\t\t\"%d\": {\n", paletteIndex);
		fprintf(file, "\t\t\t\"R\": %d,\n", parameters->palette[paletteIndex * 3]);
		fprintf(file, "\t\t\t\"G\": %d,\n", parameters->palette[paletteIndex * 3 + 1]);
		fprintf(file, "\t\t\t\"B\": %d\n", parameters->palette[paletteIndex * 3 + 2]);
		fprintf(file, "\t\t}%c\n", paletteIndex == (paletteCount - 1) ? ' ' : ',');
	}
	fprintf(file, "\t},\n");
	fprintf(file, "\t\"image_size\": {\n");
	fprintf(file, "\t\t\"x\": %d,\n", SIZE_X);
	fprintf(file, "\t\t\"y\": %d\n", SIZE_Y);
	fprintf(file, "\t},\n");
	fprintf(file, "\t\"ticks\": %d,\n", MAX_TICK);
	fprintf(file, "\t\"fps\": %d\n", GIF_FPS);
	fprintf(file, "}\n");
	fclose(file);
}
