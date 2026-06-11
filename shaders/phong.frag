#version 410 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 color;

void main() {
   
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(0.0, 0.0, 0.0) - FragPos); 
    
    //ambient componetn
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * color;
    
    // diffuse component
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * color;
    
    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}