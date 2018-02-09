#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gAlbedo;

void main()
{
  gPosition = fragPos;
  gNormal = normalize(normal);
  gAlbedo = vec3(0.95); 
}