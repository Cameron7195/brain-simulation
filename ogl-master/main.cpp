//
//  main.cpp
//  life
//
//  Created by Cameron Witkowski on 2021-05-30.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <ctime>
#include <thread>
#include <vector>
#include "life_inits.h"
#include "life_defs.h"
#include "life_graphics.h"

#include <GL/glew.h>


// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;
using namespace std;


#include "common/shader.hpp"

#define POINTS 3000
#define WIDTH 800
#define HEIGHT 800
#define GRIDSIZE 100
#define CRITRADIUS 40
#define FPS 10000
#define H_TRANSFER 0.001
#define CHARGEPCT 0.5
//Make sure gridsize is a factor of width and height!
//right now num grid squares is 144.



void testGridAssignments(float *xArr, float *yArr, int *gridArr, grid_ll **grid) {
   for (int i=0;i<POINTS;i++) {
      grid_ll *curNode = grid[gridArr[i]];
      printf("Point %d: (%.2f, %.2f) in Grid square %d\n", i, xArr[i], yArr[i], gridArr[i]);
      printf("Other pts in Grid square %d: ", gridArr[i]);

      while (curNode != NULL) {
         printf("%d, ", curNode->pt);
         curNode = curNode->next;
      }
      printf("\n\n");
   }
}

void updatePos(int n, int w, int h, int gridSize, int critRad, float *xArr, float *yArr, float *phiArr, float *vArr, int *gridIdx, grid_ll **grid) {
    
    for (int i=0;i<n;i++) {
        xArr[i] = xArr[i] + vArr[i] * cos(phiArr[i]);
        yArr[i] = yArr[i] + vArr[i] * sin(phiArr[i]);
        
        //Check if new positions are valid
        
        if (xArr[i] < 0) {xArr[i] = -xArr[i];}
        else if (xArr[i] >= w) {xArr[i] = 2*float(w) - xArr[i];}
        if (yArr[i] < 0) {yArr[i] = -yArr[i];}
        else if (yArr[i] >= h) {yArr[i] = 2*float(h) - yArr[i];}
        
        // Check if this particle changed grid squares, and modify if so.
        //printf("pt: %d at (%.2f, %.2f) in (old) grid square %d\n", i, xArr[i], yArr[i], gridIdx[i]);
        //for (int j=0;j<n;j++) {
        //    printf("    %d, (%.2f, %.2f)\n", j, xArr[j], yArr[j]);
        //}
        int numColumns = w/gridSize;
        float y = yArr[i];
        // Janky max function cuz fmax is incredibly slow for some weird reason.
        if (y > h-0.1) {
            y = h-0.1;
        }
        float x = xArr[i];
        if (x > w-0.1) {
            x = w-0.1;
        }
        int newIdx = floor(y / gridSize)*numColumns + floor(x / gridSize);
        int oldIdx = gridIdx[i];
        //printf("    OLD IDX: %d\n", oldIdx);
        //printf("    NEW IDX: %d\n", newIdx);
        if (newIdx != oldIdx) {
            grid_ll *g = (grid_ll *)malloc(sizeof(grid_ll));
            g->pt = i;
            g->next = nullptr;
            gridIdx[i] = newIdx;
            
            ll_append(&grid[newIdx], g);
            
            ll_remove(&grid[oldIdx], i);
            
        }
    }
}

void inline updater(int i, int doSquare, float *tempPhi, float *tempV, float *totalWeight, float *instaFactor, int n, int w, int h, float k, int gridSize, int critRad, float *xArr, float *yArr, float *phiArr, float *vArr, float *localWeights, float *charge, int *gridIdx, grid_ll **grid) {
    grid_ll *curNode = grid[doSquare];
    while (curNode) {
        int pnt = curNode->pt;
        if (i != pnt) {
            float dist = sqrt( (xArr[i]-xArr[pnt])*(xArr[i]-xArr[pnt]) + (yArr[i]-yArr[pnt])*(yArr[i]-yArr[pnt]) );
            if (dist < critRad) {
                float relAngle = vArr[i]*cos(phiArr[i])*(yArr[i]-yArr[pnt]) - vArr[i]*sin(phiArr[i])*(xArr[i]-xArr[pnt]);
                *tempPhi += 5*charge[i]*relAngle/(dist*dist*dist);
                *tempV += vArr[i]/(dist);
                *totalWeight += 1/(dist);
                *instaFactor += 1/(dist*dist*dist);
            }
        }
        curNode = curNode->next;
    }
    return;
}

