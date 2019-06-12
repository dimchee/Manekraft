#version 330 core
out vec4 sol;
in vec2 coord;

uniform sampler2D tex;

void main()
{
    sol = texture(tex, coord);
}