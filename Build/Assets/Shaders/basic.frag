#version 460 core
in vec3 v_Color;
in vec2 v_Texcoord;
out vec4 f_color;

uniform float u_time;
uniform sampler2D u_texture;

void main(){
    //f_color = vec4(1, 1, 0, 1.0);
    float offset = sin(u_time * gl_FragCoord.x)+gl_FragCoord.y;
    //f_color = vec4(v_Color + gl_FragCoord.y / offset, 1.0);

    f_color = texture(u_texture, v_Texcoord) * vec4(v_Color + gl_FragCoord.y / offset, 1.0);
}