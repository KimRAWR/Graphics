/* Kim Arre
 * CPE 471
 * February 19, 2015
 */

attribute vec4 aPos;
attribute vec3 aNor;
uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform vec3 uLightPos;
varying vec3 vCol;
uniform float whichSphere;
uniform float colorByNormals;

uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;
uniform int uShadeModel;


varying vec3 interpolatedNormal;
varying vec3 interpolatedPos;

vec3 updated_aNor = (M * V * vec4(aNor, 0.0)).xyz;


void main()
{
    gl_Position = P * M * V * aPos;

    if (colorByNormals > 0.0) {
      vCol = (updated_aNor + 1.0) * .5;
    }
    
    else if (uShadeModel == 0) {    // LEFT SPHERE, GOURAUD (vertex)
      vec3 Ia = .1 * UaColor;

      vec3 L = normalize(uLightPos - (M * V * aPos).xyz);
      vec3 Id = .9 * dot(normalize(updated_aNor), L) * UdColor;

      vec3 H = normalize(L - normalize(vec3(0.0, 0.0, -2.5) - (M * V * aPos).xyz));
      float NdotH = dot(normalize(updated_aNor), H);
      vec3 Is = pow(NdotH, Ushine) * UsColor;

      vCol = Ia + Id + Is;

   } else {                    // RIGHT SPHERE, PHONG (fragment)
      interpolatedPos = gl_Position.xyz;
      interpolatedNormal = updated_aNor;
   }
}
