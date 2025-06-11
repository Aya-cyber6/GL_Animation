#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;

uniform mat4 gVP;

out vec3 vColor;

void main()
{
    gl_Position = gVP * vec4(Position, 1.0);
    vColor = Color;
}
