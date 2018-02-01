#version 330 core

in vec3 normal;
in vec2 texCoord;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float farPlane;
uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float shadowCalculation(vec3 fragPos)
{
    float viewDistance = length(fragPos - viewPos);
    float discRadius = (1.0 + (viewDistance/farPlane))/25.0;
	int samples = 20;
    vec3 fragToLight = fragPos - lightPos;
	float shadow = 0.0;
	float closestDepth = 0.0;
	float currentDepth = length(fragToLight);
	float bias = 0.6;	
	for(int i = 0; i < samples; i++)
	{
      closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i]*discRadius).r;
	  closestDepth = closestDepth * farPlane;
	  
	  shadow += (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
    }
    shadow /= float(samples);    
	
    //closestDepth = closestDepth * farPlane;	
    //float currentDepth = length(fragToLight);
	//float bias = 0.6;	
	//float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;        
    // display closestDepth as debug (to visualize depth cubemap)
    //fragColor = vec4(closestDepth, closestDepth, closestDepth, 1.0);    
        
    return shadow;	
}

void main()
{
  vec3 lightColor = vec3(0.3);
  vec3 color = texture(diffuseTexture, texCoord).rgb;
  
  vec3 ambient = 0.3 * color;
  
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - fragPos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 halfwayDir = normalize(viewDir + lightDir);
  float spec = pow(max(dot(norm, halfwayDir),0.0),64.0);
  vec3 specular = spec * lightColor;
  
  float shadow = shadowCalculation(fragPos);
  
  vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
  
  fragColor = vec4(lighting, 1.0);
}

