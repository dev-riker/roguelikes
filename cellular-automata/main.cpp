/*
 *
 * Copyright (c) 2014, Alessandro Sperinde'
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the {organization} nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TILE_FLOOR 0
#define TILE_WALL 1

typedef struct {
    int r1Cutoff;
    int r2Cutoff;
    int reps;
} generation_params_t;

int **grid;
int **grid2;

int fillprob = 40;
int r1_cutoff = 5;
int r2_cutoff = 2;
int size_x = 64;
int size_y = 20;

generation_params_t *params;
generation_params_t *paramsSet;

int generations;

int RandPick(void)
{
    if (rand() % 100 < fillprob) {
        return TILE_WALL;
    }

    return TILE_FLOOR;
}

void InitMap(void)
{
    int xi, yi;

    grid  = (int **) malloc(sizeof(int *) * size_y);
    grid2 = (int **) malloc(sizeof(int *) * size_y);

    for(yi = 0; yi < size_y; yi++) {
        grid [yi] = (int *) malloc(sizeof(int) * size_x);
        grid2[yi] = (int *) malloc(sizeof(int) * size_x);
    }

    for (yi = 1; yi < (size_y - 1); yi++) {
        for (xi = 1; xi < (size_x - 1); xi++) {
            grid[yi][xi] = RandPick();
        }
    }

    for (yi = 0; yi < size_y; yi++) {
        for (xi = 0; xi < size_x; xi++) {
            grid2[yi][xi] = TILE_WALL;
        }
    }

    for (yi = 0; yi < size_y; yi++) {
        grid[yi][0] = grid[yi][size_x - 1] = TILE_WALL;
    }
    for (xi = 0; xi < size_x; xi++) {
        grid[0][xi] = grid[size_y - 1][xi] = TILE_WALL;
    }
}

void Generation(void)
{
    int xi, yi, ii, jj;

    for (yi = 1; yi < (size_y - 1); yi++) {
        for (xi = 1; xi < (size_x - 1); xi++) {
            int adjcount_r1 = 0;
            int adjcount_r2 = 0;

            for (ii = -1; ii <= 1; ii++) {
                for (jj = -1; jj <= 1; jj++) {
                    if (grid[yi + ii][xi + jj] != TILE_FLOOR) {
                        adjcount_r1++;
                    }
                }
            }

            for (ii = (yi - 2); ii <= (yi + 2); ii++) {
                for (jj = (xi - 2); jj <= (xi + 2); jj++) {
                    if (abs(ii - yi) == 2 && abs(jj - xi) == 2) {
                        continue;
                    }
                    if (ii < 0 || jj < 0 || ii >= size_y || jj >= size_x) {
                        continue;
                    }
                    if (grid[ii][jj] != TILE_FLOOR) {
                        adjcount_r2++;
                    }
                }
            }

            if (adjcount_r1 >= params->r1Cutoff || adjcount_r2 <= params->r2Cutoff) {
                grid2[yi][xi] = TILE_WALL;
            } else {
                grid2[yi][xi] = TILE_FLOOR;
            }
        }
    }

    for (yi = 1; yi < (size_y - 1); yi++) {
        for (xi = 1; xi < (size_x - 1); xi++) {
            grid[yi][xi] = grid2[yi][xi];
        }
    }
}

void PrintFunc(void)
{
    int ii;

    std::cout<<"W[0](p) = rand[0,100) < "<<fillprob<<"\n";

    for (ii = 0; ii < generations; ii++) {
        std::cout<<"Repeat "<<paramsSet[ii].reps<<": W'(p) = R[1](p) >= "<<paramsSet[ii].r1Cutoff;

        if (paramsSet[ii].r2Cutoff >= 0) {
            std::cout<<" || R[2](p) <= "<<paramsSet[ii].r2Cutoff<<"\n";
        } else {
            std::cout<<std::endl;
        }
    }
}

void PrintMap(void)
{
    int xi, yi;

    for (yi = 0; yi < size_y; yi++) {
        for (xi = 0; xi < size_x; xi++) {
            switch (grid[yi][xi]) {
                case TILE_WALL:
                    std::cout<<"#";
                    break;
                case TILE_FLOOR:
                    std::cout<<".";
                    break;
            }
        }
        std::cout<<std::endl;
    }
}

int main(int argc, char **argv)
{
    int ii, jj;

    if (argc < 7) {
        std::cerr<<"Usage: "<<argv[0]<<" xsize ysize fill (r1 r2 count)\n";
        return 1;
    }

    size_x     = atoi(argv[1]);
    size_y     = atoi(argv[2]);
    fillprob   = atoi(argv[3]);

    generations = (argc - 4) / 3;

    params = paramsSet = (generation_params_t *) malloc( sizeof(generation_params_t) * generations );

    for (ii = 4; (ii + 2) < argc; ii += 3) {
        params->r1Cutoff = atoi(argv[ii]);
        params->r2Cutoff = atoi(argv[ii + 1]);
        params->reps = atoi(argv[ii + 2]);
        params++;
    }

    srand(time(NULL));

    InitMap();

    for (ii = 0; ii < generations; ii++) {
        params = &paramsSet[ii];
        for (jj = 0; jj < params->reps; jj++) {
            Generation();
        }
    }

    PrintFunc();
    PrintMap();

    return 0;
}
