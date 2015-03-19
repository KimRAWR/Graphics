/* Kim Arre
 * CPE 471
 * February 27, 2015
 */

#define NUM_BUNNIES 1
#define NUM_PENGUINS 10

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

#include "Penguin.h"

GLFWwindow* window;
using namespace std;

vector<tinyobj::shape_t> bunny;
vector<tinyobj::shape_t> penguin;
vector<tinyobj::material_t> materials;

int g_SM = 0;
int g_width;
int g_height;
float g_Camtrans = -2.5;
float g_angle = 0;
float g_vertAngle = 0.0;
int g_mat_id =0;
glm::vec3 g_trans(0, 0, 0);
glm::vec3 g_light(6, 6, 6);
time_t startTime;
time_t endTime;

float forcedY = 0.0;
int nIndices;

float g_bunny_positionX[NUM_BUNNIES];
float g_bunny_positionZ[NUM_BUNNIES];
float g_bunny_rotation[NUM_BUNNIES];

Penguin *penguinModels[NUM_PENGUINS];
// positions for X and Z component
float penguinPositions[NUM_PENGUINS*2] = {-2, 2, -6, 4, -2, -2, -7, -2, -4, -6, 0, -6, 4, -4, 2, 2, 7, 2, 4, 6};

int randomMaterial[NUM_BUNNIES];

GLuint ShadeProg;
GLuint posBufObjB = 0;
GLuint norBufObjB = 0;
GLuint indBufObjB = 0;

GLuint posBufObjG = 0;
GLuint norBufObjG = 0;

int drawNormals = 0;

glm::vec3 lookAt = glm::vec3(0, 0, 0);
glm::vec3 eye = glm::vec3(-2, .5, 0);
glm::vec3 up = glm::vec3(0, 1, 0);
double phi = 0.0;  //pitch
double theta = 0.0; //yaw

RenderingHelper ModelTrans;
bool shouldShowMessage = true;

//Handles to the shader data
GLint h_aPosition;
GLint h_aNormal;
GLint h_uModelMatrix;
GLint h_uViewMatrix;
GLint h_uProjMatrix;
GLint h_uLightPos;
GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;
GLint h_cameratrans;

int tempCount = 0;

// Returns 0 if not intersecting, 1 for intersect
int checkIntersection(glm::vec3 center1, glm::vec3 center2, float radius1, float radius2) {
   return glm::distance(center1, center2) < radius1 + radius2;
}

void checkCollisions() {
   for (int i=0; i<NUM_PENGUINS; i++) {
      if (checkIntersection(eye, penguinModels[i]->position, .5, .5)) {
         printf("Collision :O - %d\n", tempCount);
         tempCount++;
      }
      penguinModels[i]->checkRunAway(eye);
     
   }
}



void initModelArrays() {
   for (int i=0; i<NUM_BUNNIES; i++) {
      g_bunny_positionX[i] = 0;
      g_bunny_positionZ[i] = 0;
      g_bunny_rotation[i] = 0;


   }
   for (int i=0; i<NUM_PENGUINS; i++) {
      penguinModels[i]->position = vec3(penguinPositions[i*2], 0, penguinPositions[i*2+1]);//vec3(10 * (rand() / (float)RAND_MAX - .5), 0, 25 * (rand() / (float)RAND_MAX - .5));
      //penguinModels[i]->rotation = rand() % 360; 
   }

}

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

// bounds the camera within the box
void boundCamera(glm::vec3 *eye) {
   float rightBoundary = 9.0;
   float leftBoundary = -9.0;
   float bottomBoundary = -9.0;
   float topBoundary = 9.0;

   // ensure position is within bounds
   if (eye->x > rightBoundary) {
      eye->x = rightBoundary;
   }
   else if (eye->x < leftBoundary) {
      eye->x = leftBoundary;
   }

   if (eye->z > topBoundary) {
      eye->z = topBoundary;
   }
   else if (eye->z < bottomBoundary) {
      eye->z = bottomBoundary;
   }
}

void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods) {  
   float x = .5*cos(phi)*cos(theta);
   float y = .5*sin(phi);
   float z = .5*cos(phi)*cos(90-theta);

   float movementSpeed = .35;

   lookAt = glm::vec3(x, y, z) + eye;

   // RIGHT
   if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      eye += glm::cross(lookAt - eye, up) * .25f; 
      lookAt += glm::cross(lookAt - eye, up) * .25f;
      boundCamera(&eye);
   }
   // LEFT
   else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      eye -= glm::cross(lookAt - eye, up) * .25f; 
      lookAt -= glm::cross(lookAt - eye, up) * .25f;
      boundCamera(&eye);
   }
  
   // FORWARD
   else if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      eye += glm::vec3(x, y, z) * glm::vec3(movementSpeed, movementSpeed, movementSpeed);  //forcedY
      lookAt += glm::vec3(x, y, z) * glm::vec3(movementSpeed, movementSpeed, movementSpeed);
      boundCamera(&eye);
   }
   // BACK
   else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      eye -= glm::vec3(x, y, z) * glm::vec3(movementSpeed, movementSpeed, movementSpeed);
      lookAt -= glm::vec3(x, y, z) * glm::vec3(movementSpeed, movementSpeed, movementSpeed);
      boundCamera(&eye);
   }

   // Light movement
   if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      g_light.x += 0.5; 
   }
   if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      g_light.x -= 0.5; 
   }
}

