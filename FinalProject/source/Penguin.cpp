#define NUM_PENGUINS 10

#include "GLIncludes.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "GLSL.h"
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "Penguin.h"
#include "RenderingHelper.h"
#include "shaderProgram.h"

using namespace std;


Penguin::Penguin() :
   ShadeProg("penguin.vert", "penguin.frag") {

   h_aPosition = GLSL::getAttribLocation(ShadeProg.prog, "aPos");
   h_aNormal = GLSL::getAttribLocation(ShadeProg.prog, "aNor");
   h_uProjMatrix = GLSL::getUniformLocation(ShadeProg.prog, "P");
   h_uViewMatrix = GLSL::getUniformLocation(ShadeProg.prog, "V");
   h_uModelMatrix = GLSL::getUniformLocation(ShadeProg.prog, "M");
   h_uLightPos = GLSL::getUniformLocation(ShadeProg.prog, "uLightPos");
   h_uMatAmb = GLSL::getUniformLocation(ShadeProg.prog, "UaColor");
   h_uMatDif = GLSL::getUniformLocation(ShadeProg.prog, "UdColor");
   h_uMatSpec = GLSL::getUniformLocation(ShadeProg.prog, "UsColor");
   h_uMatShine = GLSL::getUniformLocation(ShadeProg.prog, "Ushine");
   h_cameratrans = GLSL::getUniformLocation(ShadeProg.prog, "cameraTrans");

   rotation = (rand() % 3600) * .1;

   ModelTrans.useModelViewMatrix();
   ModelTrans.loadIdentity();

   upperTheta = 0;
   waddle = 0;
   waddleDirection = 1;
   direction = 1;
   wave = true;

   g_trans = glm::vec3(0, 0, 0);
   g_light = glm::vec3(6, 6, 6);

   g_angle = 0;
   g_vertAngle = 0.0;
   g_mat_id = 0;

   posBufObjP = 0;
   norBufObjP = 0;
   indBufObjP = 0;

   runningVector = glm::vec3(0.0, 0.0, 0.0);
   material = 0; // default to shiny blue, should be rose gold when touched

   std::string err = tinyobj::LoadObj(shapes, materials, "sphere.obj");
   if(!err.empty()) {
      std::cerr << err << std::endl;
   }

   loadShapes("sphere.obj", shapes);
   nIndices = (int)shapes[0].mesh.indices.size();

   // Send the position array to the GPU
   const vector<float> &posBuf = shapes[0].mesh.positions;
   glGenBuffers(1, &posBufObjP);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjP);
   glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
   
   // Send the normal array to the GPU
   vector<float> norBuf;
   glm::vec3 v1, v2, v3;
   glm::vec3 edge1, edge2, norm;
   int idx1, idx2, idx3;
   //for every vertex initialize the vertex normal to 0
   for (int j = 0; j < shapes[0].mesh.positions.size()/3; j++) {
      norBuf.push_back(0);
      norBuf.push_back(0);
      norBuf.push_back(0);
   }
   //process the mesh and compute the normals - for every face
   //add its normal to its associated vertex
   for (int i = 0; i < shapes[0].mesh.indices.size()/3; i++) {
      idx1 = shapes[0].mesh.indices[3*i+0];
      idx2 = shapes[0].mesh.indices[3*i+1];
      idx3 = shapes[0].mesh.indices[3*i+2];
      v1 = glm::vec3(shapes[0].mesh.positions[3*idx1 +0],shapes[0].mesh.positions[3*idx1 +1], shapes[0].mesh.positions[3*idx1 +2]);
      v2 = glm::vec3(shapes[0].mesh.positions[3*idx2 +0],shapes[0].mesh.positions[3*idx2 +1], shapes[0].mesh.positions[3*idx2 +2]);
      v3 = glm::vec3(shapes[0].mesh.positions[3*idx3 +0],shapes[0].mesh.positions[3*idx3 +1], shapes[0].mesh.positions[3*idx3 +2]);
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
   glGenBuffers(1, &norBufObjP);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObjP);
   glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
   
   // Send the index array to the GPU
   const vector<unsigned int> &indBuf = shapes[0].mesh.indices;
   glGenBuffers(1, &indBufObjP);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjP);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);

   // Unbind the arrays
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   GLSL::checkVersion();
   assert(glGetError() == GL_NO_ERROR);
}

