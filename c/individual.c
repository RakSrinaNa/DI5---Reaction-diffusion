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
	individual->parameters = params;
	
	Cell ** board = malloc(sizeof(Cell *) * SIZE_X);
	for(int x = 0; x < SIZE_X; x++)
	{
		board[x] = malloc(sizeof(Cell) * SIZE_Y);
		for(int y = 0; y < SIZE_Y; y++)
		{
			board[x][y].a = getRandom(1, 100);
			board[x][y].i = getRandom(1, 100);
		}
	}
	individual->board = board;
	individual->gif = NULL;
	individual->outputfile = malloc(sizeof(char) * 1024);
	sprintf(individual->outputfile, "output/%lu-%d.gif", getMicroTime(), (int) getRandom(0, 1000));
	
	return individual;
}

void IndividualDestroy(Individual * individual)
{
	if(individual == NULL)
		return;
	
	for(int x = 0; x < SIZE_X; x++)
		free(individual->board[x]);
	free(individual->board);
	ParametersDestroy(individual->parameters);
	free(individual->outputfile);
	free(individual);
}

void IndividualGenerate(Individual * individual)
{
	individual->gif = ge_new_gif(individual->outputfile, SIZE_X, SIZE_Y, individual->parameters->palette, PALETTE_DEPTH, 0);
	printf("Will write to file %s\n", individual->outputfile);
	
	char parametersPath[1024];
	sprintf(parametersPath, "%s.json", individual->outputfile);
	ParametersWriteToFile(individual->parameters, parametersPath);
	int tick = 0;
	while(tick < MAX_TICK)
	{
		printf("%s => Processing tick %02d/%02d...\n", individual->outputfile, tick + 1, MAX_TICK);
		IndividualReact(individual);
		IndividualDiffuse(individual);
		IndividualReduce(individual);
		uint8_t * image = IndividualThresholding(individual);
		tick++;
		IndividualDraw(individual, image);
		
//		char file[1024];
//		sprintf(&file, "%s-%d.txt", individual->outputfile, tick);
//		FILE * ff = fopen(file, "w");
//		for(int i = 0; i < SIZE_X; i++)
//		{
//			for(int j = 0; j < SIZE_Y; j++)
//			{
//				fprintf(ff, "%lf ", individual->board[i][j].a);
//			}
//			fprintf(ff, "\n");
//		}
//		fclose(ff);
		free(image);
	}
	ge_close_gif(individual->gif);
	
	printf("Done\n");
}

void * IndividualGenerateThread(void * individual)
{
	IndividualGenerate(individual);
	return NULL;
}

void IndividualDraw(Individual * individual, uint8_t * image)
{
	memcpy(individual->gif->frame, image, sizeof(uint8_t) * SIZE_X * SIZE_Y);
	ge_add_frame(individual->gif, 100 / GIF_FPS);
}

void IndividualReact(Individual * individual)
{
	printf("\tReacting...\n");
	for(int x = 0; x < SIZE_X; x++)
		for(int y = 0; y < SIZE_Y; y++)
		{
			cellReact(individual->parameters, &(individual->board[x][y]));
		}
}

void IndividualDiffuse(Individual * individual)
{
	printf("\tDiffusing...\n");
	int maxIteration = MAX(individual->parameters->diffusion_speed_a, individual->parameters->diffusion_speed_i);
	Cell ** originalBoard = malloc(sizeof(Cell) * SIZE_X);
	for(int x = 0; x < SIZE_X; x++)
		originalBoard[x] = malloc(sizeof(Cell) * SIZE_Y);
	for(int iteration = 0; iteration < maxIteration; iteration++)
	{
		for(int x = 0; x < SIZE_X; x++)
		{
			memcpy(&originalBoard[x], &(individual->board[x]), sizeof(individual->board[x]));
		}
		for(int x = 0; x < SIZE_X; x++)
			for(int y = 0; y < SIZE_Y; y++)
				for(int dx = -1; dx < 2; dx++)
					for(int dy = -1; dy < 2; dy++)
						if(dx != 0 || dy != 0)
						{
							int diffuseX = mod(x + dx, SIZE_X);
							int diffuseY = mod(y + dy, SIZE_Y);
							if(iteration < individual->parameters->diffusion_speed_a)
							{
								cellDiffuseA(individual->parameters, &originalBoard[x][y], &(individual->board[diffuseX][diffuseY]), 8);
							}
							if(iteration < individual->parameters->diffusion_speed_i)
							{
								cellDiffuseI(individual->parameters, &originalBoard[x][y], &(individual->board[diffuseX][diffuseY]), 8);
							}
						}
	}
	for(int x = 0; x < SIZE_X; x++)
		for(int y = 0; y < SIZE_Y; y++)
		{
			cellReact(individual->parameters, &(individual->board[x][y]));
		}
}

void IndividualReduce(Individual * individual)
{
	printf("\tReducing...\n");
	for(int x = 0; x < SIZE_X; x++)
		for(int y = 0; y < SIZE_Y; y++)
		{
			cellReduce(individual->parameters, &(individual->board[x][y]));
		}
}

uint8_t * IndividualThresholding(Individual * individual)
{
	printf("\tThresholding...\n");
	double average = 0;
	for(int x = 0; x < SIZE_X; x++)
		for(int y = 0; y < SIZE_Y; y++)
		{
			average += individual->board[x][y].a;
		}
	int colorsCount = (int) pow(2, PALETTE_DEPTH);
	int colorsMid = colorsCount / 2;
	double colorsStep = 1.0 / colorsMid;
	average /= SIZE_X * SIZE_Y;
	uint8_t * image = malloc(sizeof(uint8_t) * SIZE_X * SIZE_Y);
	for(int x = 0; x < SIZE_X; x++)
		for(int y = 0; y < SIZE_Y; y++)
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
			image[x + y * SIZE_X] = color;
		}
	return image;
}

Individual * IndividualCopy(Individual * individual)
{
	return IndividualCreate(ParametersCopy(individual->parameters));
}
