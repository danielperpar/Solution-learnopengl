#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
  vec3 color = texture(gAlbedo, texCoords).rgb;
  float ambientOcclusion = texture(ssao, texCoords).r;
  
  vec3 ambient = 0.5 * ambientOcclusion * color;
  
  vec3 fragPos = texture(gPosition, texCoords).rgb;
  vec3 normal = texture(gNormal, texCoords).rgb;
  vec3 lightDir = normalize(lightPos - fragPos);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * color * lightColor;
  
  vec3 viewDir = -fragPos;
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(halfwayDir, normal), 0.0), 8.0);
  vec3 specular = spec * color * lightColor;
  
  float att = 1.0 / length(fragPos - lightPos);
  diffuse *= att;
  specular *= att;
  
  fragColor = vec4( (ambient + diffuse /*+ specular*/), 1.0);
}
