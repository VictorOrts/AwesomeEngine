#version 330 core;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 vertex_out;
in vec2 texcoord_out;
in vec3 normal_out;
uniform sampler2D texture_diff;
uniform sampler2D texture_spec;

void main(){
	gPosition = vertex_out;
	gNormal = normalize(normal_out);
	gAlbedoSpec = texture(texture_diff,texcoord_out).rgb;
	gAlbedoSpec.a = texture(texture_spec,texcoord_out).r;
}