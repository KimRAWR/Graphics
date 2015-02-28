
#include "penguin.h"
#include "GLSL.h"
#include <string>
#include <vector>
#include "tiny_obj_loader.h"
#include <iostream>
#include <assert.h>
//#include "matrixMath.h"

Penguin::Penguin() {
   /*aPos = GLSL::getAttribLocation(shader.prog, "aPos");
   aNor = GLSL::getAttribLocation(shader.prog, "aNor");
   uMV = GLSL::getUniformLocation(shader.prog, "MV");
   uP = GLSL::getUniformLocation(shader.prog, "P");
   globeID = GLSL::getUniformLocation(shader.prog, "globe");*/

   flapDown = true;
   flappingRad = 0.0;

   /*std::vector<tinyobj::shape_t> shapes;
   std::vector<tinyobj::material_t> materials;

   std::string err = tinyobj::LoadObj(shapes, materials, "sphere.obj");
   if(!err.empty()) {
      std::cerr << err << std::endl;
   }

   nIndices = (int)shapes[0].mesh.indices.size();

   // Send the position array to the GPU
   const std::vector<float> &posBuf = shapes[0].mesh.positions;
   glGenBuffers(1, &posBufObj);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
   glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
   
   // Send the normal array to the GPU
   const std::vector<float> &norBuf = shapes[0].mesh.normals;
   glGenBuffers(1, &norBufObj);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObj);
   glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
   
   // Send the index array to the GPU
   const std::vector<unsigned int> &indBuf = shapes[0].mesh.indices;
   glGenBuffers(1, &indBufObj);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObj);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), 
    &indBuf[0], GL_STATIC_DRAW);

   // Unbind the arrays
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   assert(glGetError() == GL_NO_ERROR);*/
}

void Penguin::draw(int width, int height, float modelRotation) {
   // Use our GLSL program
   shader.use();

   // Enable and bind position array for drawing
   GLSL::enableVertexAttribArray(aPos);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObj);
   glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Enable and bind normal array for drawing
   GLSL::enableVertexAttribArray(aNor);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObj);
   glVertexAttribPointer(aNor, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Bind index array for drawing
   
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObj);
   
   // Compute and send the projection matrix
   float P[16];
   createPerspectiveMat(P, 45.0f, (float)width/height, 0.01f, 100.0f);
   glUniformMatrix4fv(uP, 1, GL_FALSE, P);
   

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
   /*glUniform1f(globeID, -1.0f);

   
   // BODY
   createTranslateMat(transMat, 0, 0, -2);
   createScaleMat(scaleMat, .35, .85, .35); //x and z should match
   multMat(temp, transMat, scaleMat);

   keypressRotate(MV, temp, modelRotation);

   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // INNER BODY
   createTranslateMat(transMat, 0, -.36, -1.79);
   createScaleMat(scaleMat, .25, .45, .1); //x and z should match
   createRotateMatX(rotationMat, .29);

   multMat(temp2, rotationMat, scaleMat);
   multMat(temp, transMat, temp2);

   keypressRotate(MV, temp, modelRotation);

   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // BEAK
   createScaleMat(scaleMat, .1, .05, .05);
   createTranslateMat(transMat, 0, .2, -1.7);
   multMat(temp, transMat, scaleMat);

   keypressRotate(MV, temp, modelRotation);

   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // LEFT FOOT
   createScaleMat(scaleMat, .1, .03, .2);
   createTranslateMat(transMat, -.1, -.7, -1.7);
   multMat(temp, transMat, scaleMat);

   keypressRotate(MV, temp, modelRotation);

   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // RIGHT FOOT
   createScaleMat(scaleMat, .1, .03, .2);
   createTranslateMat(transMat, .1, -.7, -1.7);
   multMat(temp, transMat, scaleMat);

   keypressRotate(MV, temp, modelRotation);

   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // LEFT EYE
   createScaleMat(scaleMat, .02, .02, .02);
   createTranslateMat(transMat, .13, .34, -1.73);
   multMat(temp, transMat, scaleMat);

   keypressRotate(MV, temp, modelRotation);

   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // RIGHT EYE
   createScaleMat(scaleMat, .02, .02, .02);
   createTranslateMat(transMat, -.13, .34, -1.73);
   multMat(temp, transMat, scaleMat);

   keypressRotate(MV, temp, modelRotation);

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

   keypressRotate(MV, final, modelRotation);
  
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

   keypressRotate(MV, final, modelRotation);
  
   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   // encapsulating globe
   glUniform1f(globeID, 1.0f);

   createScaleMat(scaleMat, 1, 1.05, 1);
   createTranslateMat(transMat, 0, 0, -2);
   multMat(temp, transMat, scaleMat);
   keypressRotate(MV, temp, modelRotation);

   glUniformMatrix4fv(uMV, 1, GL_FALSE, MV);
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   
   // Disable and unbind
   GLSL::disableVertexAttribArray(aPos);
   GLSL::disableVertexAttribArray(aNor);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   glUseProgram(0);
   assert(glGetError() == GL_NO_ERROR);*/
}

void Penguin::keypressRotate(float *toDraw, float *oldTemp, float modelRotation) {
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