// Returns the material number it should have
int Penguin::checkRunAway(glm::vec3 eye) {
   glm::vec3 movement;
   float rightBoundary = 9.0;
   float leftBoundary = -9.0;

   if (glm::distance(eye, position) < 3) {
      material = 3;
      
      movement = glm::normalize(position - eye);  
      runningVector += movement * glm::vec3(.01, 0.0, .01); 

      rotation = glm::degrees(atan2(movement.z, movement.x));

   } else if (glm::distance(eye, position) > 9) {
      runningVector = glm::vec3(0.0, 0.0, 0.0);
      
   } // penguin is in the middle of running 
   else {

      // Left boundary
      if (position.x <= leftBoundary) {
         printf("hit left boundary\n");
         // moving upward
         if (runningVector.z > 0) {
            rotation = 90 - (180 - rotation);
            movement = glm::vec3(glm::degrees(cos(glm::radians(rotation))), 0, glm::degrees(sin(glm::radians(rotation))));
            runningVector = movement * glm::vec3(.001, 0.0, .001);
         } // moving downward 
         else {  
            rotation -= 2 * ( 90 - (360 - rotation));
            movement = glm::vec3(glm::degrees(cos(glm::radians(rotation))), 0, glm::degrees(sin(glm::radians(rotation))));
            runningVector = movement * glm::vec3(.001, 0.0, .001);
         }
      } 

      // Right boundary
      else if(position.x >= rightBoundary) {
         printf("hit right boundary\n");
         // moving upward
         if (runningVector.z > 0) {
            rotation = 2 * (90 - rotation);//90 - (180 - rotation);
            movement = glm::vec3(glm::degrees(cos(glm::radians(rotation))), 0, glm::degrees(sin(glm::radians(rotation))));
            runningVector = movement * glm::vec3(.001, 0.0, .001);
         } // moving downward 
         else {  
            rotation -= 2 * ( 90 - (360 - rotation));
            movement = glm::vec3(glm::degrees(cos(glm::radians(rotation))), 0, glm::degrees(sin(glm::radians(rotation))));
            runningVector = movement * glm::vec3(.001, 0.0, .001);
         }
      }
   }
   position += runningVector;
   if (position.x > rightBoundary) {
      position.x = rightBoundary;
   }
   else if (position.x < leftBoundary) {
      position.x = leftBoundary;
   }

   
   return material;
}

void Penguin::updateFacingDirection() {
   
}



void Penguin::draw(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up) { //width, height?
      //glUseProgram(ShadeProg.prog);

      ModelTrans.loadIdentity();
      ModelTrans.lookAt(eye, lookAt, up);

    glUniform3f(h_uLightPos, g_light.x, g_light.y, g_light.z);
    //glUniform1i(h_uShadeM, g_SM);
    //glUniform1f(colorByNormalsID, drawNormals);
    glUniform3f(h_cameratrans, eye.x, eye.y, eye.z);

   // Enable and bind position array for drawing
   GLSL::enableVertexAttribArray(h_aPosition);
   glBindBuffer(GL_ARRAY_BUFFER, posBufObjP);
   glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Enable and bind normal array for drawing
   GLSL::enableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, norBufObjP);
   glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   // Bind index array for drawing
   nIndices = (int)shapes[0].mesh.indices.size();
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufObjP);
   
   //SetMaterial(3);

   glUniformMatrix4fv(h_uViewMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      
   //for (int i=0; i<NUM_PENGUINS; i++) {
      //SetMaterial(penguinModels[i].material);
      drawPenguinModel();
   //}

   GLSL::disableVertexAttribArray(h_aPosition);
   GLSL::disableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   SetLightModel();
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

   
   // Disable and unbind
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   glUseProgram(0);
   assert(glGetError() == GL_NO_ERROR);
}



