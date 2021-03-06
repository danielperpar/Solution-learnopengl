#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;

out vec2 texCoord;

layout(std140) uniform Matrices
{
  mat4 model;
  mat4 view;
  mat4 projection;
};

void main()
{
  gl_Position = projection * view * model * vec4(aPos,1.0);
  texCoord = aTexCoord;
}