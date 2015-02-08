/*
 * Kim Arre
 * CPE 471 - Winter 2015
 * Section 01
 */


// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <common/shader.hpp>

#include <algorithm>
#include <random>

int g_height = 800;
int g_width = 800;

void window_size_callback(GLFWwindow* window, int width, int height) {
    g_height = height;
    g_width = width;
    
    glViewport(0, 0, g_width, g_height);
}

int main( void )
{
    float l_time = 0;
    
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow( g_width, g_height, "Tutorial 02 - Red triangle", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    glfwSetWindowSizeCallback(window, window_size_callback);
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Dark blue background
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
    
    // Get a handle for our buffers
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
    GLuint winScaleID = glGetUniformLocation(programID, "windowScale");
    GLuint winCenterID = glGetUniformLocation(programID, "center");
    GLuint timeID = glGetUniformLocation(programID, "time");
    GLuint isBackgroundID = glGetUniformLocation(programID, "isBackground");
    GLuint backgroundPositionID = glGetUniformLocation(programID, "backgroundPosition");
    
    float vertices[80] = {};
    
    for (int i=0; i<80; i++) {
        vertices[i] = (float)rand()/(float)RAND_MAX*2.0f - 1.0f;
    }
    
    int count = 0;
    
    static const GLfloat g_vertex_buffer_data[] = {
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
        vertices[count++], vertices[count++], 0.0,    vertices[count++], vertices[count++], 0.0,
    };
    
    static const GLfloat g_color_buffer_data[] = {
        .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,
        .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,
        .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,
        .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,
        .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,
        .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,
        .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,
        .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,
        .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,
        .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,    .6, .075, 1.0,
    };
    
    static const GLfloat g_background_data[] = {
        -1.0, -1.0, 0.0,
        0.0, 1.0, 0.0,
        1.0, -1.0, 0.0,
        /*-2.0, 2.0, 0.0,
         2.0, 2.0, 0.0,
         0.0, -3.0, 0.0,*/
    };
    
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    GLuint backgroundbuffer;
    glGenBuffers(1, &backgroundbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, backgroundbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_background_data), g_background_data, GL_STATIC_DRAW);
    
    
    do{
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );
        
        // Use our shader
        glUseProgram(programID);
        
        float windowScale;
        if (g_width > g_height) {
            windowScale = g_height/(float)g_width;
            glUniform2f(winScaleID, windowScale, 1);
        }
        else {
            windowScale = g_width/(float)g_height;
            glUniform2f(winScaleID, 1, windowScale);
        }
        
        glUniform2f(winCenterID, g_width/2.0, g_height/2.0);
        glUniform1f(timeID, l_time);
        
        
        //determines where the fragment goes
        
        l_time += .0025;  // how fast it moves
        if (l_time >= 1.2) {  //when to reset and start animation over
            l_time = 0;
        }
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            vertexPosition_modelspaceID, // The attribute we want to configure
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
        
        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,               // The attribute we want to configure
            3,               // size
            GL_FLOAT,        // type
            GL_FALSE,        // normalized?
            0,               // stride
            (void*)0         // array buffer offset
        );
        
        // Draw the triangle !
        
        glUniform1i(isBackgroundID, 0);
        glDrawArrays(GL_POINTS, 0, 40);
        glPointSize(25.0);
        
        
        glDisableVertexAttribArray(vertexPosition_modelspaceID);
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(backgroundPositionID);
        glBindBuffer(GL_ARRAY_BUFFER, backgroundbuffer);
        glVertexAttribPointer(
             backgroundPositionID, // The attribute we want to configure
             3,                  // size
             GL_FLOAT,           // type
             GL_FALSE,           // normalized?
             0,                  // stride
             (void*)0            // array buffer offset
        );
        
        glUniform1i(isBackgroundID, 1);
        glDrawArrays(GL_TRIANGLES, 0, 9);
        
        glDisableVertexAttribArray(backgroundPositionID);
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteProgram(programID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}