void Penguin::drawPenguinModel() {
   ModelTrans.loadIdentity();
   ModelTrans.translate(position);
   ModelTrans.rotate(rotation, glm::vec3(0, 1, 0));
   ModelTrans.pushMatrix();

      // BODY
   ModelTrans.translate(glm::vec3(0, -.8, 0));   
   ModelTrans.rotate(upperTheta/3, glm::vec3(0, 0, 1));
   ModelTrans.translate(glm::vec3(0, 1, 0));
         if (waddle > 3.0) {
            waddleDirection = -1.0;
         } else if (waddle < -3.0) {
            waddleDirection = 1.0;
         }
         waddle += waddleDirection * .035;
   
   ModelTrans.pushMatrix();

      // TUMMY
      ModelTrans.scale(.6, 1, .5);
      ModelTrans.translate(glm::vec3(0, -.3, .65));
      ModelTrans.rotate(30, glm::vec3(1, 0, 0));
      ModelTrans.scale(.65, .6, .4);

      // draw tummy
      glUniformMatrix4fv(h_uModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
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
         upperTheta += direction * .7;
      }

      // draw right arm
      ModelTrans.scale(.6, 1, .5);
      glUniformMatrix4fv(h_uModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
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
         upperTheta += direction * .05;
      }

      // draw left arm
      ModelTrans.scale(.6, 1, .5);
      glUniformMatrix4fv(h_uModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

      ModelTrans.popMatrix();

   ModelTrans.pushMatrix();
      //BEAK
      ModelTrans.scale(.6, 1, .5);
      ModelTrans.translate(glm::vec3(0, .3, .8));
      ModelTrans.scale(.3, .05, .3);

      // draw beak
      glUniformMatrix4fv(h_uModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

      ModelTrans.popMatrix();

   ModelTrans.pushMatrix();
      // LEFT EYE
      ModelTrans.scale(.6, 1, .5);
      ModelTrans.translate(glm::vec3(-.28, .5, .71));
      ModelTrans.scale(.06, .03, .05);

      // draw left eye
      glUniformMatrix4fv(h_uModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

      ModelTrans.popMatrix();
   
   ModelTrans.pushMatrix();
      // RIGHT EYE
      ModelTrans.scale(.6, 1, .5);
      ModelTrans.translate(glm::vec3(.28, .5, .71));
      ModelTrans.scale(.06, .03, .05);

      // draw right eye
      glUniformMatrix4fv(h_uModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

      ModelTrans.popMatrix();

   ModelTrans.pushMatrix();
      // LEFT FOOT
      ModelTrans.scale(.6, 1, .5);
      ModelTrans.translate(glm::vec3(-.28, -.9, .1));
      ModelTrans.rotate( upperTheta/3-5, glm::vec3(1, 0, 0));
      ModelTrans.translate(glm::vec3(0, 0, .45));
      ModelTrans.scale(.25, .05, .6);

      // draw left foot
      glUniformMatrix4fv(h_uModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
      
      ModelTrans.popMatrix();

   ModelTrans.pushMatrix();
      // RIGHT FOOT
      ModelTrans.scale(.6, 1, .5);
      ModelTrans.translate(glm::vec3(.28, -.9, .1));
      ModelTrans.rotate(-upperTheta/3-5, glm::vec3(1, 0, 0));
      ModelTrans.translate(glm::vec3(0, 0, .45));
      ModelTrans.scale(.25, .05, .6);

      // draw right foot
      glUniformMatrix4fv(h_uModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
      glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
      
      ModelTrans.popMatrix();

   // draw body
   ModelTrans.scale(.6, 1, .5);
   glUniformMatrix4fv(h_uModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelTrans.modelViewMatrix));
   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
   ModelTrans.popMatrix();
}

void Penguin::loadShapes(const string &objFile, std::vector<tinyobj::shape_t>& shapes)
{
   string err = tinyobj::LoadObj(shapes, materials, objFile.c_str());
   if(!err.empty()) {
      cerr << err << endl;
   }
   //resize_obj(shapes); WAT
}

void Penguin::SetLightModel() {
  glm::mat4 Trans = glm::translate( glm::mat4(1.0f), g_light);
  
  glm::mat4 com = Trans;
  glUniformMatrix4fv(h_uModelMatrix, 1, GL_FALSE, glm::value_ptr(com));
}

