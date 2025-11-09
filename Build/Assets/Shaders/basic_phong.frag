#version 460 core

#define MAX_LIGHTS  5
#define POINT       0
#define DIRECTIONAL 1
#define SPOT        2

in VS_OUT{
     vec2 texcoord;
     vec3 position;
     vec3 normal;
} fs_in;

out vec4 f_color;

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
uniform sampler2D u_texture;
uniform int u_numLights;

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
        light_dir = normalize(light.direction);
        break;
    case SPOT:
    light_dir = normalize(light.position - position);
    light_distance = length(light.position - position);
    attenuation = calculateAttenuation(light_distance, light.range);

    float angle = acos(dot(light_dir, light.direction));
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
        //vec3 reflection = reflect(-light_dir, normal);
        //float specular_intensity = pow(max(dot(view_dir, reflection), 0.0), u_material.shininess);
        //vec3 specular = vec3(specular_intensity);
        
        //blinn phong
        vec3 half_dir = normalize(light_dir + view_dir);
        float specular_intensity = pow(max(dot(normal, half_dir), 0.0), u_material.shininess);
        vec3 specular = vec3(specular_intensity);

        //phong
        vec3 reflection = reflect(-light_dir, normal);
        //float specular_intensity = pow(max(dot(view_dir, reflection), 0.0), u_material.shininess);
        //vec3 specular = vec3(specular_intensity);




    return (diffuse + specular)*light.intensity * attenuation;
}

void main()
{
    vec3 color = u_ambientLight;
    for(int i = 0; i < u_numLights; i++){
        color += calculateLight(fs_in.position, fs_in.normal, u_lights[i]);
    }

    f_color = texture(u_texture, fs_in.texcoord) * vec4(color, 1);
}
