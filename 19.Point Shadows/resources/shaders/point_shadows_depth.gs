#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

out vec4 fragPos;

uniform mat4 pointLightMatrices[6];

void main()
{
  for(int face = 0; face < 6; ++face)
  {
    gl_Layer = face;
    for(int vertex = 0; vertex < 3; ++vertex)
  	{
	  fragPos = gl_in[vertex].gl_Position;
	  gl_Position = pointLightMatrices[face] * fragPos;
	  EmitVertex();
	}
	EndPrimitive();
  }
}

