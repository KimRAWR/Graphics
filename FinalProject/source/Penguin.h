#pragma once
#include "GLIncludes.h"
#include "GLSL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "shaderProgram.h"
#include "RenderingHelper.h"

class Penguin {
   public:
   Penguin();
   glm::vec3 position;
   float rotation;
   glm::vec3 runningVector;
   int material;
   RenderingHelper ModelTrans;
   bool isInfected;

   float g_angle;
   float g_vertAngle;
   int g_mat_id;
   glm::vec3 g_trans;
   glm::vec3 g_light;

   ShaderProgram ShadeProg;

   std::vector<tinyobj::shape_t> shapes;
   std::vector<tinyobj::material_t> materials;

   GLuint posBufObj;
   GLuint norBufObj;
   GLuint indBufObj;
   GLuint nIndices;
   GLuint posBufObjP;
   GLuint norBufObjP;
   GLuint indBufObjP;

   float upperTheta;
   float waddle;
   float waddleDirection;
   float direction;
   bool wave;

   GLint h_aPosition;
   GLint h_aNormal;
   GLint h_uModelMatrix;
   GLint h_uViewMatrix;
   GLint h_uProjMatrix;
   GLint h_uLightPos;
   GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;
   GLint h_cameratrans;

   int checkRunAway(glm::vec3 eye);
   void updateFacingDirection();
   void draw(glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up);
   void drawPenguinModel();
   void loadShapes(const std::string &objFile, std::vector<tinyobj::shape_t>& shapes);
   void SetLightModel();
   inline void safe_glUniformMatrix4fv(const GLint handle, const GLfloat data[]);
};
