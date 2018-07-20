/**** Vertex shader ****/
#pragma wtsngine vertex
#version 330 core

layout(location=0) in vec3 in_position;
layout(location=1) in vec2 in_uv;

uniform mat4 u_model;

uniform Uniforms
	{
	mat4 view;
	mat4 projection;
	} u;
	
out VertexData
	{
	vec2 uv;
	} o;

void main()
	{
	gl_Position=u.projection*u.view*u_model*vec4(in_position, 1.0f);
	
	o.uv=in_uv;
	}

/**** Fragment shader ****/
#pragma wtsngine fragment
#version 330 core

//#pragma include base.fs
//#pragma include ../../base2.fs

in VertexData
	{
	vec2 uv;
	} i;
	
uniform vec4 u_color;
uniform sampler2D u_texture;
	
out vec4 out_color;

void main()
	{
	out_color=texture(u_texture, i.uv)*u_color;
	
	if(out_color.a<1.0f/256.f)
		discard;
	
	gl_FragDepth=gl_FragCoord.z;
	}
