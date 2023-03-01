//
//  life_graphics.cpp
//  tutorial02_red_triangle
//
//  Created by Cameron Witkowski on 2021-06-02.
//

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Graph.hpp"
#include "life_graphics.h"
#include <GLUT/GLUT.h>
using namespace glm;

#include "common/shader.hpp"

GLuint vertexEdgeBuffer;
GLFWwindow* window;
int viewMode = 0;

void initGLFW() {
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void openWindow(GLFWwindow **win, int w, int h) {
    window = glfwCreateWindow(w, h, "Brain", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    *win = window;
}

void initGlew() {
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void drawVertices(int points, int edges, GLfloat g_vertex_edge_data[]) {
    
    GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
    
    GLuint VertexArrayID;
    
    int size = 8*edges*sizeof(GLfloat);
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    glGenBuffers(1, &vertexEdgeBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexEdgeBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, g_vertex_edge_data, GL_STATIC_DRAW);
    
    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT );

    // Use our shader
    glUseProgram(programID);
    
    // attribute array : edges
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        4,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Draw the triangle !
    
    
    glDrawArrays(GL_POINTS, 0, 2*edges);
    glDrawArrays(GL_LINES, 0, 2*edges);

    //glDisableVertexAttribArray(0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void updateVertices(int edges, GLfloat g_vertex_edge_data[]) {
    
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexEdgeBuffer);
    glBufferData(GL_ARRAY_BUFFER, 8*edges*sizeof(GLfloat), g_vertex_edge_data, GL_DYNAMIC_DRAW);
    
    glDrawArrays(GL_POINTS, 0, 2*edges);
    
    glDrawArrays(GL_LINES, 0, 2*edges);
    
    //glBindBuffer(GL_ARRAY_BUFFER, )
    glfwSwapBuffers(window);
    glfwPollEvents();
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f );
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    int state = glfwGetKey(window, GLFW_KEY_V);
    if (state == GLFW_PRESS)
    {
        if (viewMode == 0) {
            viewMode = 1;
        }
        else if (viewMode == 1){
            viewMode = 2;
        }
        else {
            viewMode = 0;
        }
    }
    
}

GLfloat *getPosVertexBuffer(int size, int w, int h, float *xArr, float *yArr) {
    GLfloat *out = (GLfloat *)malloc(4*size*sizeof(GLfloat));
    for (int i=0;i<size;i++) {
        out[4*i] = 2*xArr[i]/w-1;       // X coord
        out[4*i+1] = 2*yArr[i]/h-1;     // Y coord
        out[4*i+2] = 0.05f;              // scale
        out[4*i+3] = 0.0f;              // Nothing
    }
    return out;
}

GLfloat *getColorVertexBuffer(int size, int w, int h, float *xArr, float *yArr) {
    GLfloat *out = (GLfloat *)malloc(4*size*sizeof(GLfloat));
    for (int i=0;i<size;i++) {
        out[4*i] = 1.0f;                // R
        out[4*i+1] = 1.0f;         // G
        out[4*i+2] = 1.0f;              // B
        out[4*i+3] = 1.0f;              // A
    }
    return out;
}

GLfloat *getEdgeVertexBuffer(int size, int edges, int w, int h, float *xArr, float *yArr, Graph g) {
    int i;
    GLfloat *out = (GLfloat *)malloc(8*edges*sizeof(GLfloat));
    int idx = 0;
    for (i=0;i<size;i++) {
        int j = 0;
        vector< Edge > neighbours = g.getNeighbours(i);
        for (j=0;j<neighbours.size();j++) {
            float maxWeight = std::max(0.8*neighbours[j].weight, 0.8*neighbours[j].reverseEdge->weight);
            if (viewMode == 0) {
                out[8*idx] = 2*xArr[i]/w-1;       // X coord
                out[8*idx+1] = 2*yArr[i]/h-1;     // Y coord
                out[8*idx+2] = neighbours[j].weight;
                out[8*idx+3] = 1/(1+GLfloat(g.getActivation()[i])) + g.food[i];
                out[8*idx+4] = 2*xArr[neighbours[j].pt]/w-1;       // X coord
                out[8*idx+5] = 2*yArr[neighbours[j].pt]/h-1;       // Y coord
                out[8*idx+6] = neighbours[j].reverseEdge->weight;
                out[8*idx+7] = 1/(1+GLfloat(g.getActivation()[neighbours[j].pt])) + g.food[i];
                idx += 1;
            }
            else if (viewMode == 1) {
                out[8*idx] = 2*xArr[i]/w-1;       // X coord
                out[8*idx+1] = 2*yArr[i]/h-1;     // Y coord
                out[8*idx+2] = 0.4*neighbours[j].weight;
                out[8*idx+3] = g.getOxygen()[i].amount/(g.getOxygen()[i].amount + 50);
                out[8*idx+4] = 2*xArr[neighbours[j].pt]/w-1;       // X coord
                out[8*idx+5] = 2*yArr[neighbours[j].pt]/h-1;       // Y coord
                out[8*idx+6] = 0.4*neighbours[j].reverseEdge->weight;
                out[8*idx+7] = g.getOxygen()[neighbours[j].pt].amount/(g.getOxygen()[neighbours[j].pt].amount + 50);
                idx += 1;
            }
            else if (viewMode == 2) {
                
                float x = (i%BRAINWIDTH - neighbours[j].pt%BRAINWIDTH);
                float y = (i/BRAINWIDTH - neighbours[j].pt/BRAINWIDTH);
                
                
                out[8*idx] = 2*xArr[i]/w-1;       // X coord
                out[8*idx+1] = 2*yArr[i]/h-1;     // Y coord
                out[8*idx+2] = 0.0*neighbours[j].weight;
                out[8*idx+3] = 1.0/(500.0 - g.potentialField[i]);
                out[8*idx+4] = 2*xArr[neighbours[j].pt]/w-1;       // X coord
                out[8*idx+5] = 2*yArr[neighbours[j].pt]/h-1;       // Y coord
                out[8*idx+6] = 0.0*neighbours[j].reverseEdge->weight;
                out[8*idx+7] = 1.0/(500.0 - g.potentialField[neighbours[j].pt]);
                idx += 1;
            }
        }
    }
    return out;
}
