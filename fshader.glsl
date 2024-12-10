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

    vec3 normal = normalize(N);
    vec3 lightDir = normalize(L);
    vec3 viewDir = normalize(V);

    // Ambient
    vec4 ambient = vec4(0.0);
    if (useAmbient) {
        ambient = ambientStrength * lightColor;
    }
    
    // Diffuse
    vec4 diffuse = vec4(0.0);
    if (useDiffuse) {
        float diff = max(dot(normal, lightDir), 0.0);
        diffuse = diffuseStrength * diff * lightColor;
    }

    // Specular
    vec4 specular = vec4(0.0);
    if (useSpecular) {
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        specular = specularStrength * spec * lightColor;
    }

    // Combine lighting with distance attenuation
    float distance = length(L);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
    
    vec4 lighting = ambient + (diffuse + specular) * attenuation;  

    gl_FragColor = color;
}