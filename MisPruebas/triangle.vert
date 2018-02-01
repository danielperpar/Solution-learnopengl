#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 pos;

void main()
{
   gl_Position = vec4(aPos, 1.0f);
   pos = gl_Position;
}