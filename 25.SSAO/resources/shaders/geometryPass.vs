#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool flipNormal;

void main()
{
  vec4 viewPosition = view * model * vec4(aPos, 1.0);
  gl_Position = projection * viewPosition;
  fragPos = viewPosition.xyz;
  mat3 normalMatrix = transpose(inverse(mat3(view * model)));
  normal = normalMatrix * (flipNormal ? -aNormal : aNormal);
  texCoords = aTexCoords;
}


