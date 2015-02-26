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
    h_aPosition = GLSL::getAttribLocation(ShadeProg, "aPos");
    h_aNormal = GLSL::getAttribLocation(ShadeProg, "aNor");
    h_uProjMatrix = GLSL::getUniformLocation(ShadeProg, "P");
    h_uViewMatrix = GLSL::getUniformLocation(ShadeProg, "V");
    h_uModelMatrix = GLSL::getUniformLocation(ShadeProg, "M");
    h_uLightPos = GLSL::getUniformLocation(ShadeProg, "uLightPos");
}