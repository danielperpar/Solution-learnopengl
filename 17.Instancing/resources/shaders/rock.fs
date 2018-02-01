#version 330 core

struct Material
{
  sampler2D texture_diffuse1;
};

in vec2 texCoord;

out vec4 fragColor;

uniform Material material;

void main()
{
  fragColor = vec4(texture(material.texture_diffuse1, texCoord).rgb, 1.0);
}