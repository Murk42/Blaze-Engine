#version 330 core

layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_norm;

uniform mat4 u_MVP;

out vec3 p_pos;
out vec3 p_norm;

void main()
{		
	p_pos = i_pos;
	p_norm = i_norm;

	gl_Position = u_MVP * vec4(i_pos, 1);
}