/* Kim Arre
 * CPE 471
 * February 19, 2015
 */

varying vec3 vCol;

uniform float whichSphere;
uniform float colorByNormals;
varying vec3 interpolatedNormal;
varying vec3 interpolatedPos;


uniform vec3 uLightPos;
uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;
uniform int uShadeModel;

void main()
{
   if (colorByNormals > 0.0) {
      gl_FragColor = vec4(vCol, 1.0);
   }

   else if (uShadeModel == 0) {   // GOURAUD
       gl_FragColor = vec4(vCol, 1.0);
   } 
   else {    // PHONG

      vec3 Ia = .1 * UaColor;

      vec3 L = normalize(uLightPos - interpolatedNormal.xyz);
      vec3 Id = .9 * dot(normalize(interpolatedNormal), L) * UdColor;

      vec3 H = normalize(L + interpolatedPos.xyz);
      float NdotH = dot(normalize(interpolatedNormal), H);
      vec3 Is = pow(NdotH, Ushine) * UsColor;

      gl_FragColor = vec4(Ia + Id + Is, 1.0);

      /*vec3 Ia = .1 * vec3(.29, 1.0, .9);
      vec3 L = normalize(uLightPos - interpolatedPos);
      vec3 Id = .9 * dot(normalize(interpolatedNormal), L) * vec3(.29, 1.0, .9);
      gl_FragColor = vec4(Ia + Id, 1.0);*/
   }
}
