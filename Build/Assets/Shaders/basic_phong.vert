#version 460 core

#define MAX_LIGHTS  5
#define POINT       0
#define DIRECTIONAL 1
#define SPOT        2

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

out VS_OUT{
     vec2 texcoord;
     vec3 position;
     vec3 normal;
} vs_out;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform int u_numLights;

uniform struct Light{
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    float range;
    float intensity;
    float outerCutoff;
    float innerCutoff;
} u_lights[MAX_LIGHTS];

uniform struct Material{
    sampler2D baseMap;
    vec3 baseColor;

    float shininess;
    vec2 tiling;
    vec2 offset;
} u_material;


uniform vec3 u_ambientLight;

void main()
{

vs_out.texcoord = a_texcoord * u_material.tiling + u_material.offset;
mat4 model_view = u_view * u_model;
vs_out.position = vec3(model_view * vec4(a_position, 1.0));
vs_out.normal = normalize(mat3(model_view) * a_normal);

gl_Position = u_projection * model_view * vec4(a_position, 1.0);
}