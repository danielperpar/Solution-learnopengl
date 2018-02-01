#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D colorBuffer;

void main()
{
  vec3 color = texture(colorBuffer, texCoords).rgb;
  fragColor = vec4(color, 1.0);
}