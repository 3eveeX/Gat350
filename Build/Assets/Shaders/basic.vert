#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_texcoord;

out vec3 v_Color;
out vec2 v_Texcoord;
uniform float u_time;

void main()
{
    gl_Position = vec4(0.2 * a_position / sin(cos(u_time)) +  vec3(sin(u_time), 0, cos(u_time)), 1.0);
    v_Color = a_color / -tan(1 - sin(u_time + cos(u_time) * -cos(gl_Position.x * gl_Position.y)));
    v_Texcoord = a_texcoord;
}