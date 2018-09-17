#version 330 core

layout (location = 0) in vec3 vertex_position; 
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 viewproject;
uniform mat4 model;

out vec3 vertex_out;
out vec2 texcoord_out;
out vec3 normal_out;

void main(){
	gl_Position = viewproject * model * vec4(vertex_position,1.0f);
	mat3 normalMat  = transpose(inverse(mat3(model)));
	normal_out = normalMat*normal;
	texcoord_out = uv;
	vertex_out = model *vec4(vertex_position,1.0f);
}