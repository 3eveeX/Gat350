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
 vec3 color;
}vs_out;


uniform int u_numLights = 1;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform vec3 u_ambientLight;


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

float calculateAttenuation(in float light_distance, in float range){


float attenuation = max( 0.0, (1.0f - (light_distance/range)));
return attenuation * attenuation;
}
vec3 calculateLight(in vec3 position, in vec3 normal, in Light light){
    vec3 light_dir;
    float attenuation = 1.0f;
    float light_distance;
switch (light.type){
    case POINT:
        light_dir = normalize(light.position - position);
        light_distance = length(light.position - position);
        attenuation = calculateAttenuation(light_distance, light.range);
        break;
    case DIRECTIONAL:
        light_dir = -light.direction;
        break;
    case SPOT:
    light_dir = normalize(light.position - position);
    light_distance = length(light.position - position);
    attenuation = calculateAttenuation(light_distance, light.range);

    float angle = acos(dot(light_dir, -light.direction));
    if(angle > light.outerCutoff){
    attenuation = 0.0;
    } else if(angle > light.innerCutoff){
    float spotAttenuation = smoothstep(light.outerCutoff, light.innerCutoff, angle);
    attenuation *= spotAttenuation;
    }
        break;
    default:
        break;
}

//diffuse
    float NdotL = max(dot(normal, light_dir),0);
    vec3 diffuse = light.color * NdotL * u_material.baseColor;

    //specular
        vec3 view_dir = normalize(-position);
        vec3 reflection = reflect(-light_dir, normal);
        float specular_intensity = pow(max(dot(view_dir, reflection), 0.0), u_material.shininess);
        vec3 specular = vec3(specular_intensity);

        //blinn phong
        //vec3 half_dir = normalize(light_dir + view_dir);
        //float specular_intensity = pow(max(dot(normal, half_dir), 0.0), u_material.shininess);
        //vec3 specular = vec3(specular_intensity);

        //phong
        //vec3 reflection = reflect(-light_dir, normal);
        //float specular_intensity = pow(max(dot(view_dir, reflection), 0.0), u_material.shininess);
        //vec3 specular = vec3(specular_intensity);




    return (diffuse + specular)*light.intensity * attenuation;
}

void main()
{

vs_out.texcoord = a_texcoord * u_material.tiling + u_material.offset;
mat4 model_view = u_view * u_model;
vec3 position = vec3(model_view * vec4(a_position, 1.0));
vec3 normal = normalize(mat3(model_view) * a_normal);
vs_out.color = u_ambientLight;
for(int i = 0; i < u_numLights; i++){
vs_out.color += calculateLight(position, normal, u_lights[i]);
}
gl_Position = u_projection * model_view * vec4(a_position, 1.0);
}