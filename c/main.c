//
// Created by yateu on 10/02/2019.
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "headers/main.h"
#include "headers/gifenc.h"
#include "headers/constants.h"

#ifdef _WIN32
#include <dir.h>
#else
#include <sys/stat.h>
#endif

#define MAX(x, y) (x >= y ? x : y)
#define PALETTE_DEPTH 3

int main(int argv, char ** argc)
{
	printf("Starting...\n");
	
	srand(time(NULL));

#ifdef _WIN32
	mkdir("output");
#else
	mkdir("output", 0777);
#endif
	
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
	
	uint8_t palette[] = {0x5F, 0xEA, 0xDE, 0x8F, 0xDF, 0xCD, 0xBE, 0xD5, 0xBD, 0xEE, 0xCB, 0xAD, 0x8D, 0xAB, 0x8E, 0xF4, 0x8B, 0x6F, 0xF8, 0x6B, 0x50, 0xFB, 0x4B, 0x31};
	
	char filename[128];
	sprintf(filename, "output/%lu.gif", (unsigned long) time(NULL));
	
	printf("Will write to file %s\n", filename);
	ge_GIF * gif = ge_new_gif(filename, size_x, size_y, palette, PALETTE_DEPTH, 0);
	
	int tick = 0;
	while(tick < max_tick)
	{
		printf("Processing tick %02d/%02d...\n", tick + 1, max_tick);
		react(board);
		diffuse(board);
		reduce(board);
		uint8_t * image = thresholding(board);
		tick++;
		draw(image, gif);
		free(image);
	}
	ge_close_gif(gif);
	
	for(int x = 0; x < size_x; x++)
		free(board[x]);
	free(board);
	
	printf("Done\n");
	return 0;
}

void draw(uint8_t * image, ge_GIF * pGIF)
{
	memcpy(pGIF->frame, image, sizeof(uint8_t) * size_x * size_y);
	ge_add_frame(pGIF, 100 / GIF_FPS);
}

void react(Cell ** board)
{
	printf("\tReacting...\n");
	for(int x = 0; x < size_x; x++)
		for(int y = 0; y < size_y; y++)
		{
			cellReact(&(board[x][y]));
		}
}

void diffuse(Cell ** board)
{
	printf("\tDiffusing...\n");
	int maxIteration = MAX(diffusion_speed_a, diffusion_speed_i);
	Cell ** originalBoard = malloc(sizeof(Cell) * size_x);
	for(int x = 0; x < size_x; x++)
		originalBoard[x] = malloc(sizeof(Cell) * size_y);
	for(int iteration = 0; iteration < maxIteration; iteration++)
	{
		for(int x = 0; x < size_x; x++)
		{
			memcpy(&originalBoard[x], &board[x], sizeof(board[x]));
		}
		for(int x = 0; x < size_x; x++)
			for(int y = 0; y < size_y; y++)
				for(int dx = -1; dx < 2; dx++)
					for(int dy = -1; dy < 2; dy++)
						if(dx != 0 || dy != 0)
						{
							int diffuseX = mod(x + dx, size_x);
							int diffuseY = mod(y + dy, size_y);
							if(iteration < diffusion_speed_a)
							{
								cellDiffuseA(&originalBoard[x][y], &board[diffuseX][diffuseY], 8);
							}
							if(iteration < diffusion_speed_i)
							{
								cellDiffuseI(&originalBoard[x][y], &board[diffuseX][diffuseY], 8);
							}
						}
	}
	for(int x = 0; x < size_x; x++)
		for(int y = 0; y < size_y; y++)
		{
			cellReact(&(board[x][y]));
		}
}

void reduce(Cell ** board)
{
	printf("\tReducing...\n");
	for(int x = 0; x < size_x; x++)
		for(int y = 0; y < size_y; y++)
		{
			cellReduce(&(board[x][y]));
		}
}

uint8_t * thresholding(Cell ** board)
{
	printf("\tThresholding...\n");
	double average = 0;
	for(int x = 0; x < size_x; x++)
		for(int y = 0; y < size_y; y++)
		{
			average += board[x][y].a;
		}
	int colorsCount = (int) pow(2, PALETTE_DEPTH);
	int colorsMid = colorsCount / 2;
	double colorsStep = 1.0 / colorsMid;
	average /= size_x * size_y;
	uint8_t * image = malloc(sizeof(uint8_t) * size_x * size_y);
	for(int x = 0; x < size_x; x++)
		for(int y = 0; y < size_y; y++)
		{
			double value = board[x][y].a;
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

double getRandom(double min, double max)
{
	double rnd = (double) rand() / (double) RAND_MAX;
	return min + rnd * (max - min);
}

int mod(int a, int b)
{
	int r = a % b;
	return r < 0 ? r + b : r;
}