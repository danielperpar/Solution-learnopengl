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


void main()
{
  vec3 lightColor = vec3(0.5);
  vec3 color = texture(texture_diffuse, fs_in.texCoords).rgb;
  
  vec3 ambient = lightColor * color;
  
  vec3 lightDir = normalize(fs_in.lightPosTBN - fs_in.fragPosTBN);
  vec3 normal = texture(texture_normal, fs_in.texCoords).rgb;
  normal = 2.0 * normal - 1.0;
  float diff_factor = max(dot(normal, lightDir),0.0);
  vec3 diffuse = lightColor * diff_factor * color;
  
  vec3 viewDir = normalize(fs_in.viewPosTBN - fs_in.fragPosTBN);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec_factor = pow(max(dot(normal, halfwayDir),0.0), 32.0);
  vec3 specular = lightColor * spec_factor * color;
  
  fragColor = vec4(ambient + diffuse + specular, 1.0);
}