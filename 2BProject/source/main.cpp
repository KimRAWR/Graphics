//
// Shinjiro Sueda
// seriously modified ZJ Wood January 2015 - conversion to glfw
// October, 2014
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <stdio.h>
#include "GLSL.h"
#include <GLFW/glfw3.h>
#include "tiny_obj_loader.h"

GLFWwindow* window;
using namespace std;

vector<tinyobj::shape_t> shapes;
vector<tinyobj::material_t> materials;
GLuint vertexPosition_modelspaceID;
GLuint prog;
GLuint snowProg;
GLuint posBufObj = 0;
GLuint norBufObj = 0;
GLuint indBufObj = 0;
GLint aPos = 0;
GLint aNor = 0;
GLint uMV = 0;
GLint uP = 0;
GLint globeID;

int LAB5_DEBUG = 0;
int width = 1;
int height = 1;
float modelRotation;
bool flapDown = true;
float flappingRad = 0.0;
//float time = 0.0;



void loadShapes(const string &objFile)
{
	string err = tinyobj::LoadObj(shapes, materials, objFile.c_str());
	if(!err.empty()) {
		cerr << err << endl;
	}
}

void initGL()
{
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set the background color
	glClearColor(0.2f, .4f, .8f, .8f);
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), 
    &indBuf[0], GL_STATIC_DRAW);

   

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
   globeID = GLSL::getUniformLocation(prog, "globe");

   vertexPosition_modelspaceID = glGetAttribLocation(prog, "vertexPosition_modelspace");
	
	assert(glGetError() == GL_NO_ERROR);
	return true;
}

void printMat(float *A, const char *name = 0)
{
	// OpenGL uses col-major ordering:
	// [ 0  4  8 12]
	// [ 1  5  9 13]
	// [ 2  6 10 14]
	// [ 3  7 11 15]
	// The (i,j)th element is A[i+4*j].
	if(name) {
		printf("%s=[\n", name);
	}
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			printf("%- 5.2f ", A[i+4*j]);
		}
		printf("\n");
	}
	if(name) {
		printf("];");
	}
	printf("\n");
}

void createIdentityMat(float *M)
{
   for(int i = 0; i < 4; ++i) {
      for(int j = 0; j < 4; ++j) {
         if (i == j) {
            M[i+4*j] = 1;
         } else {
            M[i+4*j] = 0;
         }
      }
   }
}

void createTranslateMat(float *T, float x, float y, float z)
{
   createIdentityMat(T);
   T[12] = x;
   T[13] = y;
   T[14] = z;
}

void createScaleMat(float *S, float x, float y, float z)
{
	S[0] = x;
   S[1] = 0.0;
   S[2] = 0.0;
   S[3] = 0.0;
   S[4] = 0.0;
   S[5] = y;
   S[6] = 0.0;
   S[7] = 0.0;
   S[8] = 0.0;
   S[9] = 0.0;
   S[10] = z;
   S[11] = 0.0;
   S[12] = 0.0;
   S[13] = 0.0;
   S[14] = 0.0;
   S[15] = 1.0;
}

void createRotateMatX(float *R, float radians)
{	
	R[0] = 1.0;
   R[1] = 0.0;
   R[2] = 0.0;
   R[3] = 0.0;
   R[4] = 0.0;
   R[5] = cos(radians);
   R[6] = sin(radians);
   R[7] = 0.0;
   R[8] = 0.0;
   R[9] = -sin(radians);
   R[10] = cos(radians);
   R[11] = 0.0;
   R[12] = 0.0;
   R[13] = 0.0;
   R[14] = 0.0;
   R[15] = 1.0;
}

void createRotateMatY(float *R, float radians)
{
	R[0] = cos(radians);
   R[1] = 0.0;
   R[2] = -sin(radians);
   R[3] = 0.0;
   R[4] = 0.0;
   R[5] = 1.0;
   R[6] = 0.0;
   R[7] = 0.0;
   R[8] = sin(radians);
   R[9] = 0.0;
   R[10] = cos(radians);
   R[11] = 0.0;
   R[12] = 0.0;
   R[13] = 0.0;
   R[14] = 0.0;
   R[15] = 1.0;
}

