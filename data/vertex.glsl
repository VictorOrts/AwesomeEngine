#version 330 core
layout (location = 0) in vec3 vertex_position; 
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 vNormal;
//uniform mat4 viewproject;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main(){
	gl_Position = lightSpaceMatrix * model * vec4(vertex_position,1.0);

}