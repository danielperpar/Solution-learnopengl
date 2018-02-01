#version 330 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

uniform vec3 lightColor;

void main()
{
  fragColor = vec4(lightColor, 1.0);
  brightColor = vec4(vec3(0.0), 1.0);
  
  if(dot(lightColor, vec3(0.20, 0.70, 0.10)) > 1.0)
	brightColor = vec4(lightColor, 1.0);
}
