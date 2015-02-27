/* Kim Arre
 * CPE 471
 * February 19, 2015
 */

#include "GLIncludes.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <cassert>
#include <cmath>

#include "GLSL.h"

#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "RenderingHelper.h"
#include "givenFunctions.h"

GLFWwindow* window;
using namespace std;

vector<tinyobj::shape_t> shapes;
vector<tinyobj::material_t> materials;

int g_SM = 0;
int g_width;
int g_height;
float g_Camtrans = -2.5;
float g_angle = 0;
float g_vertAngle = 0.0;
int g_mat_id =0;
glm::vec3 g_trans(0, 0, 0);
glm::vec3 g_light(2, 6, 6);

float forcedY = 0.0;

GLuint ShadeProg;
GLuint posBufObj = 0;
GLuint norBufObj = 0;
GLuint indBufObj = 0;

GLuint posBufObjG = 0;
GLuint norBufObjG = 0;

int drawNormals = 0;

glm::vec3 lookAt = glm::vec3(0, 0, -1);
glm::vec3 eye = glm::vec3(0, 0, 0);
glm::vec3 up = glm::vec3(0, 1, 0);
double phi = 0.0;  //pitch
double theta = 0.0; //yaw

RenderingHelper ModelTrans;

//Handles to the shader data
GLint h_aPosition;
GLint h_aNormal;
GLint h_uModelMatrix;
GLint h_uViewMatrix;
GLint h_uProjMatrix;
GLint h_uLightPos;
GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;
GLint h_uShadeM;
GLint h_cameratrans;

GLint colorByNormalsID;

/* helper function to make sure your matrix handle is correct */
inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]) {
  if (handle >= 0)
    glUniformMatrix4fv(handle, 1, GL_FALSE, data);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
   phi -= .1*yoffset;
   phi = min(phi, .8);
   phi = max(phi, -.8);

   theta += .1*xoffset;

   // calculate look at location
   float x = .5*cos(phi)*cos(theta);
   float y = .5*sin(phi);
   float z = .5*cos(phi)*cos(90-theta);

   lookAt = glm::vec3(x, y, z) + eye;
}

void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods) {  
   float x = .5*cos(phi)*cos(theta);
   float y = .5*sin(phi);
   float z = .5*cos(phi)*cos(90-theta);

   lookAt = glm::vec3(x, y, z) + eye;

   // RIGHT
   if (key == GLFW_KEY_D && action == GLFW_PRESS) {
      eye += glm::cross(lookAt - eye, up) * .25f; 
      lookAt += glm::cross(lookAt - eye, up) * .25f;
   }
   // LEFT
   else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
      eye -= glm::cross(lookAt - eye, up) * .25f; 
      lookAt -= glm::cross(lookAt - eye, up) * .25f;
   }
   
   // FORWARD
   else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
      eye += glm::vec3(x, y, z) * glm::vec3(.25, forcedY, .25);
      lookAt += glm::vec3(x, y, z) * glm::vec3(.25, forcedY, .25);
   }
   // BACK
   else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
      eye -= glm::vec3(x, y, z) * glm::vec3(.25, forcedY, .25);
      lookAt -= glm::vec3(x, y, z) * glm::vec3(.25, forcedY, .25);
   }
}

/* helper function to send materials to the shader - you must create your own */
void SetMaterial(int i) {
 
  glUseProgram(ShadeProg);
  switch (i) {
    case 0: //shiny blue plastic
        glUniform3f(h_uMatAmb, 0.02, 0.02, 0.1);
        glUniform3f(h_uMatDif, 0.0, 0.08, 0.5);
        glUniform3f(h_uMatSpec, 0.14, 0.14, 0.4);
        glUniform1f(h_uMatShine, 120.0);
        break;
    case 1: // flat grey
        glUniform3f(h_uMatAmb, 0.13, 0.13, 0.14);
        glUniform3f(h_uMatDif, 0.3, 0.3, 0.4);
        glUniform3f(h_uMatSpec, 0.3, 0.3, 0.4);
        glUniform1f(h_uMatShine, 4.0);
        break;
    case 2: //gold
        glUniform3f(h_uMatAmb, 0.09, 0.07, 0.08);
        glUniform3f(h_uMatDif, 0.91, 0.782, 0.82);
        glUniform3f(h_uMatSpec, 1.0, 0.913, 0.8);
        glUniform1f(h_uMatShine, 200.0);
        break;
    case 3: // SUPER AWESOME ROSE GOLD BUNNY
        glUniform3f(h_uMatAmb, 0.63, 0.63, 0.63);
        glUniform3f(h_uMatDif, 0.96, 0.47, 0.22);
        glUniform3f(h_uMatSpec, .96, 0.6, 0.23);
        glUniform1f(h_uMatShine, 175.0);
        break;
  }
}

