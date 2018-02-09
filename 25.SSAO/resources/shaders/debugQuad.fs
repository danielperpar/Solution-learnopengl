#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D diffuseTexture;

void main()
{
  fragColor = vec4(texture(diffuseTexture, texCoords).rgb, 1.0);
}