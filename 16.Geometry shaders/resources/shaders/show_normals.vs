#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec4 normal;

layout(std140) uniform Matrices
{
  mat4 model;
  mat4 view;
  mat4 projection;
};

void main()
{
  mat3 normalMatrix = mat3(transpose(inverse(view * model)));
  normal = normalize(projection * vec4(normalMatrix * aNormal, 0.0));
  gl_Position = projection * view * model * vec4(aPos,1.0);
}

