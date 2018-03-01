#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec3 aNormal;

out vec4 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  mat3 normalMatrix = mat3(transpose(inverse(view * model)));
  normal = normalize(projection * vec4(normalMatrix * aNormal, 0.0));
  gl_Position = projection * view * model * vec4(aPos,1.0);
}

