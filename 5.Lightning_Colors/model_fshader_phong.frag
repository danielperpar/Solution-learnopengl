#version 330 core

struct Material
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light
{
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

out vec4 FragColor;

in vec3 normal; 
in vec3 FragPos;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform vec3 objectColor;

void main()
{ 
   vec3 ambient = light.ambient * material.ambient;

   vec3 lightDir = normalize(light.position - FragPos);
   vec3 norm = normalize(normal);
   float diff = max(dot(lightDir, norm), 0.0);
   vec3 diffuse = light.diffuse * (diff * material.diffuse);
   
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 refDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir,refDir), 0.0), material.shininess);
   vec3 specular = light.specular * (spec * material.specular);
   
   vec3 result = ambient + diffuse + specular;
   FragColor = vec4(result, 1.0);
}