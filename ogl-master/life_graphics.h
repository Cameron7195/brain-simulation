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

#ifndef life_graphics_h
#define life_graphics_h
void initGLFW();
void openWindow(int w, int h);
void initGlew();
void drawVertices(int points, GLfloat g_vertex_pos_scale_data[], GLfloat g_vertex_color_data[]);
void updateVertices(int points, GLfloat g_vertex_pos_scale_data[], GLfloat g_vertex_color_data[]);
void closeWindow();
GLfloat *getPosVertexBuffer(int size, int w, int h, float *xArr, float *yArr, float *phiArr, float *vArr);
GLfloat *getColorVertexBuffer(int size, int w, int h, float *xArr, float *yArr, float *phiArr, float *vArr, float *localWeights, float *charge);

#endif /* life_graphics_h */
