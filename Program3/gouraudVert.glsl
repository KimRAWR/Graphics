attribute vec4 aPos;
attribute vec3 aNor;
uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
varying vec3 vCol;

varying vec3 interpolatedNormal;
varying vec3 interpolatedPos;

vec3 light = vec3(0, 1, 2);

void main()
{
   gl_Position = P * M * V * aPos;

   vec3 Ia = .1 * vec3(.29, 1.0, .9);
   vec3 L = normalize(light - aPos.xyz);
   vec3 Id = .9 * dot(normalize(aNor), L) * vec3(.29, 1.0, .9);
   vCol = Ia + Id;
}
