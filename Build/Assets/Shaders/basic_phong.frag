#version 460 core
in vec2 v_texcoord;
in vec3 v_position;
in vec3 v_normal;

out vec4 f_color;

uniform struct Light
{
	vec3 position;
	vec3 color;
} u_light;

uniform struct Material{
    sampler2D baseMap;
    vec3 baseColor;

    float shininess;
    vec2 tiling;
    vec2 offset;
} u_material;

uniform vec3 u_ambient_light;
uniform sampler2D u_texture;

vec3 calculateLight(in vec3 position, in vec3 normal)
{
//diffuse
    vec3 light_dir = normalize(u_light.position - position);
    float intensity = max(dot(normal, light_dir),0);
    vec3 diffuse = u_light.color * intensity * u_material.baseColor;

    //specular
        vec3 view_dir = normalize(-position);
        vec3 halfway_dir = normalize(light_dir + view_dir);
        float specular_intensity = pow(max(dot(normal, halfway_dir), 0.0), u_material.shininess);
        vec3 specular = vec3(specular_intensity);

    return u_ambient_light + diffuse + specular;
}

void main()
{
	vec3 color = calculateLight(v_position, v_normal);
	f_color = texture(u_texture, v_texcoord) * vec4(color, 1);
}
