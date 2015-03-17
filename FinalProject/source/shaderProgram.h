#pragma once
#include <string>
#include "GLIncludes.h"

class ShaderProgram {
public:
   ShaderProgram(const std::string &vShaderName, const std::string &fShaderName);
   void use();
   GLuint prog;
};