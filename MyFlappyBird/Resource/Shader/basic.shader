#shader vertex
#version 330 core

layout(location = 0) in vec2 vPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
   gl_Position = projection * view * model * vec4(vPos,0.0f,1.0f);
};


#shader fragment
#version 330 core
out vec4 fragColor;

void main()
{
    fragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);
}