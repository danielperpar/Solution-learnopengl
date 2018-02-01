#version 330 core

struct Material
{
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  sampler2D texture_normal1;
  sampler2D texture_height1;
  
  float shininess;
};

struct PointLight
{
  vec3 position;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  
  float constant;
  float linear;
  float quadratic;
};

in vec3 normal;
in vec2 texCoord;
in vec3 fragPos;

out vec4 fragColor;

uniform Material material;
uniform PointLight pointLight1;
uniform PointLight pointLight2;
uniform vec3 viewPos;

vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir);

void main()
{
  vec3 lighting = vec3(0.0, 0.0, 0.0);
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 norm = normalize(normal);
  lighting += CalcPointLight(pointLight1, norm, viewDir);
  lighting += CalcPointLight(pointLight2, norm, viewDir);  
  
  fragColor = vec4(lighting, 1.0);
}

vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir)
{
  vec3 ambient = pointLight.ambient * texture(material.texture_diffuse1, texCoord).rgb;
  
  vec3 lightDir = normalize(pointLight.position - fragPos);
  
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = pointLight.diffuse * diff * texture(material.texture_diffuse1, texCoord).rgb;
  
  vec3 refl = reflect(-lightDir, normal);
  float spec = pow(max(dot(refl, viewDir),0.0), material.shininess);
  vec3 specular = pointLight.specular * spec * texture(material.texture_specular1, texCoord).rgb;
  
  float distance = length(pointLight.position - fragPos);
  float att = 1.0/(pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);
  
  ambient *= att;
  diffuse *= att;
  specular *= att;
  
  return (ambient + diffuse + specular);
}