/* helper function to set projection matrix - don't touch */
void SetProjectionMatrix() {
  glm::mat4 Projection = glm::perspective(90.0f, (float)g_width/g_height, 0.1f, 100.f);
  safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(Projection));
}

/* camera controls - do not change beyond the current set up to rotate*/
void SetView() {
  //glm::mat4 Trans = glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0, g_Camtrans));
  glm::mat4 Trans = glm::lookAt(eye, lookAt, up);
  safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(Trans));
}

/* model transforms */
void SetModel() {
  glm::mat4 Trans = glm::translate( glm::mat4(1.0f), g_trans);
  glm::mat4 RotateY = glm::rotate( glm::mat4(1.0f), g_angle, glm::vec3(0.0f, 1, 0));
  glm::mat4 RotateX = glm::rotate( glm::mat4(1.0f), g_vertAngle, glm::vec3(1, 0, 0));
  glm::mat4 com = Trans*RotateY*RotateX;
  safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(com));
}

void SetLightModel() {
  glm::mat4 Trans = glm::translate( glm::mat4(1.0f), g_light);
  
  glm::mat4 com = Trans;
  safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(com));
}

void loadShapes(const string &objFile)
{
   string err = tinyobj::LoadObj(shapes, materials, objFile.c_str());
   if(!err.empty()) {
      cerr << err << endl;
   }
   resize_obj(shapes);
}

void initGround() {

  float G_edge = 20;
  GLfloat g_backgnd_data[] = {
      -G_edge, -6.7f, -G_edge,
        -G_edge,  -6.7f, G_edge,
        G_edge, -6.7f, -G_edge,
        -G_edge,  -6.7f, G_edge,
        G_edge, -6.7f, -G_edge,
        G_edge, -6.7f, G_edge,
  };


  GLfloat nor_Buf_G[] = { 
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
  };

   glGenBuffers(1, &posBufObjG);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjG);
   glBufferData(GL_ARRAY_BUFFER, sizeof(g_backgnd_data), g_backgnd_data, GL_STATIC_DRAW);
   
   glGenBuffers(1, &norBufObjG);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObjG);
   glBufferData(GL_ARRAY_BUFFER, sizeof(nor_Buf_G), nor_Buf_G, GL_STATIC_DRAW);

}

