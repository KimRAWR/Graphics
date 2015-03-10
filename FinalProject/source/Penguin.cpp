#define NUM_PENGUINS 10

#include "GLIncludes.h"
#include "GLSL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "Penguin.h"

/*glm::vec3 runningVector[NUM_PENGUINS*3]; 

float g_penguin_position[NUM_PENGUINS*3]*/

Penguin::Penguin() {
   glm::vec3 position; 
   float rotation;

}

int Penguin::checkRunAway(glm::vec3 eye, glm::vec3 penguin) {
   if (glm::distance(eye, penguin) < 1.5) {
      
   }
   return 1;
}


