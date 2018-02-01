#version 330 core

in vec3 normal;
in vec3 pos;

out vec4 fragColor;

uniform vec3 camPos;
uniform samplerCube skybox;

void main()
{
  float ratio = 1.00/1.52;
  vec3 I = normalize(pos - camPos);
  vec3 R = refract(I, normalize(normal), ratio);
  fragColor = vec4(texture(skybox, R).rgb, 1.0);
}