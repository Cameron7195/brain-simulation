//
//  life_graphics.h
//  life
//
//  Created by Cameron Witkowski on 2021-06-03.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

#include "common/shader.hpp"
#include "Graph.hpp"

#ifndef life_graphics_h
#define life_graphics_h

void initGLFW();
void openWindow(GLFWwindow **win, int w, int h);
void initGlew();
void drawVertices(int points, int edges, GLfloat g_vertex_edge_data[]);
void updateVertices(int edges, GLfloat g_vertex_edge_data[]);
void closeWindow();
GLfloat *getEdgeVertexBuffer(int size, int edges, int w, int h, float *xArr, float *yArr, Graph g);

#endif /* life_graphics_h */
