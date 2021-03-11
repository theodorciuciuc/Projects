#version 330

// TODO: get vertex attributes from each location
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoord;
//layout (location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 v_color;
uniform float time;
uniform float deformation;

out vec3 frag_normal;
out vec3 frag_color;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	frag_normal = v_normal;
	frag_color = v_color;
	if (deformation){
		gl_Position = Projection * View * Model * vec4(v_position.x, v_position.y + rand(vec2(v_normal.x, v_normal.y)) * abs(sin(time)) / 5, v_position.z, 1);
	} 
	else {
		gl_Position = Projection * View * Model * vec4(v_position, 1);
	}
}
