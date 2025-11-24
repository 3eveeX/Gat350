#version 430

#define GRAYSCALE	(1 << 0)
#define COLORTINT	(1 << 1)

in vec2 v_texcoord;

out vec4 f_color;

// post process controls
uniform uint u_parameters = 0;
uniform float u_time = 0;
uniform float u_intensity = 1.0;
uniform vec3 u_colorTint = vec3(1, 1, 1);

uniform sampler2D u_baseMap;

void main()
{
	vec4 color = texture(u_baseMap, v_texcoord);
	vec4 postprocess = color;

	//apply postprocess effects
	if ((u_parameters & GRAYSCALE) != 0)
	{
	float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
	postprocess = vec4(vec3(gray), color.a);
	}
	if ((u_parameters & COLORTINT) != 0)
	{
	postprocess.rgb = mix(color.rgb, u_colorTint, u_intensity);
	}
	
	f_color = postprocess;
}