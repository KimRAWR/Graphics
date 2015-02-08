/*
 * Kim Arre
 * CPE 471 - Winter 2015
 * Section 01
 */

#version 120

// Input vertex data, different for all executions of this shader.
attribute vec3 vertexPosition_modelspace;
attribute vec3 vertexColor;

// Output data ; will be interpolated for each fragment.
varying vec3 fragmentColor;

uniform float time;

// the center of the screen in world space is (0, 0) which is why vector movement is just the negative of vertexPosition_modelspace

void main() {
    float rate;
    
    vec2 vector = vec2(0 - vertexPosition_modelspace.x, 0 - vertexPosition_modelspace.y);
    vec3 movement = vec3(vector.x * time, vector.y * time, 1);  // figures out the vector, direction and magnitude
    
    vec3 position = vertexPosition_modelspace + movement;
    vec2 center = vec2(0.0f, 0.0f);
    float myDist = distance(vec2(position), vec2(center));
    
    if (myDist > .25) {
        gl_Position = vec4(position, 1);
        
    }
    else {  // don't animate
        
        vec2 temp = vec2(vertexPosition_modelspace);
        vec2 stopPoint = normalize(temp);
        stopPoint = stopPoint * .25;
        
        gl_Position = vec4(stopPoint, 0, 1);
    }
    fragmentColor = vertexColor;
}


