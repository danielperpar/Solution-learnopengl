#version 330 core

struct Material
{
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
};

in VS_OUT
{
  vec3 pos;
  vec3 normal;
  vec2 texCoords;  
}fs_in;

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

uniform Material material;

void main()
{
  gPosition = fs_in.pos;
  gNormal = normalize(fs_in.normal);
  gAlbedoSpec.rgb = texture(material.texture_diffuse1, fs_in.texCoords).rgb;
  gAlbedoSpec.a = texture(material.texture_specular1, fs_in.texCoords).r;
}