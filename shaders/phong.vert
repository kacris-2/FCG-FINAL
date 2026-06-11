#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 vp;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    // Calculamos las normales de forma segura para iluminar la esfera
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = vp * vec4(FragPos, 1.0);
}