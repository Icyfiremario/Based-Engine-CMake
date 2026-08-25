#version 330 core

in vec3 aPos;
in vec3 aColor;

out vec3 ourColor;

uniform float u_time;

void main()
{
    gl_Position = vec4 (aPos, 1.0);
    ourColor = aColor * sin(u_time);
}
