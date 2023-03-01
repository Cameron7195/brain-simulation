//
//  life_inits.cpp
//  life
//
//  Created by Cameron Witkowski on 2021-05-30.
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "life_inits.h"
#include "life_defs.h"


void initPoints(int n, float **xArr, float **yArr, float **phiArr, float **vArr, int **gridArr, float **localWeights, float **charge, float chargePct) {
    *xArr = (float *)malloc(n*sizeof(float));
    *yArr = (float *)malloc(n*sizeof(float));
    *phiArr = (float *)malloc(n*sizeof(float));
    *vArr = (float *)malloc(n*sizeof(float));
    *gridArr = (int *)malloc(n*sizeof(int));
    *localWeights = (float *)malloc(n*sizeof(float));
    *charge = (float *)malloc(n*sizeof(float));
    for (int i=0;i<n;i++) {
        (*xArr)[i] = 0;
        (*yArr)[i] = 0;
        (*phiArr)[i] = 0;
        (*vArr)[i] = 0;
        (*gridArr)[i] = 0;
        (*localWeights)[i] = 0;
        if (float(rand())/float(RAND_MAX) < chargePct) {
            (*charge)[i] = 1;
        }
        else {
            (*charge)[i] = -1;
        }
        printf("chargei: %f\n", (*charge)[i]);
    }
    return;
}

void initGrid(int gridSize, int w, int h, grid_ll ***grid) {
    int n = w*h/gridSize/gridSize;
    *grid = (grid_ll **)malloc(n*sizeof(grid_ll *));
    for (int i=0;i<n;i++) {
        (*grid)[i] = nullptr;
    }
    return;
}

void randomizeArr(int n, float *arr, float min, float max) {
   for (int i=0;i<n;i++) {
      arr[i] = (max-min)*float(rand())/float((RAND_MAX)) + min;
   }
}

void initGridIdx(int n, int gridSize, int w, int h, float *xArr, float *yArr, int *gridIdx, grid_ll **grid) {
    int numColumns = w/gridSize;

    for (int i=0;i<n;i++) {
        grid_ll *g = (grid_ll *)malloc(sizeof(grid_ll));
        g->pt = i;
        g->next = nullptr;

        int idx = floor(yArr[i] / gridSize)*numColumns + floor(xArr[i] / gridSize);
        gridIdx[i] = idx;
        ll_append(&grid[idx], g);
    }
}
