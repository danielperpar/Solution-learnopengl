#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform bool horizontal;
uniform sampler2D colorBuffer;

#define numSamples 9

void main()
{
  float weights[numSamples] = { 0.016216, 0.054054, 0.1216216, 0.1945946, 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216};
  vec2 texSize = textureSize(colorBuffer, 0);
  vec2 texelSize = vec2(1.0/texSize.x,1.0/texSize.y); 
  vec3 blurColor = vec3(0.0);
  if(horizontal)
  {
    for(int i = -numSamples/2; i <= numSamples/2; i++)
	{
	  float coordX = texCoords.x + i * texelSize.x;
	  vec3 color = texture(colorBuffer, vec2(coordX, texCoords.y)).rgb;
	  blurColor += weights[i + 4] * color;
	}
  }
  else
  {
    for(int i = -numSamples/2; i <= numSamples/2; i++)
	{
	  float coordY = texCoords.y + i * texelSize.y;
	  vec3 color = texture(colorBuffer, vec2(texCoords.x, coordY)).rgb;
	  blurColor += weights[i + 4] * color;
	}
  }
  fragColor = vec4(blurColor, 1.0);
}
