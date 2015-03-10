#pragma once
#include "GLIncludes.h"
#include "GLSL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

class Penguin {
   public:
   Penguin();
   glm::vec3 position;
   float rotation;
   int checkRunAway(glm::vec3 eye, glm::vec3 penguin);


};
