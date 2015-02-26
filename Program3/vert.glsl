attribute vec4 aPos;
attribute vec3 aNor;
uniform mat4 P;
uniform mat4 MV;
varying vec3 vCol;

varying vec3 interpolatedNormal;
varying vec3 interpolatedPos;

vec3 updated_aNor = (MV * vec4(aNor, 0.0)).xyz;

void main()
{
	gl_Position = P * MV * aPos;
	//vCol = 0.5*(aNor + 1.0);

   interpolatedPos = gl_Position.xyz;
   interpolatedNormal = updated_aNor;
}
