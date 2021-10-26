#V
#version 330 core

layout(location = 0) in vec4 position;
void main(){
   gl_Position = position;
};


#F
#version 330 core

layout(location = 0) out vec4 color;
void main(){
   color = vec4(0.5, 0.0, 0.5, 1.0);
};