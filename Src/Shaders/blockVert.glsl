#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
uniform mat4 proj;
uniform mat4 view;
uniform vec3 pos;

out vec2 coord;

void main()
{
    gl_Position = proj * view * vec4(aPos + pos, 1.0);
    coord = aTexCoord;
}