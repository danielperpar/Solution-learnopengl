#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;

out VS_OUT
{
 vec2 texCoord;
 float time_vs;
}vs_out;

layout(std140) uniform Matrices
{
  mat4 model;
  mat4 view;
  mat4 projection;
  float time;
};

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0);
  vs_out.texCoord = aTexCoord;
  vs_out.time_vs = time;
}