void mainLoop(int n) {
    printf("N is %d", n);
}

void updatePhi(int n, int w, int h, float k, int gridSize, int critRad, float *xArr, float *yArr, float *phiArr, float *vArr, float *localWeights, float *charge, int *gridIdx, grid_ll **grid) {
    
    int gridRows = h/gridSize;
    int gridColumns = w/gridSize;

/*
    vector<thread> threads;
        
    threads.push_back(thread(mainLoop));
    
     
      for (auto &th : threads) {
        th.join();
      }
 */
    
    for (int i=0;i<n;i++) {
        //STEP 1: DETERMINE HOW MANY GRID SQUARES THIS POINT TOUCHES - Use x and y heuristic.
        // If point is CRITRADIUS or closer to a gridsquare edge, then count both squares.

        int localXPos = int(round(xArr[i])) % gridSize;
        int localYPos = int(round(yArr[i])) % gridSize;
        uint_t corner = 0; // 11 is NW square, 12 is NE, 21 is SW, 22 is SE.
        int doSquare = 0;
        
        float tempPhi = phiArr[i];
        float tempV = 0;
        float totalWeight = 0;
        float instaFactor = 0;
        
        //printf("Point %d is in grid square %d at (%.2f, %.2f)\n", i, gridIdx[i], xArr[i], yArr[i]);

        if (localXPos < critRad && gridIdx[i] % gridColumns != 0) { // Count square to the left as well
            corner += 1;
            doSquare = gridIdx[i] - 1;
            updater(i, doSquare, &tempPhi, &tempV, &totalWeight, &instaFactor, n, w, h, k, gridSize, critRad, xArr, yArr, phiArr, vArr, localWeights, charge, gridIdx, grid);
            //printf("    Counting grid square %d too\n", doSquare);
        }
        else if (localXPos > gridSize - critRad && gridIdx[i] % gridColumns != gridColumns-1) { // Count square to the right as well
            corner += 2;
            doSquare = gridIdx[i] + 1;
            updater(i, doSquare, &tempPhi, &tempV, &totalWeight, &instaFactor, n, w, h, k, gridSize, critRad, xArr, yArr, phiArr, vArr, localWeights, charge, gridIdx, grid);
            //printf("    Counting grid square %d too\n", doSquare);
        }

        if (localYPos < critRad && gridIdx[i] >= gridColumns) { // Count square above as well
            corner += 10;
            doSquare = gridIdx[i] - gridColumns;
            updater(i, doSquare, &tempPhi, &tempV, &totalWeight, &instaFactor, n, w, h, k, gridSize, critRad, xArr, yArr, phiArr, vArr, localWeights, charge, gridIdx, grid);
            //printf("    Counting grid square %d too\n", doSquare);
        }
        else if (localYPos > gridSize - critRad && gridIdx[i] < (gridRows-1) * gridColumns) { // Count square below as well
            corner += 20;
            doSquare = gridIdx[i] + gridColumns;
            updater(i, doSquare, &tempPhi, &tempV, &totalWeight, &instaFactor, n, w, h, k, gridSize, critRad, xArr, yArr, phiArr, vArr, localWeights, charge, gridIdx, grid);
            //printf("    Counting grid square %d too\n", doSquare);
        }

        if (corner > 10 && corner != 20) { // We gotta do a corner square damn.
            if (corner == 11) { //NW
                doSquare = gridIdx[i] - gridColumns - 1;
                updater(i, doSquare, &tempPhi, &tempV, &totalWeight, &instaFactor, n, w, h, k, gridSize, critRad, xArr, yArr, phiArr, vArr, localWeights, charge, gridIdx, grid);
                //printf("    Counting grid square %d too\n", doSquare);
            }
            else if (corner == 12) { //NE
                doSquare = gridIdx[i] - gridColumns + 1;
                updater(i, doSquare, &tempPhi, &tempV, &totalWeight, &instaFactor, n, w, h, k, gridSize, critRad, xArr, yArr, phiArr, vArr, localWeights, charge, gridIdx, grid);
                //printf("    Counting grid square %d too\n", doSquare);
            }
            else if (corner == 21) { //SW
                doSquare = gridIdx[i] + gridColumns - 1;
                updater(i, doSquare, &tempPhi, &tempV, &totalWeight, &instaFactor, n, w, h, k, gridSize, critRad, xArr, yArr, phiArr, vArr, localWeights, charge, gridIdx, grid);
                //printf("    Counting grid square %d too\n", doSquare);
            }
            else if (corner == 22) { //SE
                doSquare = gridIdx[i] + gridColumns + 1;
                updater(i, doSquare, &tempPhi, &tempV, &totalWeight, &instaFactor, n, w, h, k, gridSize, critRad, xArr, yArr, phiArr, vArr, localWeights, charge, gridIdx, grid);
                //printf("    Counting grid square %d too\n", doSquare);
            }
            else {
                printf("PROBLEM!!!! BAD CORNER THIS SHOULD NOT HAPPEN!!! corner = %d\n", corner);
            }
        }

        // Do current square:
        doSquare = gridIdx[i];
        //printf("    Counting grid square %d NOW\n", doSquare);

        updater(i, doSquare, &tempPhi, &tempV, &totalWeight, &instaFactor, n, w, h, k, gridSize, critRad, xArr, yArr, phiArr, vArr, localWeights, charge, gridIdx, grid);

        if (vArr[i] > 15) {
            phiArr[i] = tempPhi;
        }
        else if (charge[i] > 0) {
            phiArr[i] = tempPhi + M_PI_2;
        }
        else {
            phiArr[i] = tempPhi + M_PI_2;
        }
        
        
        if (phiArr[i] > 2*M_PI) {
            phiArr[i] -= 2*M_PI;
        }
        else if (phiArr[i] < -2*M_PI) {
            phiArr[i] += 2*M_PI;
        }
        
        
        // Fix friction / heat transfer
        float localAvgSpeed;
        if (totalWeight == 0) {
            localAvgSpeed = vArr[i];
        }
            else {
            localAvgSpeed = tempV/totalWeight;
            }
        vArr[i] = 0.99*(vArr[i] + k*localAvgSpeed)/(1+k)
        + 0.04*(xArr[i]*xArr[i]/w/w + yArr[i]*yArr[i]/h/h) + fmin(instaFactor/300000, 30);
        
        localWeights[i] = totalWeight;
       
       
    //STEP 3: update positions.
   }
}