void initGL()
{
   initGround();

   // Set the background color
   glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
   // Enable Z-buffer test
   glEnable(GL_DEPTH_TEST);
   glPointSize(18);
   
   // Send the position array to the GPU
   const vector<float> &posBuf = shapes[0].mesh.positions;
   glGenBuffers(1, &posBufObj);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
   glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
   
   // TODO compute the normals per vertex - you must fill this in 
   vector<float> norBuf;
   int idx1, idx2, idx3;
   glm::vec3 v1, v2, v3;

   //for every vertex initialize a normal to 0
   for (int j = 0; j < shapes[0].mesh.positions.size()/3; j++) {
      norBuf.push_back(0);
      norBuf.push_back(0);
      norBuf.push_back(0);
   }
   // DO work here to compute the normals for every face
   //then add its normal to its associated vertex
   for (int i = 0; i < shapes[0].mesh.indices.size()/3; i++) {
      idx1 = shapes[0].mesh.indices[3*i+0];
      idx2 = shapes[0].mesh.indices[3*i+1];
      idx3 = shapes[0].mesh.indices[3*i+2];
      v1 = glm::vec3(shapes[0].mesh.positions[3*idx1 +0],shapes[0].mesh.positions[3*idx1 +1], shapes[0].mesh.positions[3*idx1 +2]); 
      v2 = glm::vec3(shapes[0].mesh.positions[3*idx2 +0],shapes[0].mesh.positions[3*idx2 +1], shapes[0].mesh.positions[3*idx2 +2]); 
      v3 = glm::vec3(shapes[0].mesh.positions[3*idx3 +0],shapes[0].mesh.positions[3*idx3 +1], shapes[0].mesh.positions[3*idx3 +2]); 

      // Calculate two vectors from the three points 
      v1 = v1 - v2;
      v2 = v2 - v3;

      // Take the cross product of the two vectors to get the normal vector 
      
      glm::vec3 out = glm::vec3(glm::cross(v1, v2)); 
      
      norBuf[3*idx1+0] += out.x;
      norBuf[3*idx1+1] += out.y;
      norBuf[3*idx1+2] += out.z;

      norBuf[3*idx2+0] += out.x;
      norBuf[3*idx2+1] += out.y;
      norBuf[3*idx2+2] += out.z;

      norBuf[3*idx3+0] += out.x;
      norBuf[3*idx3+1] += out.y;
      norBuf[3*idx3+2] += out.z;
   }

   for (int i = 0; i < norBuf.size(); i += 3) {
      glm::vec3 toNormalize = glm::vec3(norBuf[i+0], norBuf[i+1], norBuf[i+2]);
      toNormalize = glm::normalize(toNormalize);
      norBuf[i+0] = toNormalize.x;
      norBuf[i+1] = toNormalize.y;
      norBuf[i+2] = toNormalize.z;
   }

   glGenBuffers(1, &norBufObj);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObj);
   glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
   
   // Send the index array to the GPU
   const vector<unsigned int> &indBuf = shapes[0].mesh.indices;
   glGenBuffers(1, &indBufObj);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObj);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

   // Unbind the arrays
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   GLSL::checkVersion();
   assert(glGetError() == GL_NO_ERROR);

   //initialize the modeltrans matrix stack
   ModelTrans.useModelViewMatrix();
   ModelTrans.loadIdentity();

   initGround();

}

bool installShaders(const string &vShaderName, const string &fShaderName)
{
   GLint rc;
   
   // Create shader handles
   GLuint VS = glCreateShader(GL_VERTEX_SHADER);
   GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
   
   // Read shader sources
   const char *vshader = GLSL::textFileRead(vShaderName.c_str());
   const char *fshader = GLSL::textFileRead(fShaderName.c_str());
   glShaderSource(VS, 1, &vshader, NULL);
   glShaderSource(FS, 1, &fshader, NULL);
   
   // Compile vertex shader
   glCompileShader(VS);
   GLSL::printError();
   glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
   GLSL::printShaderInfoLog(VS);
   if(!rc) {
      printf("Error compiling vertex shader %s\n", vShaderName.c_str());
      return false;
   }
   
   // Compile fragment shader
   glCompileShader(FS);
   GLSL::printError();
   glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
   GLSL::printShaderInfoLog(FS);
   if(!rc) {
      printf("Error compiling fragment shader %s\n", fShaderName.c_str());
      return false;
   }
   
   // Create the program and link
      ShadeProg = glCreateProgram();
      glAttachShader(ShadeProg, VS);
      glAttachShader(ShadeProg, FS);
      glLinkProgram(ShadeProg);
   
      GLSL::printError();
      glGetProgramiv(ShadeProg, GL_LINK_STATUS, &rc);
      GLSL::printProgramInfoLog(ShadeProg);
      if(!rc) {
         printf("Error linking shaders %s and %s\n", vShaderName.c_str(), fShaderName.c_str());
         return false;
      }

   /* get handles to attribute data */
    h_aPosition = GLSL::getAttribLocation(ShadeProg, "aPos");
    h_aNormal = GLSL::getAttribLocation(ShadeProg, "aNor");
    h_uProjMatrix = GLSL::getUniformLocation(ShadeProg, "P");
    h_uViewMatrix = GLSL::getUniformLocation(ShadeProg, "V");
    h_uModelMatrix = GLSL::getUniformLocation(ShadeProg, "M");
    //uMV = GLSL::getUniformLocation(ShadeProg, "MV");
    h_uLightPos = GLSL::getUniformLocation(ShadeProg, "uLightPos");
    h_uMatAmb = GLSL::getUniformLocation(ShadeProg, "UaColor");
    h_uMatDif = GLSL::getUniformLocation(ShadeProg, "UdColor");
    h_uMatSpec = GLSL::getUniformLocation(ShadeProg, "UsColor");
    h_uMatShine = GLSL::getUniformLocation(ShadeProg, "Ushine");
    h_uShadeM = GLSL::getUniformLocation(ShadeProg, "uShadeModel");
    h_cameratrans = GLSL::getUniformLocation(ShadeProg, "cameraTrans");
    colorByNormalsID = GLSL::getUniformLocation(ShadeProg, "colorByNormals");
   
   assert(glGetError() == GL_NO_ERROR);
   return true;
}


