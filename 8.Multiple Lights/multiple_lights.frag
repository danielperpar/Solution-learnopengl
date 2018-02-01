#version 330 core

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct DirLight
{
  vec3 direction;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
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

struct SpotLight
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

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 fragColor;

uniform Material material;
uniform DirLight dirLight;
uniform vec3 viewPos;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
  vec3 norm = normalize(normal);
  vec3 viewDir = normalize(viewPos - fragPos);
  
  vec3 result = vec3(0.0, 0.0, 0.0);
  result += CalcDirLight(dirLight, norm, viewDir);
  
  for(int i = 0; i < NR_POINT_LIGHTS; i++)
  {
    result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
  }
  
  //result += CalcSpotLight(spotLight, norm, fragPos, viewDir);
  
  fragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir)
{
  vec3 ambient = dirLight.ambient * texture(material.diffuse, texCoord).rgb;
  
  vec3 dir = normalize(-dirLight.direction); 
  float diff = max(dot(dir, normal), 0.0);
  vec3 diffuse = dirLight.diffuse * diff * texture(material.diffuse, texCoord).rgb;
  
  vec3 refl = reflect(-dir, normal);
  float spec = pow(max(dot(refl, viewDir), 0.0), material.shininess);
  vec3 specular = dirLight.specular * spec * texture(material.specular, texCoord).rgb;
 
  return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 ambient = pointLight.ambient * texture(material.diffuse, texCoord).rgb;

  vec3 dir = normalize(pointLight.position - fragPos);
  
  float diff = max(dot(dir, normal),0.0);
  vec3 diffuse = pointLight.diffuse * diff * texture(material.diffuse, texCoord).rgb;
  
  vec3 refl = reflect(-dir, normal);
  float spec = pow(max(dot(viewDir, refl), 0.0), material.shininess);
  vec3 specular = pointLight.specular * spec * texture(material.specular, texCoord).rgb;
  
  float distance = length(pointLight.position - fragPos);
  float att = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);
  
  ambient *= att;
  diffuse *= att;
  specular *= att;
  
  return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 ambient = spotLight.ambient * texture(material.diffuse, texCoord).rgb;

  vec3 dir = normalize(spotLight.position - fragPos);
  
  float diff = max(dot(dir, normal),0.0);
  vec3 diffuse = spotLight.diffuse * diff * texture(material.diffuse, texCoord).rgb;
  
  vec3 refl = reflect(-dir, normal);
  float spec = pow(max(dot(refl, viewDir),0.0),material.shininess);
  vec3 specular = spotLight.specular * spec * texture(material.specular, texCoord).rgb;
  
  float theta = dot(dir, -spotLight.direction);
  float epsilon = spotLight.cutoff - spotLight.outerCutoff;
  float intensity = clamp((theta - spotLight.outerCutoff) / epsilon, 0.0, 1.0);
  
  ambient *= intensity;
  diffuse *= intensity;
  specular *= intensity;
  
  float distance = length(spotLight.position - fragPos);
  float att = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * distance * distance);
  
  ambient *= att;
  diffuse *= att;
  specular *= att;
  
  return ambient + diffuse + specular;
}



