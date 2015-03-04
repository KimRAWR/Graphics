//
// seriously modified ZJ Wood January 2015 - conversion to glfw
// inclusion of matrix stack Feb. 2015
// original from Shinjiro Sueda
// October, 2014
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <stdio.h>
#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "RenderingHelper.h"


GLFWwindow* window;
using namespace std;

vector<tinyobj::shape_t> shapes;
vector<tinyobj::material_t> materials;
GLuint prog;
GLuint posBufObj = 0;
GLuint norBufObj = 0;
GLuint indBufObj = 0;
GLint aPos = 0;
GLint aNor = 0;
GLint uMV = 0;
GLint uP = 0;
bool wave = true;
float direction = 1;
bool upperDown = false;
float translate = 0;
float waddle = 0;
float waddleDirection = 1;

static float g_width, g_height;
float lowerTheta, upperTheta;

//declare a matrix stack
RenderingHelper ModelTrans;

void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods) {  
   if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
      wave = !wave;
   }
   if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      translate += 5;
   }
   if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      translate -= 5;
   }
}

void loadShapes(const string &objFile)
{
	string err = tinyobj::LoadObj(shapes, materials, objFile.c_str());
	if(!err.empty()) {
		cerr << err << endl;
	}
}

void initGL()
{
	// Set the background color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable Z-buffer test
	glEnable(GL_DEPTH_TEST);
	
	// Send the position array to the GPU
	const vector<float> &posBuf = shapes[0].mesh.positions;
	glGenBuffers(1, &posBufObj);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	// Send the normal array to the GPU
	const vector<float> &norBuf = shapes[0].mesh.normals;
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
   lowerTheta = 0;
   upperTheta = 0;
   waddle = 0;
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
	prog = glCreateProgram();
	glAttachShader(prog, VS);
	glAttachShader(prog, FS);
	glLinkProgram(prog);
	GLSL::printError();
	glGetProgramiv(prog, GL_LINK_STATUS, &rc);
	GLSL::printProgramInfoLog(prog);
	if(!rc) {
		printf("Error linking shaders %s and %s\n", vShaderName.c_str(), fShaderName.c_str());
		return false;
	}
	
	// Set up the shader variables
	aPos = GLSL::getAttribLocation(prog, "aPos");
	aNor = GLSL::getAttribLocation(prog, "aNor");
	uMV = GLSL::getUniformLocation(prog, "MV");
	uP = GLSL::getUniformLocation(prog, "P");
	
	assert(glGetError() == GL_NO_ERROR);
	return true;
}


