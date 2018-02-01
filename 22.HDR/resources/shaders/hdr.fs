#version 330 core

in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texture_hdr;
uniform float exposure;

void main()
{
  vec3 hdrColor = texture(texture_hdr, texCoords).rgb;
  //Reinhard tone mapping
  //vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
  
  vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
  
  fragColor = vec4(mapped, 1.0);
  
}