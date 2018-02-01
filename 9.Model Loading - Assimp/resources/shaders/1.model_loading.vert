#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 normal;
out vec2 texCoord;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0);
  normal = mat3(normalMatrix) * aNormal;
  texCoord = aTexCoord;
  fragPos = vec3(view * model * vec4(aPos, 1.0));  
} 