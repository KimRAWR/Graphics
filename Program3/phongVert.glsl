attribute vec4 aPos;
attribute vec3 aNor;
uniform mat4 P;
uniform mat4 MV;
varying vec3 vCol;

varying vec3 interpolatedNormal;
varying vec3 interpolatedPos;


vec3 light = vec3(0, 1, 2);

void main()
{
   gl_Position = P * MV * aPos;

   interpolatedPos = gl_Position.xyz;
   interpolatedNormal = aNor;
   
}
