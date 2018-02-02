#version 330 core

struct Light
{
  vec3 position;
  vec3 color;
};
#define numLights 10
in VS_OUT
{
  vec2 texCoords;
  Light outLights[numLights];
}fs_in;

out vec4 fragColor;

uniform vec3 viewPos;
uniform sampler2D gPositions;
uniform sampler2D gNormals;
uniform sampler2D gAlbedoSpec;

void main()
{
  vec3 fragPos = texture(gPositions, fs_in.texCoords).rgb;
  vec3 normal = texture(gNormals, fs_in.texCoords).rgb;
  vec3 albedo = texture(gAlbedoSpec, fs_in.texCoords).rgb;
  float specMap = texture(gAlbedoSpec, fs_in.texCoords).a;
  
  vec3 lighting;
  
  for(int i = 0; i < numLights; i++)
  {
    vec3 fragToLight = normalize(fs_in.outLights[i].position - fragPos);
	float diff = max(dot(fragToLight, normal), 0.0);
	vec3 diffuse = diff * fs_in.outLights[i].color * albedo;
	
	vec3 fragToView = normalize(fs_in.outLights[i].position - fragPos);
	vec3 halfwayDir = normalize(fragToLight + fragToView);
	float spec = pow(max(dot(normal, halfwayDir),0.0),64.0);
	vec3 specular = spec * specMap * fs_in.outLights[i].color * albedo;
	
	vec3 ambient = vec3(0.3) * fs_in.outLights[i].color * albedo;
	
	float att = 1/length(fragPos - fs_in.outLights[i].position);
	
	lighting += att * (ambient + diffuse + specular);
  }
  
  fragColor = vec4(lighting, 1.0);
}