void testLL() {
    grid_ll *root = nullptr;
    grid_ll *a0 = (grid_ll *)malloc(sizeof(grid_ll));
    a0->pt = 0;
    a0->next = nullptr;
    ll_append(&root, a0);
    
    
    grid_ll *a1 = (grid_ll *)malloc(sizeof(grid_ll));
    a1->pt = 1;
    a1->next = nullptr;
    ll_append(&root, a1);
    
    
    grid_ll *a2 = (grid_ll *)malloc(sizeof(grid_ll));
    a2->pt = 2;
    a2->next = nullptr;
    ll_append(&root, a2);
    
    
    grid_ll *curNode = root;
    int level = 0;
    while (curNode != nullptr && level < 20) {
        int pt = curNode->pt;
        for (int i=0;i<level;i++) {printf(" ");}
        printf("pt: %d\n", pt);
        level += 1;
        curNode = curNode->next;
    }
    
    ll_remove(&root, 2);
    curNode = root;
    level = 0;
    while (curNode != nullptr && level < 20) {
        int pt = curNode->pt;
        for (int i=0;i<level;i++) {printf(" ");}
        printf("pt: %d\n", pt);
        level += 1;
        curNode = curNode->next;
    }
}

int main() {
    float *xArr, *yArr, *phiArr, *vArr, *localWeights, *charge;
    int *gridArr;
    grid_ll **grid;
    
    //testLL();


    //return;
    
    srand(10000);

    initPoints(POINTS, &xArr, &yArr, &phiArr, &vArr, &gridArr, &localWeights, &charge, CHARGEPCT);

    initGrid(GRIDSIZE, WIDTH, HEIGHT, &grid);

    randomizeArr(POINTS, xArr, 0, WIDTH);
    randomizeArr(POINTS, yArr, 0, HEIGHT);
    randomizeArr(POINTS, phiArr, 0, 2*M_PI);
    randomizeArr(POINTS, vArr, 0.2, 1);
    


    printf("No errors?\n");

    initGridIdx(POINTS, GRIDSIZE, WIDTH, HEIGHT, xArr, yArr, gridArr, grid);

    //testGridAssignments(xArr, yArr, gridArr, grid);


    grid_ll g, g1;
    g.pt = 0;
    g.next = &g1;
    g1.pt = 1;

    // Initialise GLFW
    initGLFW();
    openWindow(WIDTH, HEIGHT);
    initGlew();
    // Open a window and create its OpenGL context
    
    // Ensure we can capture the escape key being pressed below

    // Dark black background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


    // Create and compile our GLSL program from the shaders

/*
    GLfloat g_vertex_pos_scale_data[] = {
         0.0f, 0.0f, 0.2f, 0.0f,
         0.5f, 0.5f, 0.02f, 0.0f,
         0.8f, 0.8f, 0.04f, 0.0f
    };
    GLfloat g_vertex_color_data[] = {
        1.0f, 0.0f, 0.0f, 0.9f,
        0.8f, 1.0f, 1.0f, 0.7f,
        1.0f, 0.4f, 1.0f, 0.3f
    };*/

    //int s = sizeof(g_vertex_pos_scale_data);
    
    int size = 4*POINTS*sizeof(GLfloat);
    
    //X, Y, size, R, G, B, A
    GLfloat *g_vertex_pos_scale_data = getPosVertexBuffer(POINTS, WIDTH, HEIGHT, xArr, yArr, phiArr, vArr);
    GLfloat *g_vertex_color_data = getColorVertexBuffer(POINTS, WIDTH, HEIGHT, xArr, yArr, phiArr, vArr, localWeights, charge);
    drawVertices(POINTS, g_vertex_pos_scale_data, g_vertex_color_data);
    clock_t tSim = clock();
    clock_t tDisp = clock();
    int simSteps = 0;
    float h = 1/float(FPS);
    while (1) {
        
        if (float(clock() - tDisp)/CLOCKS_PER_SEC > 0.02) {
            GLfloat *g_vertex_pos_scale_data = getPosVertexBuffer(POINTS, WIDTH, HEIGHT, xArr, yArr, phiArr, vArr);
            GLfloat *g_vertex_color_data = getColorVertexBuffer(POINTS, WIDTH, HEIGHT, xArr, yArr, phiArr, vArr, localWeights, charge);
            updateVertices(POINTS, g_vertex_pos_scale_data, g_vertex_color_data);
            free(g_vertex_pos_scale_data);
            free(g_vertex_color_data);
            tDisp = clock();
        }
        
        if (float(clock() - tSim)/CLOCKS_PER_SEC > h) {
            updatePhi(POINTS, WIDTH, HEIGHT, H_TRANSFER, GRIDSIZE, CRITRADIUS, xArr, yArr, phiArr, vArr, localWeights, charge, gridArr, grid);
            updatePos(POINTS, WIDTH, HEIGHT, GRIDSIZE, CRITRADIUS, xArr, yArr, phiArr, vArr, gridArr, grid);
            tSim = clock();
            simSteps += 1;
            if (simSteps % 1000 == 0) {
                printf("Simulation has completed %d timesteps\n", simSteps);
            }
        }
    }
   
   return 0;
}
