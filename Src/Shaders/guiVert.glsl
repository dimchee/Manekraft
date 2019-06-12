#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform vec2 pos;
uniform vec2 scale;

out vec2 coord;

void main()
{
    gl_Position = vec4(aPos*scale + pos, -1.0, 1.0);
    coord = aTexCoord;
}