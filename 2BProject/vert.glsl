attribute vec4 aPos;
attribute vec3 aNor;
attribute vec3 vertexPosition_modelspace;
uniform mat4 P;
uniform mat4 MV;
varying vec3 vCol;
varying vec4 normalVal;


void main()
{
	gl_Position = P * MV * aPos;
	vCol = 0.5*(aNor + 1.0);
   normalVal = MV * vec4(aNor,0);
}