/* helper function to send materials to the shader - you must create your own */
void SetMaterial(int i) {
 
  glUseProgram(ShadeProg);
  switch (i) {
    case 0: //shiny blue plastic
        glUniform3f(h_uMatAmb, 0.9, 0.9, 0.9);
        //glUniform3f(h_uMatDif, 0.0, 0.08, 0.5);
        glUniform3f(h_uMatDif, .43, .68, .99);
        glUniform3f(h_uMatSpec, 0.14, 0.14, 0.4);
        glUniform1f(h_uMatShine, 120.0);
        break;
    case 1: // flat white
        glUniform3f(h_uMatAmb, 0.13, 0.13, 0.14);
        glUniform3f(h_uMatDif, 0.9, 0.9, 0.99);
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
  glm::mat4 Projection = glm::perspective(80.0f, (float)g_width/g_height, 0.1f, 100.f);
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

void loadShapes(const string &objFile, std::vector<tinyobj::shape_t>& shapes)
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

void setBunny(int numBunny) {
  glm::mat4 Trans = glm::translate( glm::mat4(1.0f), glm::vec3(g_bunny_positionX[numBunny], 5, g_bunny_positionZ[numBunny]));
  glm::mat4 RotateY = glm::rotate( glm::mat4(1.0f), g_bunny_rotation[numBunny], glm::vec3(0.0f, 1, 0));
  glm::mat4 Scale = glm::scale( glm::mat4(1.0f), glm::vec3(10.0, 7.0, 10.0));
  glm::mat4 com = Trans*RotateY*Scale;
  safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(com));
}

void initBunny(std::vector<tinyobj::shape_t>& shape) {

   // Send the position array to the GPU
   const vector<float> &posBuf = shape[0].mesh.positions;
   glGenBuffers(1, &posBufObjB);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjB);
   glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
   
   // Send the normal array to the GPU
   vector<float> norBuf;
   glm::vec3 v1, v2, v3;
   glm::vec3 edge1, edge2, norm;
   int idx1, idx2, idx3;
   //for every vertex initialize the vertex normal to 0
   for (int j = 0; j < shape[0].mesh.positions.size()/3; j++) {
      norBuf.push_back(0);
      norBuf.push_back(0);
      norBuf.push_back(0);
   }
   //process the mesh and compute the normals - for every face
   //add its normal to its associated vertex
   for (int i = 0; i < shape[0].mesh.indices.size()/3; i++) {
      idx1 = shape[0].mesh.indices[3*i+0];
      idx2 = shape[0].mesh.indices[3*i+1];
      idx3 = shape[0].mesh.indices[3*i+2];
      v1 = glm::vec3(shape[0].mesh.positions[3*idx1 +0],shape[0].mesh.positions[3*idx1 +1], shape[0].mesh.positions[3*idx1 +2]);
      v2 = glm::vec3(shape[0].mesh.positions[3*idx2 +0],shape[0].mesh.positions[3*idx2 +1], shape[0].mesh.positions[3*idx2 +2]);
      v3 = glm::vec3(shape[0].mesh.positions[3*idx3 +0],shape[0].mesh.positions[3*idx3 +1], shape[0].mesh.positions[3*idx3 +2]);
      edge1 = v2 - v1;
      edge2 = v3 - v1;
      norm = glm::cross(edge1, edge2);
      norBuf[3*idx1+0] += (norm.x);
      norBuf[3*idx1+1] += (norm.y);
      norBuf[3*idx1+2] += (norm.z);
      norBuf[3*idx2+0] += (norm.x);
      norBuf[3*idx2+1] += (norm.y);
      norBuf[3*idx2+2] += (norm.z);
      norBuf[3*idx3+0] += (norm.x);
      norBuf[3*idx3+1] += (norm.y);
      norBuf[3*idx3+2] += (norm.z);
   }
   glGenBuffers(1, &norBufObjB);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObjB);
   glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
   
   // Send the index array to the GPU
   const vector<unsigned int> &indBuf = shape[0].mesh.indices;
   glGenBuffers(1, &indBufObjB);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjB);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

   // Unbind the arrays
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   GLSL::checkVersion();
   assert(glGetError() == GL_NO_ERROR);
}

/*void initPenguin(std::vector<tinyobj::shape_t>& shape) {

   
}*/

