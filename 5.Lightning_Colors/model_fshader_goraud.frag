#version 330 core

out vec4 FragColor;
in vec3 lightning;

uniform vec3 objectColor;

void main()
{ 
   FragColor = vec4(lightning * objectColor, 1.0) ;
}