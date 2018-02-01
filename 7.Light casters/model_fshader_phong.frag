#version 330 core

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light
{
  vec3 position;
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  
  float constant;
  float linear;
  float quadratic;
  float cutoff;
  float outerCutoff;
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
   
   float theta = dot(lightDir, normalize(-light.direction));
   float epsilon = light.cutoff - light.outerCutoff;
   float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
   diffuse *= intensity;
   specular *= intensity;
   
   float distance = length(light.position - FragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
   ambient *= attenuation;
   diffuse *= attenuation;
   specular *= attenuation;
	
   vec3 result = ambient + diffuse + specular;
   FragColor = vec4(result, 1.0);	 
}