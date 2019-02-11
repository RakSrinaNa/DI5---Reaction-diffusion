//
// Created by yateu on 10/02/2019.
//

#ifndef TURING_MAIN_H
#define TURING_MAIN_H

#include <stdint.h>
#include "constants.h"
#include "cell.h"
#include "gifenc.h"

int mod(int a, int b);

double getRandom(double min, double max);

void react(Cell ** board);

void diffuse(Cell ** board);

void reduce(Cell ** board);

uint8_t * thresholding(Cell ** board);

void draw(uint8_t * image, ge_GIF * pGIF);

#endif //TURING_MAIN_H
