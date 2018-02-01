#version 330 core

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emissive;
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
in vec2 texCoord;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

uniform float emissionStrength;
uniform float time;

void main()
{ 
   vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;

   vec3 lightDir = normalize(light.position - FragPos);
   vec3 norm = normalize(normal);
   float diff = max(dot(lightDir, norm), 0.0);
   vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoord).rgb;
   
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 refDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir,refDir), 0.0), material.shininess);
   vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;
   
   
   vec2 newTexCoord = texCoord + vec2(0.0, time);
   //vec3 emissive = emissionStrength * texture(material.emissive, newTexCoord).rgb;
   
   vec3 result = ambient + diffuse + specular /*+ emissive*/;
   FragColor = vec4(result, 1.0);
}