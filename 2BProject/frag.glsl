varying vec3 vCol;
varying vec4 normalVal;
uniform float globe;

void main()
{
	if (globe < 0.0) {
      gl_FragColor = vec4(vCol.r, vCol.g, vCol.b, 1.0);
   }
   else {
      gl_FragColor = vec4(0.9 - normalVal.z * -1.0, 0.9-normalVal.z * -1.0, 0.9-normalVal.z * -1.0, .6);

      if (normalVal.z > 0.0) {
         gl_FragColor = vec4(0.8 - normalVal.z * -1.0, 0.8-normalVal.z * -1.0, 0.8-normalVal.z * -1.0, .5);
      }
      if (normalVal.z > 0.2) {
         discard;
      }
   }

}
