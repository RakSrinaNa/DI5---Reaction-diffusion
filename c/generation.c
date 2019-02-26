//
// Created by Thomas Couchoud on 2019-02-12.
//

#include <stdlib.h>
#include <pthread.h>
#include "headers/generation.h"

Generation * GenerationCreate()
{
	Generation * generation = malloc(sizeof(Generation));
	generation->populationSize = 0;
	generation->individuals = NULL;
	return generation;
}

void GenerationDestroy(Generation * generation)
{
	for(int i = 0; i < generation->populationSize; i++)
		IndividualDestroy(generation->individuals[i]);
	free(generation->individuals);
	free(generation);
}

int GenerationAppend(Generation * generation, Individual * individual)
{
	generation->populationSize++;
	Individual ** temp = realloc(generation->individuals, sizeof(Individual *) * generation->populationSize);
	if(temp != NULL)
	{
		generation->individuals = temp;
		generation->individuals[generation->populationSize - 1] = individual;
		return 0;
	}
	generation->populationSize--;
	return 1;
}

void GenerationGenerate(Generation * generation)
{
	pthread_t * threads = malloc(sizeof(pthread_t) * generation->populationSize);
	
	for(int i = 0; i < generation->populationSize; ++i)
	{
		pthread_create(threads + i, NULL, IndividualGenerateThread, (void *) (generation->individuals[i]));
	}
	
	/* wait for threads to finish */
	for(int i = 0; i < generation->populationSize; ++i)
	{
		pthread_join(threads[i], NULL);
	}
	//	for(int i = 0; i < generation->populationSize; i++)
	//		IndividualGenerate(generation->individuals[i]);
}
