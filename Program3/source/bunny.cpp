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

LightObj::LightObj() : shader("lightVert.glsl", "lightFrag.glsl") {

}