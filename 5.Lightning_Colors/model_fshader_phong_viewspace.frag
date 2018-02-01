#version 330 core
out vec4 FragColor;

in vec3 normal; 
in vec3 FragPos;
in vec3 lightPosition;

uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{ 
   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor;

   vec3 lightDir = normalize(lightPosition - FragPos);
   vec3 norm = normalize(normal);
   float diff = max(dot(lightDir, norm), 0.0);
   vec3 diffuse = diff * lightColor;
   
   vec3 viewDir = normalize(-FragPos);
   vec3 refDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir,refDir), 0.0), 32);
   float specularStrength = 0.5;
   vec3 specular = specularStrength * spec * lightColor;
   
   vec3 result = (specular + diffuse + ambient) * objectColor; 
   FragColor = vec4(result, 1.0);
}