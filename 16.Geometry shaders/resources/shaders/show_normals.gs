#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in vec4 normal[];

const float MAGNITUDE = 0.1;

void GenerateLine(int index)
{
  gl_Position = gl_in[index].gl_Position;
  EmitVertex();
  gl_Position = gl_in[index].gl_Position + normal[index] * MAGNITUDE;
  EmitVertex();
  EndPrimitive();
}

void main()
{
  GenerateLine(0);
  GenerateLine(1);
  GenerateLine(2);
}