void createRotateMatZ(float *R, float radians)
{
	R[0] = cos(radians);
   R[1] = sin(radians);
   R[2] = 0.0;
   R[3] = 0.0;
   R[4] = -sin(radians);
   R[5] = cos(radians);
   R[6] = 0.0;
   R[7] = 0.0;
   R[8] = 0.0;
   R[9] = 0.0;
   R[10] = 1.0;
   R[11] = 0.0;
   R[12] = 0.0;
   R[13] = 0.0;
   R[14] = 0.0;
   R[15] = 1.0;
}

void multMat(float *C, const float *A, const float *B)
{
   float c = 0;
   for(int k = 0; k < 4; ++k) {
      // Process kth column of C
      for(int i = 0; i < 4; ++i) {
         // Process ith row of C.
         // The (i,k)th element of C is the dot product
         // of the ith row of A and kth col of B.
         c = 0;
         //vector dot
         for(int j = 0; j < 4; ++j) {
            c += A[i + 4*j] * B[j + 4*k];
         }
         C[i + 4*k] = c;
      }
   }
}

void createPerspectiveMat(float *m, float fovy, float aspect, float zNear, float zFar)
{
	// http://www-01.ibm.com/support/knowledgecenter/ssw_aix_61/com.ibm.aix.opengl/doc/
   //openglrf/gluPerspective.htm%23b5c8872587rree
	float f = 1.0f/tan(0.5f*fovy);
	m[ 0] = f/aspect;
	m[ 1] = 0.0f;
	m[ 2] = 0.0f;
	m[ 3] = 0.0f;
	m[ 4] = 0;
	m[ 5] = f;
	m[ 6] = 0.0f;
	m[ 7] = 0.0f;
	m[ 8] = 0.0f;
	m[ 9] = 0.0f;
	m[10] = (zFar + zNear)/(zNear - zFar);
	m[11] = -1.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 2.0f*zFar*zNear/(zNear - zFar);
	m[15] = 0.0f;
}

void keypressRotate(float *toDraw, float *oldTemp) {
   float transMat_origin[16] = {0};
   float newRotation[16] = {0};
   float newTemp[16] = {0};
   float atOrigin[16] = {0};
   float transMat[16] = {0};

   // move the model to the origin
   createTranslateMat(transMat_origin, 0, 0, 2);
   multMat(atOrigin, transMat_origin, oldTemp);

   // rotate him around depending on keypresses
   createRotateMatY(newRotation, modelRotation);
   multMat(newTemp, newRotation, atOrigin);

   // move him back 
   createTranslateMat(transMat, 0, 0, -2);
   multMat(toDraw, transMat, newTemp);
}

