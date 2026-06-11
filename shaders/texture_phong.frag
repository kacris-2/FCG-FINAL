#version 410 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse;
uniform vec3 lightPos;   
uniform vec3 viewPos;    
uniform bool isSun;      

void main()
{
    vec4 texColor = texture(texture_diffuse, TexCoords);

    if (isSun) {
        // El sol brilla directamente con su textura ignorando oscuridades
        FragColor = texColor;
    } 
    else {
        // Luz Ambiental básica
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
        
        // Luz Difusa basada en la posición central del Sol (0,0,0)
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
        
        vec3 result = (ambient + diffuse) * texColor.rgb;
        FragColor = vec4(result, texColor.a);
    }
}