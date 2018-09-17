#version 330 core
#define MAX_LIGHT 5
struct Material{
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float shininess;
};
struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    float intesity;
	//PointLight
    float constant;
    float linear;
    float quadratic;
	//SpotLight
	float cutOff;
	float outerCutOff;

};

uniform Material mat;
uniform Light mylight[2];
in vec2 uv_out;
in vec3 vertex_out;
in vec3 normal_out;
//Tema luces sombras
in vec4 FragPosLightSpace[2];
flat in int current_light;
uniform sampler2D shadowMap;
uniform sampler2D shadowMap2;

uniform sampler2D diffuse_tex;
uniform sampler2D specular_tex;
uniform vec3 CameraPos;

out vec4 FragColor;

//FUNCTIONS
float ShadowCalculation(vec4 fragPosLightSpace,vec3 lightPos,sampler2D shadowMapp);
vec3 DirLight  (Light rlight,vec2 TexCoords,vec3 normal,vec3 viewDir);
vec3 PointLight(Light rlight,vec2 TexCoords,vec3 normal,vec3 FragPos,vec3 viewDir);
vec3 SpotLight(Light rlight,vec2 TexCoords,vec3 normal,vec3 FragPos,vec3 viewDir);
//END FUNCTIONS
void main(){

	vec3 norm = normalize(normal_out);
	vec3 viewDir = normalize(CameraPos - vertex_out);
	vec2 newtexc = vec2(uv_out.x,1.0f-uv_out.y);
	vec3 result;
	if(current_light>0){
	
	float shadow = ShadowCalculation(FragPosLightSpace[0],mylight[0].position,shadowMap);
	float shadow2 = ShadowCalculation(FragPosLightSpace[1],mylight[1].position,shadowMap2); 
	vec3 dir1 = DirLight(mylight[0],newtexc,norm,viewDir);
	vec3 dir2 = DirLight(mylight[1],newtexc,norm,viewDir);
	//shadow = min(shadow, 0.75);
	//shadow2 = min(shadow2, 0.75);
	result = dir1*shadow;
	result += dir2*shadow2;
	float gamma = 1.0;
    result.rgb = pow(result.rgb,vec3(1.0f/gamma));


	FragColor = vec4(result,1);
	}
	else{
	FragColor = texture(diffuse_tex,newtexc);
	}
	//
}

vec3 DirLight(Light rlight,vec2 TexCoords, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-rlight.direction);
	//diff
	float diff = max(dot(normal, lightDir), 0.0);
	//spec
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
	vec3 ambient =  rlight.color * mat.ambient * vec3(texture(diffuse_tex, TexCoords));
	vec3 diffuse =  diff * rlight.color*mat.diffuse *vec3(texture(diffuse_tex, TexCoords));
	vec3 specular = mat.specular *spec* vec3(texture(specular_tex,TexCoords));
    return (ambient + diffuse + specular)*rlight.intesity;
}

vec3 PointLight(Light rlight,vec2 TexCoords,vec3 normal,vec3 FragPos,vec3 viewDir){
	vec3 lightDir = normalize(rlight.position - FragPos);
	float diff = max(dot(normal,lightDir),0.0);
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
	float distance = length(rlight.position- FragPos);
	float attenuation = 1.0f/(rlight.constant + rlight.linear * distance + rlight.quadratic * (distance*distance));
	vec3 ambient = rlight.color * mat.ambient * vec3(texture(diffuse_tex,TexCoords));
	vec3 diffuse = diff * rlight.color*mat.diffuse *vec3(texture(diffuse_tex, TexCoords));
	vec3 specular = mat.specular *spec* vec3(texture(specular_tex,TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}


vec3 SpotLight(Light rlight,vec2 TexCoords,vec3 normal,vec3 FragPos,vec3 viewDir)
{
    vec3 lightDir = normalize(rlight.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    float distance = length(rlight.position - FragPos);
    float attenuation = 1.0f / (rlight.constant + rlight.linear * distance + rlight.quadratic * (distance * distance));    
    float theta = dot(lightDir, normalize(-rlight.direction)); 
    float epsilon = rlight.cutOff - rlight.outerCutOff;
    float intensity = clamp((theta - rlight.outerCutOff) / epsilon, 0.0, 1.0);
    vec3 ambient = rlight.color * mat.ambient * vec3(texture(diffuse_tex,TexCoords));
	vec3 diffuse = diff * rlight.color*mat.diffuse *vec3(texture(diffuse_tex, TexCoords));
	vec3 specular = mat.specular *spec* vec3(texture(specular_tex,TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

float ShadowCalculation(vec4 fragPosLightSpace,vec3 lightPos,sampler2D shadowMapp)
{

	
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMapp, projCoords.xy).r; 
    float currentDepth = projCoords.z;

   vec3 normal = normalize(normal_out);
    vec3 lightDir = normalize(lightPos - vertex_out);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMapp, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMapp, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias < pcfDepth  ? 1.0 : 0.5;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}