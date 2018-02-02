#version 330 core

struct Light
{
  vec3 position;
  vec3 color;
};

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

#define numLights 10
out VS_OUT
{
  vec2 texCoords;
  Light outLights[numLights];
}vs_out;
  
uniform Light lights[numLights];
uniform mat4 lightModel;

void main()
{
  gl_Position = vec4(aPos, 1.0);
  vs_out.texCoords = aTexCoords;
  
  for(int i = 0; i < numLights; i++)
  {
    vs_out.outLights[i].position = vec3(lightModel * vec4(lights[i].position, 1.0));
  	vs_out.outLights[i].color = lights[i].color;
  }
}