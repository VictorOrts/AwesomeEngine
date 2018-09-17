#version 330 core
layout (location = 0) in vec3 vertex_position; 
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 vNormal;
uniform mat4 viewproject;
uniform mat4 model;
//Temas luces y sombras
uniform mat4 lightSpaceMatrix[2];
uniform int currentLight;

out vec2 uv_out;
out vec3 normal_out;
out vec3 vertex_out;
out vec4 FragPosLightSpace[2];
flat out int current_light;


void main(){
	uv_out = uv;
	gl_Position = viewproject * model * vec4(vertex_position,1.0f);
	vertex_out = vec3(model * vec4(vertex_position, 1.0f));
	normal_out = transpose(inverse(mat3(model))) * vNormal;
	
	FragPosLightSpace[0] = lightSpaceMatrix[0] * model *vec4(vertex_position, 1.0);
	FragPosLightSpace[1] = lightSpaceMatrix[1] * model *vec4(vertex_position, 1.0);
	current_light = currentLight;
}