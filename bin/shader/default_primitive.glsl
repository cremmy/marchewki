/**** Vertex shader ****/
#pragma wtsngine vertex
#version 330 core

layout(location=0) in vec3 in_position;
//layout(location=1) in vec2 in_uv;

uniform vec4 u_color;

uniform Uniforms
	{
	mat4 view;
	mat4 projection;
	} u;

void main()
	{	
	gl_Position=u.projection*u.view*vec4(in_position, 1.0f);
	}

/**** Fragment shader ****/
#pragma wtsngine fragment
#version 330 core

uniform vec4 u_color;
	
out vec4 out_color;

void main()
	{
	out_color=u_color;
	gl_FragDepth=gl_FragCoord.z;
	}