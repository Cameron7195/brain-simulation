//
//  main.cpp
//  life
//
//  Created by Cameron Witkowski on 2021-05-30.
//
// HELLO WORLD

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <iostream>
#include <ctime>
#include <thread>
#include <vector>
#include "life_graphics.h"
#include "Graph.hpp"


#include <GL/glew.h>


// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;
using namespace std;


#include "common/shader.hpp"

#define WIDTH 1420
#define HEIGHT 840
#define GRIDSIZE 100
#define CRITRADIUS 40
#define FPS 10000
#define H_TRANSFER 0.001
//Make sure gridsize is a factor of width and height!
//right now num grid squares is 144.


int main() {
    Graph brain(POINTS, BRAINWIDTH, 0, 1);
    GLFWwindow *window;
    
    float xArr[POINTS];
    float yArr[POINTS];
    int w = BRAINWIDTH;
    int h = POINTS/BRAINWIDTH;
    for (int i=0;i<POINTS;i++) {
        xArr[i] = WIDTH*2/3*(i%w)/(w-1) + WIDTH/6;
        yArr[i] = HEIGHT*2/3*(i/w)/(h-1) + HEIGHT*2/3/(h-1)/2*(i%2) + HEIGHT/6;
        //yArr[i] = HEIGHT*2/3*(i/w)/(h-1) + HEIGHT/6;
        printf("Point %d at: (%.2f, %.2f)\n", i, xArr[i], yArr[i]);
    }
    
    int numEdges = 0;
    for (int i=0;i<POINTS;i++) {
        numEdges += brain.getNeighbours(i).size();
    }
    
    printf("THERE ARE %d edges!!\n", numEdges);

    // Initialise GLFW
    initGLFW();
    openWindow(&window, WIDTH, HEIGHT);
    initGlew();
    // Open a window and create its OpenGL context
    
    // Ensure we can capture the escape key being pressed below

    // Dark black background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    
    int size = 4*POINTS*sizeof(GLfloat);
    
    //X, Y, size, R, G, B, A

    GLfloat *g_vertex_edge_data = getEdgeVertexBuffer(POINTS, numEdges, WIDTH, HEIGHT, xArr, yArr, brain);
    

    
    drawVertices(POINTS, numEdges, g_vertex_edge_data);
    clock_t tSim = clock();
    clock_t tDisp = clock();
    int simSteps = 0;
    float h_step = 1/float(FPS);
    while (1) {
        
        int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (state == GLFW_PRESS)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            ypos = HEIGHT - ypos;
            printf("Click at (%.2f, %.2f)\n", xpos, ypos);
            // Activate neuron closest to click.
            int closestNeuron = 0;
            double closestDist = 999999999;
            for (int i=0;i<POINTS;i++) {
                double dist = (xArr[i]-xpos)*(xArr[i]-xpos) + (yArr[i]-ypos)*(yArr[i]-ypos);
                if (dist < closestDist) {
                    closestDist = dist;
                    closestNeuron = i;
                }
            }
            if (closestNeuron == 0) {
                printf("Feeding brain...\n");
                brain.feed();
            }
            else {
                printf("Activating: %d\n", closestNeuron);
                brain.activate(closestNeuron);
            }
        }
        
        if (float(clock() - tDisp)/CLOCKS_PER_SEC > 0.001 || true) {
            brain.evolve();
            GLfloat *g_vertex_edge_data = getEdgeVertexBuffer(POINTS, numEdges, WIDTH, HEIGHT, xArr, yArr, brain);
            updateVertices(numEdges, g_vertex_edge_data);
            free(g_vertex_edge_data);
            tDisp = clock();
        }
        
        if (float(clock() - tSim)/CLOCKS_PER_SEC > h_step) {
            tSim = clock();
            simSteps += 1;
            if (simSteps % 1000 == 0) {
                printf("Simulation has completed %d timesteps\n", simSteps);
                int x=0;
            }
        }
    }
   
   return 0;
}
