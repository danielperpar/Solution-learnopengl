#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D textureScene;
uniform sampler2D textureBrightness;
uniform float exposure;

uniform bool bloom;

void main()
{
  vec3 colorScene = texture(textureScene, texCoords).rgb;
  vec3 brightness = texture(textureBrightness, texCoords).rgb;
  vec3 bloomHDR = bloom ? colorScene + brightness : colorScene;
  vec3 bloomLDR = vec3(1.0) - exp(-bloomHDR * exposure);
  fragColor = vec4(bloomLDR, 1.0);
}