//
//  life_inits.h
//  life
//
//  Created by Cameron Witkowski on 2021-05-30.
//
#include "life_defs.h"

#ifndef life_inits_h
#define life_inits_h
void initPoints(int n, float **xArr, float **yArr, float **phiArr, float **vArr, int **gridArr, float **localWeights, float **charge, float chargePct);
void initGrid(int gridSize, int w, int h, grid_ll ***grid);
void randomizeArr(int n, float *arr, float min, float max);
void initGridIdx(int n, int gridSize, int w, int h, float *xArr, float *yArr, int *gridIdx, grid_ll **grid);
#endif /* life_inits_h */
