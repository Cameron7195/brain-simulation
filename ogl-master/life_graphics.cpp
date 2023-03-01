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
using namespace glm;

#include "common/shader.hpp"

GLFWwindow* window;
GLuint vertexBuffer, vertexColorBuffer;
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

void openWindow(int w, int h) {
    window = glfwCreateWindow(w, h, "Life Simulation", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
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

void drawVertices(int points, GLfloat g_vertex_pos_scale_data[], GLfloat g_vertex_color_data[]) {
    
    GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
    
    GLuint VertexArrayID;
    
    int size = 4*points*sizeof(GLfloat);
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, g_vertex_pos_scale_data, GL_STATIC_DRAW);
    
    glGenBuffers(1, &vertexColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, g_vertex_color_data, GL_STATIC_DRAW);
    
    
    
    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT );

    // Use our shader
    glUseProgram(programID);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        4,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );
    
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexColorBuffer);
    glVertexAttribPointer(
        1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        4,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_POINTS, 0, points);

    //glDisableVertexAttribArray(0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
    //glDeleteBuffers(1, &vertexbuffer);
    //glDeleteBuffers(1, &vertexColorBuffer);
    //glDeleteVertexArrays(1, &VertexArrayID);
    //glDeleteProgram(programID);
    // OPENGL IS HELLA LEAKY!!!!!!!!!
}

void updateVertices(int points, GLfloat g_vertex_pos_scale_data[], GLfloat g_vertex_color_data[]) {
    int size = 4*points*sizeof(GLfloat);
    glClear( GL_COLOR_BUFFER_BIT );
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, g_vertex_pos_scale_data, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, g_vertex_color_data, GL_STATIC_DRAW);
    
    glDrawArrays(GL_POINTS, 0, points);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

GLfloat *getPosVertexBuffer(int size, int w, int h, float *xArr, float *yArr, float *phiArr, float *vArr) {
    GLfloat *out = (GLfloat *)malloc(4*size*sizeof(GLfloat));
    for (int i=0;i<size;i++) {
        out[4*i] = 2*xArr[i]/w-1;       // X coord
        out[4*i+1] = 2*yArr[i]/h-1;     // Y coord
        out[4*i+2] = 0.05f;              // scale
        out[4*i+3] = 0.0f;              // Nothing
    }
    return out;
}

GLfloat *getColorVertexBuffer(int size, int w, int h, float *xArr, float *yArr, float *phiArr, float *vArr, float *localWeights, float *charge) {
    GLfloat *out = (GLfloat *)malloc(4*size*sizeof(GLfloat));
    for (int i=0;i<size;i++) {
        out[4*i] = 1/(1+0.05*vArr[i]);                // R
        out[4*i+1] = 1/(1+0.4*localWeights[i]);         // G
        out[4*i+2] = charge[i]/2 + 0.5;              // B
        out[4*i+3] = 1.0f;              // A
        if (i == 1002) {
            out[4*i] = 0.8f;                // R
            out[4*i+1] = 0.8f;         // G
            out[4*i+2] = 1.0f;              // B
            out[4*i+3] = 1.0f;              // A
        }
    }
    return out;
}
