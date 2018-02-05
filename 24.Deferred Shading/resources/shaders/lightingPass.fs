#version 330 core

struct Light
{
  vec3 position;
  vec3 color;
  
  float linear;
  float quadratic;
  float radius;
};

in vec2 texCoords;
  
out vec4 fragColor;

uniform vec3 viewPos;
uniform sampler2D gPositions;
uniform sampler2D gNormals;
uniform sampler2D gAlbedoSpec;
#define numLights 32
uniform Light lights[numLights];

void main()
{
  vec3 fragPos = texture(gPositions, texCoords).rgb;
  vec3 normal = texture(gNormals, texCoords).rgb;
  vec3 albedo = texture(gAlbedoSpec, texCoords).rgb;
  float specMap = texture(gAlbedoSpec, texCoords).a;
  
  vec3 lighting;
  vec3 fragToView = normalize(viewPos - fragPos);
  
  for(int i = 0; i < numLights; i++)
  {
    float distance = length(lights[i].position - fragPos);
	lighting = albedo * 0.1;
    if(distance < lights[i].radius)
    {
	  vec3 fragToLight = normalize(lights[i].position - fragPos);
	  float diff = max(dot(fragToLight, normal), 0.0);
	  vec3 diffuse = diff * lights[i].color * albedo;
	
	  vec3 halfwayDir = normalize(fragToLight + fragToView);
	  float spec = pow(max(dot(normal, halfwayDir),0.0),64.0);
	  vec3 specular = spec * lights[i].color * specMap;
	
	  float attenuation = 1.0 / (1.0 + lights[i].linear * distance);// + lights[i].quadratic * distance * distance);
      diffuse *= attenuation;
      specular *= attenuation;
      lighting += diffuse + specular; 
	}
  }
  
  fragColor = vec4(lighting, 1.0);
}
