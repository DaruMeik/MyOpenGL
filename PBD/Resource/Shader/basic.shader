#shader vertex
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

out vec3 fragNorm;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float iTime;

void main()
{
   gl_Position = projection * view * model * (vec4(pos,1.0f) - vec4(0.0f, 0.01f*iTime, 0.0f, 0.0f));
   fragNorm = normal;
};


#shader fragment
#version 330 core
out vec4 fragColor;

in vec3 fragNorm;

uniform float iTime;
uniform vec3 iResolution;
uniform float t;

void main()
{
    fragColor = vec4(fragNorm/2.0f + 0.5f, 1.0f);
}