#version 330 core

in VS_OUT
{
  vec2 texCoords;
  vec3 fragPos;
  vec3 normal;
}fs_in;

out vec4 fragColor;

uniform sampler2D texture_diffuse;
uniform vec3 lightPos1;
uniform vec3 lightColor1;
uniform vec3 lightPos2;
uniform vec3 lightColor2;
uniform vec3 viewPos;

uniform float attexp;

void main()
{
  vec3 color = texture(texture_diffuse, fs_in.texCoords).rgb;
  
  //vec3 ambient1 = 0.0 * lightColor1 * color;
  //vec3 ambient2 = 0.0 * lightColor2 * color;
  
  vec3 fragToLight1 = normalize(lightPos1 - fs_in.fragPos);
  float diff1 = max(dot(fragToLight1, normalize(fs_in.normal)), 0.0);
  vec3 diffuse1 = diff1 * lightColor1 * color;
  float att1 = 1 / pow(length(lightPos1 - fs_in.fragPos),attexp);
  diffuse1 = att1 * diffuse1;
  
  vec3 fragToLight2 = normalize(lightPos2 - fs_in.fragPos);
  float diff2 = max(dot(fragToLight2, normalize(fs_in.normal)), 0.0);
  vec3 diffuse2 = diff2 * lightColor2 * color;
  float att2 = 1 / pow(length(lightPos2 - fs_in.fragPos),attexp);
  diffuse2 = att2 * diffuse2;
  
  fragColor = vec4(diffuse1 + diffuse2, 1.0);
}

