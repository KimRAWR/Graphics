#include "shaderProgram.h"
#include "GLIncludes.h"
#include "GLSL.h"
#include <stdlib.h>
#include <stdio.h>

ShaderProgram::ShaderProgram(const std::string &vShaderName, const std::string &fShaderName) {
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
      exit(1);
   }
   
   // Compile fragment shader
   glCompileShader(FS);
   GLSL::printError();
   glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
   GLSL::printShaderInfoLog(FS);
   if(!rc) {
      printf("Error compiling fragment shader %s\n", fShaderName.c_str());
      exit(1);
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
      exit(1);
   }
}

void ShaderProgram::use() {
   glUseProgram(prog);
}