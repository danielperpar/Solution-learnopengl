#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 normal;
out vec3 lightPosition;

uniform vec3 lightPos;
uniform mat4 lightModel;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normMatrix;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   normal = mat3(normMatrix) * aNormal;
   FragPos = vec3(view * model * vec4(aPos, 1.0));
   lightPosition = vec3(view * lightModel * vec4(lightPos, 1.0));
}