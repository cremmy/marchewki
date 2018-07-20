/**** Vertex shader ****/
#pragma wtsngine vertex
#version 330 core

layout(location=0) in vec3 in_position;
layout(location=1) in vec2 in_uv;
layout(location=2) in vec3 in_normal;
layout(location=3) in vec3 in_tangent;

uniform mat4 u_model;

layout(std140) uniform Uniforms
	{
	mat4 view;
	mat4 projection;
	} u;
	
out VertexData
	{
	vec3 position;
	vec2 uv;
	vec3 cam_direction;
	mat3 TBN;
	} o;

void main()
	{
	gl_Position=u.projection*u.view*u_model*vec4(in_position, 1.0f);
	
	o.position=(u.view*u_model*vec4(in_position, 1.0f)).xyz;
	o.uv=in_uv;
	o.cam_direction=-normalize(o.position);
	
	vec3 normal=normalize((u_model*vec4(in_normal, 0.0f)).xyz);
	vec3 tangent=normalize((u_model*vec4(in_tangent, 0.0f)).xyz);
	vec3 bitangent=normalize(cross(normal, tangent));
	o.TBN=transpose(mat3(tangent, bitangent, normal));
	}

/**** Fragment shader ****/
#pragma wtsngine fragment
#version 330 core
	
uniform vec4 u_color;
uniform sampler2D u_texture;
uniform sampler2D u_normal;

//uniform mat4 u_model;

layout(std140) uniform Uniforms
	{
	mat4 view;
	mat4 projection;
	} u;
	
layout(std140) uniform Light
	{
	vec4 direction;
	vec4 color;
	} l;
	
layout(std140) uniform Material
	{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float specularexp;
	float transparency;
	} m;
	
in VertexData
	{
	vec3 position;
	vec2 uv;
	vec3 cam_direction;
	mat3 TBN;
	} i;
	
out vec4 out_color;

void main()
	{
	vec4 baseColor=vec4(m.diffuse.rgb, 1)*u_color*texture(u_texture, i.uv);
	vec3 fNormal=texture(u_normal, i.uv).rgb*2.0-vec3(1, 1, 1);
	vec3 fLight=i.TBN*l.direction.xyz;

	// Ambient
	out_color=vec4(m.ambient.rgb, m.transparency)*baseColor*vec4(l.color.rgb, 1);
	// Diffuse
	out_color.rgb+=baseColor.rgb*l.color.rgb*clamp(dot(fLight, fNormal), 0.0, 1.0);
	// Specular
	vec3 spec_reflection=reflect(-fLight, fNormal);
	float spec_cosAlpha=max(dot(i.cam_direction, spec_reflection), 0.0);
	out_color.rgb+=m.specular.rgb*pow(spec_cosAlpha, m.specularexp);
	// Przezroczystosc
	out_color.a*=m.transparency;
	
	//if(out_color.a<1.0f/256.f)
	//	discard;
	
	gl_FragDepth=gl_FragCoord.z;
	}
