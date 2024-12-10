#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec4 vNormal;

varying vec4 color;
varying vec3 N, L, V;

uniform mat4 ctm;
uniform mat4 projection;
uniform mat4 model_view;
uniform vec4 lightPosition;  // Light position in world space

void main() {

    color = vColor;
    gl_Position = projection * model_view * ctm * vPosition;
}