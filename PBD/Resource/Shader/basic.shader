#shader vertex
#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texCoord;

out vec2 fragCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
   gl_Position = projection * view * model * vec4(pos,0.0f,1.0f);
   fragCoord = texCoord;
};


#shader fragment
#version 330 core
out vec4 fragColor;

in vec2 fragCoord;

uniform float iTime;
uniform vec3 iResolution;
uniform float t;

void main()
{
    fragColor = vec4(fragCoord * cos(iTime), 0.0f, 1.0f);
}