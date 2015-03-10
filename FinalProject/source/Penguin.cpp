#define NUM_PENGUINS 10

#include "GLIncludes.h"
#include "GLSL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "Penguin.h"


Penguin::Penguin() {
   glm::vec3 position; 
   float rotation;
   glm::vec3 runningVector = glm::vec3(0.0, 0.0, 0.0);
   int material = 1; // default to flat grey, should be rose gold when touched
}

// Returns the material number it should have
int Penguin::checkRunAway(glm::vec3 eye) {
   if (glm::distance(eye, position) < 3) {
      material = 3;
      printf("penguin should run away!\n");

      glm::vec3 movement = glm::normalize(position - eye);
      runningVector += movement * glm::vec3(.1, 0, .1);

   } else if (glm::distance(eye, position) > 9) {
      runningVector = glm::vec3(0.0, 0.0, 0.0);
   }

   position += runningVector;
   return material;
}


