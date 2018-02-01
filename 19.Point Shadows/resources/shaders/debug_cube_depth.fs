#version 330 core

in vec4 fragPos;

out vec4 fragColor;

uniform samplerCube depthMap;
uniform vec3 lightPos;

void main()
{
  vec3 texCoord = fragPos.xyz - lightPos;
  float sample = texture(depthMap, texCoord).r;
  fragColor = vec4(sample, sample, sample, 1.0);
}