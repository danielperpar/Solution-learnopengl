#version 330 core

in VS_OUT
{
  vec3 fragPos;
  vec3 normal;
  vec2 texCoords;
  vec4 fragPosLightSpace;
} fs_in;

out vec4 fragColor;

uniform sampler2D shadowMap;
uniform sampler2D diffuseTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
  float shadow = 0.0;
  
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;
  if(projCoords.z > 1.0)
  {
    return shadow;
  }
  else
  {
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	
	//percentage-closer filtering
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
      for(int y = -1; y <= 1; ++y)
      {
        float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
      }    
    }
    shadow /= 9.0;
  }
  return shadow;
}

void main()
{
  vec3 color = texture(diffuseTexture, fs_in.texCoords).rgb;
  vec3 normal = normalize(fs_in.normal);
  vec3 lightColor = vec3(0.3);
  
  vec3 ambient = 0.3 * color;
  
  vec3 lightDir = normalize(lightPos - fs_in.fragPos);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * lightColor;
  
  vec3 viewDir = normalize(viewPos - fs_in.fragPos);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir),0.0), 64.0);
  vec3 specular = spec * lightColor;
  
  float shadow = ShadowCalculation(fs_in.fragPosLightSpace, normal, lightDir);
  vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
  
  fragColor = vec4(lighting, 1.0);
}