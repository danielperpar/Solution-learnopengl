#version 330 core
out vec4 FragColor;
in vec4 pos;

void main()
{ 
   if(pos.x >= 0.0f)
   {
     if(abs(pos.y) <= pos.x)
     {
      FragColor = vec4(0.0f,0.0f,0.0f,1.0f);
     }
     else
     {
      FragColor = vec4(1.0f,1.0f,0.0f,1.0f); 
     }
   }
}