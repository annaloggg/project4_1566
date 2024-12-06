#version 120

varying vec4 color;
varying vec3 N, L, V;
varying vec3 worldPos;

uniform vec4 lightColor;
uniform float ambientStrength;
uniform float diffuseStrength; 
uniform float specularStrength;
uniform float shininess;
uniform bool useAmbient;
uniform bool useDiffuse;
uniform bool useSpecular;

void main() {

    gl_FragColor = color;
}