void drawSnow() {
   // Use our GLSL program
   glUseProgram(snowProg);
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
	
	// Compute and send the projection matrix
	float P[16];
	createPerspectiveMat(P, 45.0f, (float)width/height, 0.01f, 100.0f);
	glUniformMatrix4fv(uP, 1, GL_FALSE, P);
   
   if (LAB5_DEBUG) {
      float A[16], B[16], C[16];
      for(int i = 0; i < 16; ++i) { A[i] = i; }
      for(int i = 0; i < 16; ++i) { B[i] = i*i; }
      multMat(C, A, B);
      printMat(A, "A");
      printMat(B, "B");
      printMat(C, "C");
   }

   float MV[16] = {0};
   float rotationMat[16] = {0};
   float transMat[16] = {0};
   float pivotTrans[16] = {0};
   float scaleMat[16] = {0};
   float temp[16] = {0};
   float temp2[16] = {0};
   float finalRotation[16] = {0};
   float final[16] = {0};


   if (flappingRad > .4f) {
      flapDown = true;
   }
   else if (flappingRad < 0.0f) {
      flapDown = false;
   }

   if (flapDown) {
      flappingRad -= .01f;
   } else {
      flappingRad += .01f;
   }

   // Not the encapsulating globe
   glUniform1f(globeID, -1.0f);

   // BODY
   createTranslateMat(transMat, 0, 0, -2);
   createScaleMat(scaleMat, .35, .85, .35); //x and z should match
   multMat(temp, transMat, scaleMat);

   keypressRotate(MV, temp);

	glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
	glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // INNER BODY
   createTranslateMat(transMat, 0, -.36, -1.79);
   createScaleMat(scaleMat, .25, .45, .1); //x and z should match
   createRotateMatX(rotationMat, .29);

   multMat(temp2, rotationMat, scaleMat);
   multMat(temp, transMat, temp2);

   keypressRotate(MV, temp);

   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // BEAK
   createScaleMat(scaleMat, .1, .05, .05);
   createTranslateMat(transMat, 0, .2, -1.7);
   multMat(temp, transMat, scaleMat);

   keypressRotate(MV, temp);

   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // LEFT FOOT
   createScaleMat(scaleMat, .1, .03, .2);
   createTranslateMat(transMat, -.1, -.7, -1.7);
   multMat(temp, transMat, scaleMat);

   keypressRotate(MV, temp);

   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // RIGHT FOOT
   createScaleMat(scaleMat, .1, .03, .2);
   createTranslateMat(transMat, .1, -.7, -1.7);
   multMat(temp, transMat, scaleMat);

   keypressRotate(MV, temp);

   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // LEFT EYE
   createScaleMat(scaleMat, .02, .02, .02);
   createTranslateMat(transMat, .13, .34, -1.73);
   multMat(temp, transMat, scaleMat);

   keypressRotate(MV, temp);

   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // RIGHT EYE
   createScaleMat(scaleMat, .02, .02, .02);
   createTranslateMat(transMat, -.13, .34, -1.73);
   multMat(temp, transMat, scaleMat);

   keypressRotate(MV, temp);

   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // LEFT WING
   createScaleMat(scaleMat, .03, .4, .1);
   createTranslateMat(pivotTrans, 0, -.4, 0); // pivot translation
   createTranslateMat(transMat, -.33, .2, -2);
   createRotateMatZ(rotationMat, -.5 + flappingRad);
   
   multMat(temp, pivotTrans, scaleMat);
   multMat(temp2, rotationMat, temp);
   multMat(final, transMat, temp2);

   keypressRotate(MV, final);
  
   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // RIGHT WING
   createScaleMat(scaleMat, .03, .4, .1);
   createTranslateMat(pivotTrans, 0, -.4, 0); // pivot translation
   createTranslateMat(transMat, .33, .2, -2);
   createRotateMatZ(rotationMat, .5 - flappingRad);
   
   multMat(temp, pivotTrans, scaleMat);
   multMat(temp2, rotationMat, temp);
   multMat(final,transMat, temp2);

   keypressRotate(MV, final);
  
   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // encapsulating globe
   glUniform1f(globeID, 1.0f);

   createScaleMat(scaleMat, 1, 1.05, 1);
   createTranslateMat(transMat, 0, 0, -2);
   multMat(temp, transMat, scaleMat);
   keypressRotate(MV, temp);

   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

	
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
	width = w;
	height = h;
}

void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mods) {  
   if (key == GLFW_KEY_A && action == GLFW_PRESS) {
      modelRotation -= .45;
   }
   else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
      modelRotation += .45;
   }
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
   window = glfwCreateWindow( 1024, 768, "lab 5 - object", NULL, NULL);
   if( window == NULL ){
      fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, \
       they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
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

   /*float vertices[80] = {};
    
    for (int i=0; i<80; i++) {
        vertices[i] = (float)rand()/(float)RAND_MAX*2.0f - 1.2f;
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

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

//installShaders("vert.glsl", "frag.glsl");*/
    do{
      //installShaders("SimpleVertexShader.glsl", "frag.glsl");
      // 1rst attribute buffer : vertices
        /*glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            vertexPosition_modelspaceID, // The attribute we want to configure
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        glDrawArrays(GL_POINTS, 0, 40);
        glPointSize(25.0);
        
        glDisableVertexAttribArray(vertexPosition_modelspaceID);*/

      
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
