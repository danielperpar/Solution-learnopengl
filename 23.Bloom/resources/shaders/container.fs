#version 330 core

struct Light
{
  vec3 position;
  vec3 color;
};

in VS_OUT
{
  vec3 fragPos;
  vec3 normal;
  vec2 texCoords;
}fs_in;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

uniform vec3 viewPos;
#define numLights 4
uniform Light lights[numLights];

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform bool isContainer;
uniform float attExp;

void main()
{
  vec3 diffuse = vec3(0.0);
  vec3 specular = vec3(0.0);
  vec3 lighting = vec3(0.0);
  vec3 fragToLight = vec3(0.0);
  vec3 fragToView = normalize(viewPos - fs_in.fragPos);
  vec3 diffuseColor = texture(diffuseTexture, fs_in.texCoords).rgb;
  vec3 ambient = 0.0 * diffuseColor;
  
  for(int i = 0; i < numLights; i++)
  {
    fragToLight = normalize(lights[i].position - fs_in.fragPos);
    float diff = max(dot(fs_in.normal, fragToLight), 0.0);
	diffuse = diff * lights[i].color * diffuseColor;
	
	vec3 halfwayDir = normalize(fs_in.normal + fragToView);
	float spec = pow(max(dot(fs_in.normal, halfwayDir), 0.0), 64.0);
	if(isContainer)
	  specular = spec * lights[i].color * texture(specularTexture, fs_in.texCoords).rgb * diffuseColor;
	else
	  specular = spec * lights[i].color * diffuseColor;
	
	float distance = length(lights[i].position - fs_in.fragPos);
	float att = 1/pow(distance, attExp);
	lighting += att * (ambient + diffuse + specular);
  }
  
  float brightness = dot(lighting, vec3(0.20, 0.70, 0.10));
  fragColor = vec4(lighting, 1.0);
  brightColor = vec4(vec3(0.0), 1.0);
  if(brightness > 1.0)
    brightColor = fragColor;
}