#shader vertex
#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
   gl_Position = projection * view * model * vec4(pos,0.0f,1.0f);
   v_TexCoord = texCoord;
};


#shader fragment
#version 330 core
out vec4 fragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
    fragColor = vec4(v_TexCoord, 0.0f, 1.0f);
    //fragColor = texture(u_Texture, v_TexCoord);
}