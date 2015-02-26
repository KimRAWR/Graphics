
uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;
uniform vec3 uLightPos;

varying vec3 vCol;
varying vec3 interpolatedNormal;
varying vec3 interpolatedPos;

void main()
{
	//gl_FragColor = vec4(vCol.r, vCol.g, vCol.b, 1.0);

   vec3 Ia = .1 * UaColor;

   vec3 L = normalize(uLightPos );//- interpolatedNormal.xyz);
   vec3 Id = .9 * max(0.0, dot(normalize(interpolatedNormal), L)) * UdColor;

   vec3 H = normalize(L + interpolatedPos.xyz);
   float NdotH = max(0.0, dot(normalize(interpolatedNormal), H));
   vec3 Is = pow(NdotH, Ushine) * UsColor;

   gl_FragColor = vec4(Ia + Id + Is, 1.0);
}