void drawGL()
{
   // Clear the screen
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   // Use our GLSL program
   glUseProgram(ShadeProg);

    SetProjectionMatrix();
    //SetView();

    SetModel();
    SetMaterial(g_mat_id);
    glUniform3f(h_uLightPos, g_light.x, g_light.y, g_light.z);
    glUniform1i(h_uShadeM, g_SM);
    glUniform1f(colorByNormalsID, drawNormals);
    glUniform1f(h_cameratrans, g_Camtrans);

   // Enable and bind position array for drawing
   GLSL::enableVertexAttribArray(h_aPosition);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
   glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Enable and bind normal array for drawing
   GLSL::enableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObj);
   glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Bind index array for drawing
   int nIndices = (int)shapes[0].mesh.indices.size();
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObj);
   
   //glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
   ModelTrans.loadIdentity();
   ModelTrans.lookAt(eye, lookAt, up);
   //ModelTrans.translate(glm::vec3(-.8 + translate, 0, 0));  //global trans and rotate
   ModelTrans.scale(.4, .4, .4);
   ModelTrans.translate(glm::vec3(0, 0, 0));  //global trans and rotate
   //ModelTrans.pushMatrix();
      
      ModelTrans.translate(glm::vec3(3, 0, 0));
      
      glUniformMatrix4fv(h_uViewMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

      ModelTrans.translate(glm::vec3(-6, 0, 0));

      glUniformMatrix4fv(h_uViewMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

      ModelTrans.translate(glm::vec3(3, 0, 3));

      glUniformMatrix4fv(h_uViewMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

      ModelTrans.translate(glm::vec3(0, 0, -6));

      glUniformMatrix4fv(h_uViewMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
   
   GLSL::disableVertexAttribArray(h_aPosition);
   GLSL::disableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   SetLightModel();
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   
   // Disable and unbind
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // ==========================================================
   // DRAW THE GROUND

   glEnableVertexAttribArray(h_aPosition);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjG);
   glVertexAttribPointer( h_aPosition, 3,  GL_FLOAT, GL_FALSE, 0, (void*)0);
   GLSL::enableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObjG);
   glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

   glDrawArrays(GL_TRIANGLES, 0, 6);

   GLSL::disableVertexAttribArray(h_aPosition);
   GLSL::disableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glUseProgram(0);
   assert(glGetError() == GL_NO_ERROR);
   
}


void window_size_callback(GLFWwindow* window, int w, int h){
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   g_width = w;
   g_height = h;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_N && action == GLFW_PRESS)  
      drawNormals = !drawNormals;

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
      g_angle += 10;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
      g_angle -= 10;
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
     g_mat_id = (g_mat_id+1)%4; 
    if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
      g_SM = !g_SM; 
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
      g_vertAngle += 10;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
      g_vertAngle -= 10;
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
     g_light.x += 0.5; 
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
     g_light.x -= 0.5; 
}

int main(int argc, char **argv)
{

// Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
   glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Open a window and create its OpenGL context
   g_width = 1024;
   g_height = 768;
   window = glfwCreateWindow( g_width, g_height, "P3 - shading", NULL, NULL);
   if( window == NULL ){
      fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
// Initialize GLEW
   if (glewInit() != GLEW_OK) {
      fprintf(stderr, "Failed to initialize GLEW\n");
      return -1;
   }

   // Ensure we can capture the escape key being pressed below
   glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   loadShapes("cow-nonormals.obj");
   //loadShapes("sphere.obj");
   //loadShapes("cube.obj");
   initGL();
   installShaders("vert.glsl", "frag.glsl");

   glClearColor(0.6f, 0.6f, 0.8f, 1.0f);


    do{
      drawGL();
        // Swap buffers
      glfwSwapBuffers(window);
      glfwPollEvents();
      glfwSetKeyCallback(window, keyPressed);
      glfwSetScrollCallback(window, scrollCallback);

    } // Check if the ESC key was pressed or the window was closed
   while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0 );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

   return 0;
}