#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D diffuseTexture;
uniform bool isSpecular;

void main()
{
  vec3 color;
  if(isSpecular)
	 {
	  float specMapValue = texture(diffuseTexture, texCoords).a;
	  color = vec3(specMapValue);
	 }
  else
    color = texture(diffuseTexture, texCoords).rgb;
	
  fragColor = vec4(color, 1.0);
}