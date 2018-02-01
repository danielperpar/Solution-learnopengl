#version 330 core

struct Material
{
  sampler2D texture_diffuse1;
  sampler2D texture_reflection1;
};

in vec3 pos;
in vec3 normal;
in vec2 texCoord;

out vec4 fragColor;

uniform Material material;
uniform vec3 camPos;
uniform samplerCube skybox;

void main()
{
  vec4 diffuse = texture(material.texture_diffuse1, texCoord);
  
  vec3 I = normalize(pos - camPos);
  vec3 R = reflect(I, normalize(normal));
  vec4 reflection;
  float refl = texture(material.texture_reflection1, texCoord).r;
  if(refl > 0.1)
     reflection = vec4(texture(skybox, R).rgb, 1.0);
  
  fragColor = diffuse + refl * reflection;
}