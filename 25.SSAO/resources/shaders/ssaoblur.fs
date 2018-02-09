#version 330 core

in vec2 texCoords;

out float fragColor;

uniform sampler2D ssaoMap;

void main()
{
  vec2 texelSize = 1.0/textureSize(ssaoMap,0);
  float color;
  for(int x = -2; x < 2; x++)
  {
    for(int y = -2; y < 2; y++)
    {
	 vec2 offset = vec2(float(x), float(y)) * texelSize;
     color += texture(ssaoMap, texCoords + offset).r; 
    }
  }
  color /= 16.0;
  fragColor = color;
}