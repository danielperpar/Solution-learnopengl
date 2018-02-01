#version 330 core

in VS_OUT
{
  vec2 texCoords;
  vec3 fragPosTBN;
  vec3 viewPosTBN;
  vec3 lightPosTBN;
}fs_in;

out vec4 fragColor;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_depth;
uniform float depthScale;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
  int minLayers = 8;
  int maxLayers = 32;

  float numLayers = mix(maxLayers, minLayers, max(dot(viewDir, vec3(0.0,0.0,1.0)), 0.0));
  float layerDepth = 1.0 / numLayers;
  vec2 currentDispCoords = texCoords;
  float currentMapDepth = texture(texture_depth, currentDispCoords).r;
  float currentLayerDepth = 0.0;
  vec2 p = viewDir.xy * depthScale;
  while(currentMapDepth > currentLayerDepth)
  {
    currentDispCoords -= layerDepth * p;
	currentMapDepth = texture(texture_depth, currentDispCoords).r;
	currentLayerDepth += layerDepth;
  }
  vec2 prevDispCoords = currentDispCoords + layerDepth * p;
  float prevMapDepth = texture(texture_depth, prevDispCoords).r;
  float prevLayerDepth = currentLayerDepth - layerDepth;
  
  float d1 = abs(prevMapDepth - prevLayerDepth);
  float d2 = abs(currentMapDepth - currentLayerDepth);
  float w = d1 / (d1 + d2);
  vec2 dir = normalize(currentDispCoords - prevDispCoords);
  vec2 dispCoords = prevDispCoords + dir * length(currentDispCoords - prevDispCoords) * w;
  
  return dispCoords;
}

void main()
{
  vec3 viewDir = normalize(fs_in.viewPosTBN - fs_in.fragPosTBN);
  vec2 dispTexCoord = ParallaxMapping(fs_in.texCoords, viewDir);
  
  //if(dispTexCoord.x < 0.0 || dispTexCoord.y < 0.0 || dispTexCoord.x > 1.0 || dispTexCoord.y > 1.0)
  // discard;
  
  vec3 lightColor = vec3(0.3);
  vec3 color = texture(texture_diffuse, dispTexCoord).rgb;
  vec3 ambient = lightColor * color;
  
  vec3 lightDir = normalize(fs_in.lightPosTBN - fs_in.fragPosTBN);
  vec3 normal = texture(texture_normal, dispTexCoord).rgb;
  normal = 2.0 * normal - 1.0;
  float diff_factor = max(dot(normal, lightDir),0.0);
  vec3 diffuse = lightColor * diff_factor * color;
  
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec_factor = pow(max(dot(normal, halfwayDir),0.0), 32.0);
  vec3 specular = lightColor * spec_factor * color;
  
  fragColor = vec4(ambient + diffuse + specular, 1.0);
}