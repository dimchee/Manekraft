#version 330 core
out vec4 sol;
in vec2 coord;

uniform sampler2D tex;

void main()
{
    vec4 col = texture(tex, coord);
    if(col.x < 0.1) discard;
    sol = col;
}