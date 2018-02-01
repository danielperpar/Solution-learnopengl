#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out VS_OUT
{
  vec2 texCoords;
  vec3 fragPosTBN;
  vec3 viewPosTBN;
  vec3 lightPosTBN;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0);
  vs_out.texCoords = aTexCoords;
  
  mat3 normalMatrix = transpose(inverse(mat3(model)));
 
  vec3 normal = normalize(normalMatrix * aNormal);
  vec3 tangent = normalize(normalMatrix * aTangent);
  
  tangent = tangent - dot(tangent,normal) * normal;
  vec3 bitangent = cross(normal,tangent);
  
  mat3 TBN = transpose(mat3(tangent, bitangent, normal));
  
  vs_out.fragPosTBN = TBN * vec3(model * vec4(aPos, 1.0));
  vs_out.viewPosTBN = TBN * viewPos;
  vs_out.lightPosTBN = TBN * lightPos;
}