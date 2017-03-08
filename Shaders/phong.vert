#version 410 core

layout (location = 0) in vec3 position_attr;
layout (location = 1) in vec4 colour_attr;
layout (location = 2) in vec3 normal_attr;
layout (location = 3) in vec2 texcoords_attr;

uniform highp mat4 ModelViewMatrix;
uniform highp mat3 NormalMatrix;
uniform highp mat4 ProjectionMatrix;

// Outputs
out VS_OUT
{
    vec3 N;
    vec3 L;
    vec3 V;
    vec4 C;
    vec2 uv;
} vs_out;

// Position of light
uniform vec3 light_pos = vec3(100.0, 100.0, 100.0);

void main(void)
{
    // Calculate view-space coordinate
    vec4 P = ModelViewMatrix * vec4(position_attr, 1);

    // Calculate normal in view-space
    vs_out.N = NormalMatrix * normal_attr;

    // Calculate light vector
    vs_out.L = light_pos - P.xyz;

    // Calculate view vector
    vs_out.V = -P.xyz;

    // Store the colour attribute
    vs_out.C = colour_attr;

    // Pass along the texture coordinates
    vs_out.uv = texcoords_attr;

    // Calculate the clip-space position of each vertex
    gl_Position = ProjectionMatrix * P;
}