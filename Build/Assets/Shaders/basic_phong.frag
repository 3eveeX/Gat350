#version 460 core

#define MAX_LIGHTS  5

#define POINT       0
#define DIRECTIONAL 1
#define SPOT        2

#define  BASE_MAP     (1 << 0)
#define  SPECULAR_MAP (1 << 1)
#define  EMISSIVE_MAP (1 << 2)
#define  NORMAL_MAP	  (1 << 3)
in VS_OUT{
     vec2 texcoord;
     vec3 position;
     vec3 normal;
     mat3 tbn;
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
    
    vec3 baseColor;
    vec3 emissiveColor;

    float shininess;
    vec2 tiling;
    vec2 offset;
    uint parameters;
} u_material;
uniform sampler2D u_baseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_emissiveMap;
uniform sampler2D u_normalMap;

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

vec3 calculateNormal()
{
	// generate the normals from the normal map
	vec3 normal = texture(u_normalMap, fs_in.texcoord).rgb;
	// convert rgb normal (0 <-> 1) to xyz (-1 <-> 1)
    normal = normalize(normal * 2.0 - 1.0);
	// transform normals to model view space
	normal = normalize(fs_in.tbn * normal);

	return normal;
}

void main()
{
   
   

    vec4 emissive = ((u_material.parameters & EMISSIVE_MAP) != 0u) ? 
    texture(u_emissiveMap, fs_in.texcoord) * vec4(u_material.emissiveColor, 1.0) : vec4(u_material.emissiveColor, 1.0);

    vec3 normal = ((u_material.parameters & NORMAL_MAP) != 0u)
	? calculateNormal()
	: fs_in.normal;

	vec3 color = u_ambientLight;
    for(int i = 0; i < u_numLights; i++){
        color += calculateLight(fs_in.position, normal, u_lights[i]);
    }

    float specularMask = ((u_material.parameters & SPECULAR_MAP) != 0u) ? 
    texture(u_specularMap, fs_in.texcoord).r : 0.0;
    f_color = texture(u_baseMap, fs_in.texcoord) * vec4(color, 1.0) + emissive;

    //f_color = vec4(gl_FragCoord.x/1920, gl_FragCoord.y/1080, gl_FragCoord.z/gl_FragCoord.w, 1.0);
}
