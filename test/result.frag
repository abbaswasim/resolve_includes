#version 400 core

layout (location = 0) out vec4 position;

in vec3 pos;
in vec2 uv;

uniform sampler2D albedo;

float depth_squared(float depth)
{
    return depth * depth;
}

vec3 normal(vec3 n)
{
    return normalize(n);
}

out vec4 color;

void main()
{
    color = vec4(1.0);
}