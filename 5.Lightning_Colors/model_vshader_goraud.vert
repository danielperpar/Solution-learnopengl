#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 lightning;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform mat4 lightModel;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normMatrix;


void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   vec3 normal = mat3(normMatrix) * aNormal;
   vec3 vertPos = vec3(model * vec4(aPos, 1.0));
   vec3 lightPosition = vec3(lightModel * vec4(lightPos, 1.0));
   
   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor;

   vec3 lightDir = normalize(lightPosition - vertPos);
   vec3 norm = normalize(normal);
   float diff = max(dot(lightDir, norm), 0.0);
   vec3 diffuse = diff * lightColor;
   
   vec3 viewDir = normalize(viewPos - vertPos);
   vec3 refDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir,refDir), 0.0), 32);
   float specularStrength = 0.5;
   vec3 specular = 0.5 * spec * lightColor;
   
   lightning = specular + diffuse + ambient; 
   
}