void drawGL()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Use our GLSL program
	glUseProgram(prog);

	// Enable and bind position array for drawing
	GLSL::enableVertexAttribArray(aPos);
	glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Enable and bind normal array for drawing
	GLSL::enableVertexAttribArray(aNor);
	glBindBuffer(GL_ARRAY_BUFFER, norBufObj);
	glVertexAttribPointer(aNor, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Bind index array for drawing
	int nIndices = (int)shapes[0].mesh.indices.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObj);
	
	// Compute and send the projection matrix - leave this as is
   glm::mat4 Projection = glm::perspective(80.0f, (float)g_width/g_height, 0.1f, 100.f); 
	glUniformMatrix4fv(uP, 1, GL_FALSE, glm::value_ptr(Projection));

   
   ModelTrans.loadIdentity();
   ModelTrans.translate(glm::vec3(0, 0, -2));  //global trans and rotate
   ModelTrans.rotate(translate, glm::vec3(0, 1, 0));
   ModelTrans.pushMatrix();

   // BODY
   ModelTrans.translate(glm::vec3(0, -1, 0));   
   ModelTrans.rotate(waddle, glm::vec3(0, 0, 1));
   ModelTrans.translate(glm::vec3(0, 1, 0));
         if (waddle > 5.0) {
            waddleDirection = -1.0;
         } else if (waddle < -5.0) {
            waddleDirection = 1.0;
         }
         waddle += waddleDirection * .25;
   
   ModelTrans.pushMatrix();

      // TUMMY
      ModelTrans.scale(.6, 1, .5);
      ModelTrans.translate(glm::vec3(0, -.3, .65));
      ModelTrans.rotate(30, glm::vec3(1, 0, 0));
      ModelTrans.scale(.65, .6, .4);

      // draw tummy
      glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
      ModelTrans.popMatrix();

   // Push original body 
   ModelTrans.pushMatrix();

      // RIGHT ARM
      ModelTrans.translate(glm::vec3(.49, .22, .10));
      ModelTrans.rotate(upperTheta, glm::vec3(0, 0, 1));
      // translate to pivot
      ModelTrans.translate(glm::vec3(.18, -.44, 0));

      ModelTrans.rotate(20, glm::vec3(0, 0, 1));
      ModelTrans.rotate(-5, glm::vec3(0, 1, 0));
      ModelTrans.scale(.1, .48, .4);
      
      if (wave) {
         if (upperTheta > 10.0) {
            direction = -1.0;
         } else if (upperTheta < -30.0) {
            direction = 1.0;
         }
         upperTheta += direction * .35;
      }

      // draw right arm
      ModelTrans.scale(.6, 1, .5);
      glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

      ModelTrans.popMatrix();

   ModelTrans.pushMatrix();
      // LEFT ARM
      ModelTrans.translate(glm::vec3(-.47, .18, .1));
      ModelTrans.rotate(-upperTheta, glm::vec3(0, 0, 1));
      // translate to pivot
      ModelTrans.translate(glm::vec3(-.18, -.44, 0));
      
      ModelTrans.rotate(-20, glm::vec3(0, 0, 1));
      ModelTrans.rotate(5, glm::vec3(0, 1, 0));
      ModelTrans.scale(.1, .48, .4);
      
      if (wave) {
         if (upperTheta > 20.0) {
            direction = -1.0;
         } else if (upperTheta < -10.0) {
            direction = 1.0;
         }
         upperTheta += direction * .35;
      }

      // draw left arm
      ModelTrans.scale(.6, 1, .5);
      glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

      ModelTrans.popMatrix();

   ModelTrans.pushMatrix();
      //BEAK
      ModelTrans.scale(.6, 1, .5);
      ModelTrans.translate(glm::vec3(0, .3, .8));
      ModelTrans.scale(.3, .05, .3);

      // draw beak
      glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

      ModelTrans.popMatrix();

   ModelTrans.pushMatrix();
      // LEFT EYE
      ModelTrans.scale(.6, 1, .5);
      ModelTrans.translate(glm::vec3(-.28, .5, .71));
      ModelTrans.scale(.06, .03, .05);

      // draw left eye
      glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

      ModelTrans.popMatrix();
   
   ModelTrans.pushMatrix();
      // RIGHT EYE
      ModelTrans.scale(.6, 1, .5);
      ModelTrans.translate(glm::vec3(.28, .5, .71));
      ModelTrans.scale(.06, .03, .05);

      // draw right eye
      glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

      ModelTrans.popMatrix();

   ModelTrans.pushMatrix();
      // LEFT FOOT
      ModelTrans.scale(.6, 1, .5);
      ModelTrans.translate(glm::vec3(-.28, -.9, .45));
      ModelTrans.scale(.25, .05, .6);

      // draw left foot
      glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
      
      ModelTrans.popMatrix();

   ModelTrans.pushMatrix();
      // RIGHT FOOT
      ModelTrans.scale(.6, 1, .5);
      ModelTrans.translate(glm::vec3(.28, -.9, .45));
      ModelTrans.scale(.25, .05, .6);

      // draw right foot
      glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
      
      ModelTrans.popMatrix();

   // draw body
   ModelTrans.scale(.6, 1, .5);
   glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
   ModelTrans.popMatrix();

         // draw tummy
         /*glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
         glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
         ModelTrans.popMatrix();*/

      // draw body
      
      //ModelTrans.pushMatrix();

  // ModelTrans.popMatrix();

         /*// Upper arm
         ModelTrans.translate(glm::vec3(.55, 0, 0));
         ModelTrans.rotate(upperTheta, glm::vec3(0, 0, 1));
         ModelTrans.translate(glm::vec3(.3, 0, 0));
         
         
         if (wave) {
            if (upperTheta > 40.0) {
               direction = -1.0;
            } else if (upperTheta < 0.0) {
               direction = 1.0;
            }
            upperTheta += direction * .35;
         }

         ModelTrans.pushMatrix();

            // lowerArm
            ModelTrans.translate(glm::vec3(.3, 0, 0));
            ModelTrans.rotate(lowerTheta, glm::vec3(0, 0, 1));
            ModelTrans.translate(glm::vec3(.3, 0, 0));
            

            if (wave) {
               if (lowerTheta > 40.0) {
                  direction = -1.0;
               } else if (lowerTheta < 0.0) {
                  direction = 1.0;
               }


               lowerTheta += direction * .35;
            }

            // draw lower arm
            ModelTrans.scale(.65, .2, 1);
            glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
            glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
         ModelTrans.popMatrix();


         // draw upper arm
         ModelTrans.scale(.65, .2, 1);
         glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
         glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
      ModelTrans.popMatrix();

      // draw torso
      ModelTrans.scale(1, 1.5, 1);  //global trans and rotate
      glUniformMatrix4fv(uMV, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
   */
	
   // Disable and unbind
	GLSL::disableVertexAttribArray(aPos);
	GLSL::disableVertexAttribArray(aNor);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
	assert(glGetError() == GL_NO_ERROR);
	
}

void reshapeGL(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	g_width = w;
	g_height = h;
}


int main(int argc, char **argv)
{

   // Initialise GLFW
   if( !glfwInit() ) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
   }

   glfwWindowHint(GLFW_SAMPLES, 4);
   glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

   g_width = 1024;
   g_height = 768;
   // Open a window and create its OpenGL context
   window = glfwCreateWindow( g_width, g_height, "lab 6 - HM", NULL, NULL);
   if( window == NULL ){
      fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

   // Ensure we can capture the escape key being pressed below
   glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	loadShapes("sphere.obj");

   std::cout << " loaded the object " << endl;

   // Initialize glad
   if (!gladLoadGL()) {
      fprintf(stderr, "Unable to initialize glad");
      glfwDestroyWindow(window);
      glfwTerminate();
      return 1;
   }

	initGL();
	installShaders("vert.glsl", "frag.glsl");

    do{
      drawGL();
        // Swap buffers
      glfwSwapBuffers(window);
      glfwPollEvents();
      glfwSetKeyCallback(window, keyPressed);

    } // Check if the ESC key was pressed or the window was closed
   while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0 );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

	return 0;
}