void initGL()
{

   initGround();

   for (int i=0; i<NUM_PENGUINS; i++) {
      penguinModels[i] = new Penguin();
   }
   initModelArrays();

   // Set the background color
   glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
   
   // Enable Z-buffer test
   glEnable(GL_DEPTH_TEST);
   glPointSize(18);
   
   //initPenguin(penguin);
   initBunny(bunny);

   //initialize the modeltrans matrix stack
   ModelTrans.useModelViewMatrix();
   ModelTrans.loadIdentity();

   initGround();

   GLSL::checkVersion();
   assert(glGetError() == GL_NO_ERROR);

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
    h_uLightPos = GLSL::getUniformLocation(ShadeProg, "uLightPos");
    h_uMatAmb = GLSL::getUniformLocation(ShadeProg, "UaColor");
    h_uMatDif = GLSL::getUniformLocation(ShadeProg, "UdColor");
    h_uMatSpec = GLSL::getUniformLocation(ShadeProg, "UsColor");
    h_uMatShine = GLSL::getUniformLocation(ShadeProg, "Ushine");
    h_cameratrans = GLSL::getUniformLocation(ShadeProg, "cameraTrans");
   
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

    // ==========================================================
   // DRAW THE GROUND

   SetMaterial(1);

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
  
    // ==============================================================
    // DRAW BUNNY

    glUniform3f(h_uLightPos, g_light.x, g_light.y, g_light.z);
    glUniform3f(h_cameratrans, eye.x, eye.y, eye.z);

   // Enable and bind position array for drawing
   GLSL::enableVertexAttribArray(h_aPosition);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjB);
   glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Enable and bind normal array for drawing
   GLSL::enableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObjB);
   glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Bind index array for drawing
   nIndices = (int)bunny[0].mesh.indices.size();
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjB);
   
   //SetMaterial(2);
   ModelTrans.loadIdentity();
   ModelTrans.lookAt(eye, lookAt, up);
   
   ModelTrans.pushMatrix();

   glUniformMatrix4fv(h_uViewMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      
   for (int i=0; i<NUM_BUNNIES; i++) {
      SetMaterial(2);
      setBunny(i);
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
   }

   GLSL::disableVertexAttribArray(h_aPosition);
   GLSL::disableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   SetLightModel();
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   glUseProgram(0);
   // Disable and unbind
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   // ==========================================================
   // DRAW THE PENGUIN
 
   for (int i=0; i<NUM_PENGUINS; i++) {
      SetMaterial(penguinModels[i]->material);
      penguinModels[i]->draw(eye, lookAt, up);
   }
   
}

void window_size_callback(GLFWwindow* window, int w, int h){
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   g_width = w;
   g_height = h;
}


void victoryMessage() {
//printf(" \n");
printf("                           _ _                 \n");
printf("                     /\\   | | |                \n");
printf("                    /  \\  | | |                \n");
printf("       _____       / /\\ \\ | | |               \n");
printf("      |  __ \\     / ____ \\| | |   (_)          \n");
printf("      | |__) |__ /_/_   \\_\\_|_|  _ _ _ __  ___ \n");
printf("      |  ___/ _ \\ '_ \\ / _` | | | | | '_ \\/ __|\n");
printf("      | |  |  __/ | | | (_| | |_| | | | | \\__ \\\n");
printf("      |_|   \\___|_| |_|\\__, |\\__,_|_|_| |_|___/\n");
printf("       _____        __  __/ |   _           _  \n");
printf("      |_   _|      / _||___/   | |         | | \n");
printf("        | |  _ __ | |_ ___  ___| |_ ___  __| | \n");
printf("        | | | '_ \\|  _/ _ \\/ __| __/ _ \\/ _` | \n");
printf("       _| |_| | | | ||  __/ (__| ||  __/ (_| | \n");
printf("      |_____|_| |_|_| \\___|\\___|\\__\\___|\\__,_| \n");
                                          
                                          

}

int main(int argc, char **argv)
{
   time(&startTime);
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
    loadShapes("cube.obj", bunny);
    //loadShapes("sphere.obj", penguin);
   //loadShapes("cube.obj");
   initGL();
   installShaders("vert.glsl", "frag.glsl");

   glClearColor(0.6f, 0.6f, 0.8f, 1.0f);

    do{
      drawGL();

      // check for collisions
      /*for (int i=0; i<NUM_PENGUINS; i++) {
        if (checkIntersection(eye, penguinModels[i]->position, .5, .5)) {
            printf("Collision :O - %d\n", tempCount);
            tempCount++;
         }
         penguinModels[i]->checkRunAway(eye);
         
      } */
      checkCollisions();

      // check for infections
      bool allInfected = true;
      for (int i=0; i<NUM_PENGUINS; i++) {
         if (penguinModels[i]->isInfected == false) {
            allInfected = false;
            break;
         }
      }

      if (allInfected && shouldShowMessage) {
         victoryMessage();
         time(&endTime);
         printf("\n        It took you %d minutes and %ld seconds!\n\n", (int)(endTime-startTime)/60, (endTime-startTime)%60);
         shouldShowMessage = false;
      }
      

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
