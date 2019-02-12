//
// Created by Thomas Couchoud on 2019-02-12.
//

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "headers/individual.h"
#include "headers/constants.h"
#include "headers/utils.h"

Individual * IndividualCreate(Parameters * params)
{
	if(params == NULL)
		return NULL;
	Individual * individual = malloc(sizeof(Individual));
	individual->params = params;
	
	Cell ** board = malloc(sizeof(Cell *) * size_x);
	for(int x = 0; x < size_x; x++)
	{
		board[x] = malloc(sizeof(Cell) * size_y);
		for(int y = 0; y < size_y; y++)
		{
			board[x][y].a = getRandom(1, 100);
			board[x][y].i = getRandom(1, 100);
		}
	}
	individual->board = board;
	
	uint8_t palette[] = {0x5F, 0xEA, 0xDE, 0x8F, 0xDF, 0xCD, 0xBE, 0xD5, 0xBD, 0xEE, 0xCB, 0xAD, 0x8D, 0xAB, 0x8E, 0xF4, 0x8B, 0x6F, 0xF8, 0x6B, 0x50, 0xFB, 0x4B, 0x31};
	
	individual->outputfile = malloc(sizeof(char) * 1024);
	sprintf(individual->outputfile, "output/%lu.gif", getMicrotime());
	
	printf("Will write to file %s\n", individual->outputfile);
	ge_GIF * gif = ge_new_gif(individual->outputfile, size_x, size_y, palette, PALETTE_DEPTH, 0);
	individual->gif = gif;
	
	return individual;
}

void IndividualDestroy(Individual * individual)
{
	if(individual == NULL)
		return;
	ge_close_gif(individual->gif);
	
	for(int x = 0; x < size_x; x++)
		free(individual->board[x]);
	free(individual->board);
	free(individual->params);
	free(individual->outputfile);
	free(individual);
}

void IndividualGenerate(Individual * individual)
{
	int tick = 0;
	while(tick < max_tick)
	{
		printf("Processing tick %02d/%02d...\n", tick + 1, max_tick);
		IndividualReact(individual);
		IndividualDiffuse(individual);
		IndividualReduce(individual);
		uint8_t * image = IndividualThresholding(individual);
		tick++;
		IndividualDraw(individual, image);
		free(image);
	}
	
	printf("Done\n");
}

void IndividualDraw(Individual * individual, uint8_t * image)
{
	memcpy(individual->gif->frame, image, sizeof(uint8_t) * size_x * size_y);
	ge_add_frame(individual->gif, 100 / GIF_FPS);
}

void IndividualReact(Individual * individual)
{
	printf("\tReacting...\n");
	for(int x = 0; x < size_x; x++)
		for(int y = 0; y < size_y; y++)
		{
			cellReact(individual->params, &(individual->board[x][y]));
		}
}

void IndividualDiffuse(Individual * individual)
{
	printf("\tDiffusing...\n");
	int maxIteration = MAX(individual->params->diffusion_speed_a, individual->params->diffusion_speed_i);
	Cell ** originalBoard = malloc(sizeof(Cell) * size_x);
	for(int x = 0; x < size_x; x++)
		originalBoard[x] = malloc(sizeof(Cell) * size_y);
	for(int iteration = 0; iteration < maxIteration; iteration++)
	{
		for(int x = 0; x < size_x; x++)
		{
			memcpy(&originalBoard[x], &(individual->board[x]), sizeof(individual->board[x]));
		}
		for(int x = 0; x < size_x; x++)
			for(int y = 0; y < size_y; y++)
				for(int dx = -1; dx < 2; dx++)
					for(int dy = -1; dy < 2; dy++)
						if(dx != 0 || dy != 0)
						{
							int diffuseX = mod(x + dx, size_x);
							int diffuseY = mod(y + dy, size_y);
							if(iteration < individual->params->diffusion_speed_a)
							{
								cellDiffuseA(individual->params, &originalBoard[x][y], &(individual->board[diffuseX][diffuseY]), 8);
							}
							if(iteration < individual->params->diffusion_speed_i)
							{
								cellDiffuseI(individual->params, &originalBoard[x][y], &(individual->board[diffuseX][diffuseY]), 8);
							}
						}
	}
	for(int x = 0; x < size_x; x++)
		for(int y = 0; y < size_y; y++)
		{
			cellReact(individual->params, &(individual->board[x][y]));
		}
}

void IndividualReduce(Individual * individual)
{
	printf("\tReducing...\n");
	for(int x = 0; x < size_x; x++)
		for(int y = 0; y < size_y; y++)
		{
			cellReduce(individual->params, &(individual->board[x][y]));
		}
}

uint8_t * IndividualThresholding(Individual * individual)
{
	printf("\tThresholding...\n");
	double average = 0;
	for(int x = 0; x < size_x; x++)
		for(int y = 0; y < size_y; y++)
		{
			average += individual->board[x][y].a;
		}
	int colorsCount = (int) pow(2, PALETTE_DEPTH);
	int colorsMid = colorsCount / 2;
	double colorsStep = 1.0 / colorsMid;
	average /= size_x * size_y;
	uint8_t * image = malloc(sizeof(uint8_t) * size_x * size_y);
	for(int x = 0; x < size_x; x++)
		for(int y = 0; y < size_y; y++)
		{
			double value = individual->board[x][y].a;
			double coefficient = value / average;
			int color = colorsCount - 1;
			for(int i = 0; i < colorsCount; ++i)
			{
				if(1 + (i - colorsMid) * colorsStep >= coefficient)
				{
					color = i;
					break;
				}
			}
			image[x + y * size_x] = color;
		}
	return image;
}

Individual * IndividualCopy(Individual * individual)
{
	return IndividualCreate(ParametersCopy(individual->params));
}
